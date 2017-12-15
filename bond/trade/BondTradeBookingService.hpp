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

class BondTradesConnector : public InputFileConnector<string, Trade<Bond>> {
 public:
  BondTradesConnector(const string &filePath, Service<string, Trade<Bond>> *connectedService);
 private:
  Trade<Bond> parse(string line) override;
};

class BondTradeBookingService : public TradeBookingService<Bond> {
 public:
  void Subscribe(BondTradesConnector *connector);
  void OnMessage(Trade<Bond> &data) override;
  void BookTrade(const Trade<Bond> &trade) override;
};

Trade<Bond> BondTradesConnector::parse(string line) {
  auto split = splitString(line, ',');
  string productId = split[0], tradeId = split[1], bookId = split[3];
  double price = stod(split[2]);
  long quantity = stol(split[4]);
  Side side = split[5].compare("0") == 0 ? Side::BUY : Side::SELL;

  auto bond = BondProductService::GetInstance()->GetData(productId);
  return Trade<Bond>(bond, tradeId, price, bookId, quantity, side);
}
BondTradesConnector::BondTradesConnector(const string &filePath, Service<string, Trade<Bond>> *connectedService)
    : InputFileConnector(filePath, connectedService) {}

void BondTradeBookingService::OnMessage(Trade<Bond> &data) {
  dataStore[data.GetTradeId()] = data;
  BookTrade(data);
}

void BondTradeBookingService::Subscribe(BondTradesConnector *connector) {
  connector->read();
}

void BondTradeBookingService::BookTrade(const Trade<Bond> &trade) {
  // TODO: Check if trade has already been processed (Not part of project, since we never generate duplicate trades)
  for (auto listener : this->GetListeners()) {
    listener->ProcessAdd(const_cast<Trade<Bond> &>(trade));
  }
}

class BondExecutionServiceListener : public ServiceListener<ExecutionOrder<Bond>> {
 private:
  BondTradeBookingService *listeningService;
 public:
  BondExecutionServiceListener(BondTradeBookingService *listeningService) : listeningService(listeningService) {}
  void ProcessAdd(ExecutionOrder<Bond> &data) override {
    // TODO: Generate tradeId here
    Trade<Bond> trade(data.GetProduct(), "tradeid", data.GetPrice(), "bookid", 100000, Side::BUY);
    listeningService->BookTrade(trade);
  }

  void ProcessRemove(ExecutionOrder<Bond> &data) override {

  }

  void ProcessUpdate(ExecutionOrder<Bond> &data) override {

  }
};

#endif //BONDTRADINGSYSTEM_BONDTRADEBOOKINGSERVICE_H
