//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDTRADEBOOKINGSERVICE_H
#define BONDTRADINGSYSTEM_BONDTRADEBOOKINGSERVICE_H

#include <sstream>
#include "../../base/products.hpp"
#include "../../base/tradebookingservice.hpp"
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
  string id = split[0], ticker = split[2], maturityDate = split[4];
  BondIdType bondIdType = stoi(split[1]) == 0 ? BondIdType::CUSIP : BondIdType::ISIN;
  float coupon = stof(split[3]);
  double mid = stod(split[5]), bidOfferSpread = stod(split[6]);

  //TODO: Fix boost date here.
  //TODO: Bond goes out of scope after return.
  auto product =
      Bond(id,
           bondIdType,
           ticker,
           coupon,
           boost::gregorian::date());
  return Trade<Bond>(product, "tradeid", mid, "bookid", 0, Side::BUY);
}
BondTradesConnector::BondTradesConnector(const string &filePath, Service<string, Trade<Bond>> *connectedService)
    : InputFileConnector(filePath, connectedService) {}

void BondTradeBookingService::OnMessage(Trade<Bond> &data) {
  std::cout << "OnMessage in BondTradeBookingService" << std::endl;
  dataStore.insert({"dummy", data});
  BookTrade(data);
}

void BondTradeBookingService::Subscribe(BondTradesConnector *connector) {
  connector->read();
}

void BondTradeBookingService::BookTrade(const Trade<Bond> &trade) {
  for (auto listener : this->GetListeners()) {
    listener->ProcessAdd(const_cast<Trade<Bond> &>(trade));
  }
}
#endif //BONDTRADINGSYSTEM_BONDTRADEBOOKINGSERVICE_H
