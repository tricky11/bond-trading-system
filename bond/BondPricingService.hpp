//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDPRICINGSERVICE_H
#define BONDTRADINGSYSTEM_BONDPRICINGSERVICE_H

#include <sstream>
#include "../base/products.hpp"
#include "../base/pricingservice.hpp"
#include "../connectors/InputFileConnector.hpp"
#include "../utils/string.h"

class BondPricesConnector : public InputFileConnector<string, Price<Bond>> {
 public:
  BondPricesConnector(const string &filePath, Service<string, Price<Bond>> *connectedService);
 private:
  Price<Bond> parse(string line) override;
};

class BondPricingService : public PricingService<Bond> {
 public:
  void Subscribe(BondPricesConnector *connector);
  void OnMessage(Price<Bond> &data) override;
};

Price<Bond> BondPricesConnector::parse(string line) {
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
  return Price<Bond>(product, mid, bidOfferSpread);
}
BondPricesConnector::BondPricesConnector(const string &filePath, Service<string, Price<Bond>> *connectedService)
    : InputFileConnector(filePath, connectedService) {}

void BondPricingService::OnMessage(Price<Bond> &data) {
  std::cout << "OnMessage in BondPricingService" << std::endl;
  dataStore.insert({"dummy", data});
  for (auto listener : this->GetListeners()) {
    listener->ProcessAdd(data);
  }
}
void BondPricingService::Subscribe(BondPricesConnector *connector) {
  connector->read();
}
#endif //BONDTRADINGSYSTEM_BONDPRICINGSERVICE_H
