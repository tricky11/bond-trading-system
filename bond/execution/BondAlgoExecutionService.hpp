//
// Created by Harish Reddy on 12/13/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDALGOEXECUTIONSERVICE_H
#define BONDTRADINGSYSTEM_BONDALGOEXECUTIONSERVICE_H

#include "../../base/positionservice.hpp"
#include "../../base/products.hpp"
#include "../../base/soa.hpp"
#include "../../base/executionservice.hpp"

template<typename T>
class AlgoExecution {
 public:
  explicit AlgoExecution(const ExecutionOrder<T> &executionOrder) : executionOrder(executionOrder) {}

  const ExecutionOrder<T> &getExecutionOrder() const {
    return executionOrder;
  }

 private:
  const ExecutionOrder<T> &executionOrder;
};

/**
 * Listens to updates from the BondMarketDataService
 * and acts on the OrderBook information as it becomes available.
 */
class BondAlgoExecutionService : public Service<string, AlgoExecution<Bond>> {
 public:
  BondAlgoExecutionService() {}

  /**
 * Process an OrderBook update.
 * If the spread is tightest, execute the full volume available.
 * Alternate between BID and OFFER.
 */
  void ProcessOrderBook(OrderBook<Bond> &orderBook) {
    auto topBid = orderBook.GetBidStack()[0];
    auto topOffer = orderBook.GetOfferStack()[0];
    double spread = topOffer.GetPrice() - topBid.GetPrice();
    if (spread <= 1.0 / 128) {

      long volume = states[currentState] == BID ? topBid.GetQuantity()
                                                : topOffer.GetQuantity();
      double price = states[currentState] == BID ? topBid.GetPrice()
                                                 : topOffer.GetPrice();

      ExecutionOrder<Bond>
          executionOrder
          (orderBook.GetProduct(),
           states[currentState],
           "Order_" + to_string(orderNumber),
           MARKET,
           price,
           volume,
           0,
           "",
           false);
      AlgoExecution<Bond> algoExecution(executionOrder);
      for (auto listener : this->GetListeners()) {
        listener->ProcessAdd(algoExecution);
      }
      cycleState();
    }
  }

  void OnMessage(AlgoExecution<Bond> &data) override {

  }

 private:
  std::array<PricingSide, 2> states = {{PricingSide::BID, PricingSide::OFFER}};
  unsigned int currentState = 0;
  void cycleState() {
    currentState = (currentState + 1) % states.size();
    orderNumber++;
  }
  unsigned long int orderNumber = 1;
};

class BondMarketDataServiceListener : public ServiceListener<OrderBook<Bond>> {
 public:
  explicit BondMarketDataServiceListener(BondAlgoExecutionService *listeningService)
      : listeningService(listeningService) {}

  void ProcessAdd(OrderBook<Bond> &data) override {
    listeningService->ProcessOrderBook(data);
  }

  void ProcessRemove(OrderBook<Bond> &data) override {
    // An OrderBook is never removed.
  }

  void ProcessUpdate(OrderBook<Bond> &data) override {
    listeningService->ProcessOrderBook(data);
  }

 private:
  BondAlgoExecutionService *listeningService;
};

#endif //BONDTRADINGSYSTEM_BONDALGOEXECUTIONSERVICE_H
