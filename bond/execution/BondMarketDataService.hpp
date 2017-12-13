//
// Created by Harish Reddy on 12/13/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDMARKETDATASERVICE_H
#define BONDTRADINGSYSTEM_BONDMARKETDATASERVICE_H

#include <sstream>
#include "../../base/products.hpp"
#include "../../base/marketdataservice.hpp"
#include "../../connectors/InputFileConnector.hpp"
#include "../../utils/string.h"

class BondMarketDataConnector : public InputFileConnector<string, OrderBook<Bond>> {
 public:
  BondMarketDataConnector(const string &filePath, Service<string, OrderBook<Bond>> *connectedService);
 private:
  OrderBook<Bond> parse(string line) override;
};

class BondMarketDataService : public MarketDataService<Bond> {
 public:
  const BidOffer &GetBestBidOffer(const string &productId) override;
  const OrderBook<Bond> &AggregateDepth(const string &productId) override;
  void Subscribe(BondMarketDataConnector *connector);
  void OnMessage(OrderBook<Bond> &data) override;
};

OrderBook<Bond> BondMarketDataConnector::parse(string line) {
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
  return OrderBook<Bond>(product, vector<Order>(), vector<Order>());
}

BondMarketDataConnector::BondMarketDataConnector(const string &filePath,
                                                 Service<string, OrderBook<Bond>> *connectedService)
    : InputFileConnector(filePath, connectedService) {}

void BondMarketDataService::OnMessage(OrderBook<Bond> &data) {
  std::cout << "OnMessage in BondMarketDataService" << std::endl;
  dataStore.insert({"dummy", data});
  for(auto listener:this->GetListeners()){
    listener->ProcessAdd(data);
  }
}

void BondMarketDataService::Subscribe(BondMarketDataConnector *connector) {
  connector->read();
}
const BidOffer &BondMarketDataService::GetBestBidOffer(const string &productId) {
  BidOffer bidOffer(Order(99.0, 100000, PricingSide::BID), Order(101.0, 200000, PricingSide::OFFER));
  return bidOffer;
}
const OrderBook<Bond> &BondMarketDataService::AggregateDepth(const string &productId) {
  Bond product("bondid", BondIdType::CUSIP, "ticker", 2.0, boost::gregorian::date());
  OrderBook<Bond> book(product, vector<Order>(), vector<Order>());
  return book;
}

#endif //BONDTRADINGSYSTEM_BONDMARKETDATASERVICE_H
