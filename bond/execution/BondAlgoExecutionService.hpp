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

class BondAlgoExecutionService : public Service<string, AlgoExecution<Bond>> {
 public:
  void ProcessOrderBook(OrderBook<Bond> &orderBook) {
    auto product = Bond("id", BondIdType::CUSIP, "ticker", 2.0, boost::gregorian::date());

    ExecutionOrder<Bond> executionOrder
        (product, PricingSide::BID, "orderid", OrderType::LIMIT, 100.0, 100000, 200000, "parentorderId", true);
    AlgoExecution<Bond> algoExecution(executionOrder);
    for (auto listener : this->GetListeners()) {
      listener->ProcessAdd(algoExecution);
    }
  }

  void OnMessage(AlgoExecution<Bond> &data) override {

  }
};

class BondMarketDataServiceListener : public ServiceListener<OrderBook<Bond>> {
 public:
  explicit BondMarketDataServiceListener(BondAlgoExecutionService *listeningService)
      : listeningService(listeningService) {}

  void ProcessAdd(OrderBook<Bond> &data) override {
    std::cout << "ProcessAdd in BondMarketDataServiceListener" << std::endl;
    listeningService->ProcessOrderBook(data);
  }

  void ProcessRemove(OrderBook<Bond> &data) override {

  }
  void ProcessUpdate(OrderBook<Bond> &data) override {

  }

 private:
  BondAlgoExecutionService *listeningService;
};

#endif //BONDTRADINGSYSTEM_BONDALGOEXECUTIONSERVICE_H
