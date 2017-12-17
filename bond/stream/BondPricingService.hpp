//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDPRICINGSERVICE_H
#define BONDTRADINGSYSTEM_BONDPRICINGSERVICE_H

#include <sstream>
#include "../../base/products.hpp"
#include "../../base/pricingservice.hpp"
#include "../../connectors/InputFileConnector.hpp"
#include "../../utils/string.h"
#include "../BondProductService.hpp"

class BondPricesConnector : public InputFileConnector<string, Price<Bond>> {
 public:
  BondPricesConnector(const string &filePath, Service<string, Price<Bond>> *connectedService);
 private:
  void parse(string line) override;
};

class BondPricingService : public PricingService<Bond> {
 public:
  BondPricingService() {}
  void Subscribe(BondPricesConnector *connector);
  void OnMessage(Price<Bond> &data) override;
};

void BondPricesConnector::parse(string line) {
  auto split = splitString(line, ',');
  string id = split[0];
  double mid = convertFractionalPriceToDouble(split[1]), bidOfferSpread = convertFractionalPriceToDouble(split[2]);

  auto bond = BondProductService::GetInstance()->GetData(id);
  auto price = Price<Bond>(bond, mid, bidOfferSpread);
  connectedService->OnMessage(price);
}

BondPricesConnector::BondPricesConnector(const string &filePath, Service<string, Price<Bond>> *connectedService)
    : InputFileConnector(filePath, connectedService) {}

void BondPricingService::OnMessage(Price<Bond> &data) {
  if (dataStore.find(data.GetProduct().GetProductId()) == dataStore.end()) {
    dataStore.insert(make_pair(data.GetProduct().GetProductId(), data));
    for (auto listener : this->GetListeners()) {
      listener->ProcessAdd(data);
    }
  } else {
    dataStore.insert(make_pair(data.GetProduct().GetProductId(), data));
    dataStore.insert({data.GetProduct().GetProductId(), data});
    for (auto listener : this->GetListeners()) {
      listener->ProcessUpdate(data);
    }
  }
}

void BondPricingService::Subscribe(BondPricesConnector *connector) {
  connector->read();
}
#endif //BONDTRADINGSYSTEM_BONDPRICINGSERVICE_H
