//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDTRADEBOOKINGSERVICE_H
#define BONDTRADINGSYSTEM_BONDTRADEBOOKINGSERVICE_H

#include <sstream>
#include "../../base/products.hpp"
#include "../../base/tradebookingservice.hpp"
#include "../../base/executionservice.hpp"
#include "../../connectors/InputFileConnector.hpp"
#include "../../utils/string.h"

/**
 * Reads data from trades.csv
 */
class BondTradesConnector : public InputFileConnector<string, Trade<Bond>> {
 public:
  BondTradesConnector(const string &filePath, Service<string, Trade<Bond>> *connectedService);
 private:
  void parse(string line) override;
};

/**
 * Processes trades.
 */
class BondTradeBookingService : public TradeBookingService<Bond> {
 public:
  BondTradeBookingService() {}
  void Subscribe(BondTradesConnector *connector);
  void OnMessage(Trade<Bond> &data) override;
  void BookTrade(const Trade<Bond> &trade) override;
};

void BondTradesConnector::parse(string line) {
  auto split = splitString(line, ',');
  string productId = split[0], tradeId = split[1], bookId = split[3];
  double price = stod(split[2]);
  long quantity = stol(split[4]);
  Side side = split[5].compare("0") == 0 ? Side::BUY : Side::SELL;

  auto bond = BondProductService::GetInstance()->GetData(productId);
  auto trade = Trade<Bond>(bond, tradeId, price, bookId, quantity, side);
  connectedService->OnMessage(trade);
}
BondTradesConnector::BondTradesConnector(const string &filePath, Service<string, Trade<Bond>> *connectedService)
    : InputFileConnector(filePath, connectedService) {}

/**
 * Store the new trade data.
 * @param data
 */
void BondTradeBookingService::OnMessage(Trade<Bond> &data) {
  // TODO: Check if trade has already been processed (Not part of project, since we never generate duplicate trades)
  dataStore.insert(make_pair(data.GetTradeId(), data));
  BookTrade(data);
}

void BondTradeBookingService::Subscribe(BondTradesConnector *connector) {
  connector->read();
}

/**
 * Notify all listeners of the new trade.
 * @param trade
 */
void BondTradeBookingService::BookTrade(const Trade<Bond> &trade) {
  for (auto listener : this->GetListeners()) {
    listener->ProcessAdd(const_cast<Trade<Bond> &>(trade));
  }
}

class BondExecutionServiceListener : public ServiceListener<ExecutionOrder<Bond>> {
 private:
  BondTradeBookingService *listeningService;
  std::array<string, 3> states = {{"TRSY1", "TRSY2", "TRSY3"}};
  unsigned int currentState = 0;
  void cycleState() {
    currentState = (currentState + 1) % states.size();
  }

 public:
  BondExecutionServiceListener(BondTradeBookingService *listeningService) : listeningService(listeningService) {}
  void ProcessAdd(ExecutionOrder<Bond> &data) override {

    // This is called by the ExecutionService after deciding to execute a trade.
    // TODO: Generate tradeId here
    Trade<Bond> trade(data.GetProduct(),
                      "tradeid",
                      data.GetPrice(),
                      states[currentState],
                      data.GetVisibleQuantity() + data.GetHiddenQuantity(),
                      data.GetSide() == OFFER ? BUY : SELL);
    listeningService->BookTrade(trade);
  }

  void ProcessRemove(ExecutionOrder<Bond> &data) override {
    // NO-OP : ExecutionOrders are never removed in this project.
  }

  void ProcessUpdate(ExecutionOrder<Bond> &data) override {
    // NO-OP : ExecutionOrders are never updated in this project.
  }
};

#endif //BONDTRADINGSYSTEM_BONDTRADEBOOKINGSERVICE_H
