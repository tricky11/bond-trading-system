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
  string id = split[0];
  auto bond = BondProductService::GetInstance()->GetData(id);
  vector<Order> bidStack;
  vector<Order> offerStack;
  for (int i = 1; i <= 5; ++i) {
    Order bid(stod(split[i]), stol(split[i + 1]), PricingSide::BID);
    Order offer(stod(split[10 + i]), stol(split[11 + i]), PricingSide::OFFER);
    bidStack.push_back(bid);
    offerStack.push_back(offer);
  }
  return OrderBook<Bond>(bond, bidStack, offerStack);
}

BondMarketDataConnector::BondMarketDataConnector(const string &filePath,
                                                 Service<string, OrderBook<Bond>> *connectedService)
    : InputFileConnector(filePath, connectedService) {}

void BondMarketDataService::OnMessage(OrderBook<Bond> &data) {
  if (dataStore.find(data.GetProduct().GetProductId()) == dataStore.end()) {
    dataStore[data.GetProduct().GetProductId()] = data;
    for (auto listener:this->GetListeners()) {
      listener->ProcessAdd(data);
    }
  } else {
    dataStore[data.GetProduct().GetProductId()] = data;
    for (auto listener:this->GetListeners()) {
      listener->ProcessUpdate(data);
    }
  }
}

void BondMarketDataService::Subscribe(BondMarketDataConnector *connector) {
  connector->read();
}
const BidOffer &BondMarketDataService::GetBestBidOffer(const string &productId) {
  if (!(dataStore.find(productId) == dataStore.end())) {
    OrderBook<Bond> orderBook = dataStore[productId];
    BidOffer bidOffer
        (Order(orderBook.GetBidStack()[0].GetPrice(), orderBook.GetBidStack()[0].GetQuantity(), PricingSide::BID),
         Order(orderBook.GetOfferStack()[0].GetPrice(),
               orderBook.GetOfferStack()[0].GetQuantity(),
               PricingSide::OFFER));
    return bidOffer;
  }
}
const OrderBook<Bond> &BondMarketDataService::AggregateDepth(const string &productId) {
  if (!(dataStore.find(productId) == dataStore.end())) {
    OrderBook<Bond> orderBook = dataStore[productId];
    double totalBidCost = 0.0;
    long totalBidVolume = 0;
    double totalOfferCost = 0.0;
    long totalOfferVolume = 0;
    for (int i = 0; i < 5; ++i) {
      totalBidVolume += orderBook.GetBidStack()[i].GetQuantity();
      totalBidCost += orderBook.GetBidStack()[i].GetQuantity() * orderBook.GetBidStack()[i].GetPrice();
      totalOfferVolume += orderBook.GetOfferStack()[i].GetQuantity();
      totalOfferCost += orderBook.GetOfferStack()[i].GetQuantity() * orderBook.GetOfferStack()[i].GetPrice();
    }
    double averageBidPrice = totalBidVolume / totalBidCost;
    double averageOfferPrice = totalOfferVolume / totalOfferCost;
    vector<Order> aggregatedBidStack({Order(averageBidPrice, totalBidVolume, PricingSide::BID)});
    vector<Order> aggregatedOfferStack({Order(averageOfferPrice, totalOfferVolume, PricingSide::OFFER)});
    OrderBook<Bond> aggregateOrderBook(orderBook.GetProduct(), aggregatedBidStack, aggregatedOfferStack);
    return aggregateOrderBook;
  }
}

#endif //BONDTRADINGSYSTEM_BONDMARKETDATASERVICE_H
