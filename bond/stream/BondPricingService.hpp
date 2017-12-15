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
  Price<Bond> parse(string line) override;
};

class BondPricingService : public PricingService<Bond> {
 public:
  void Subscribe(BondPricesConnector *connector);
  void OnMessage(Price<Bond> &data) override;
};

Price<Bond> BondPricesConnector::parse(string line) {
  auto split = splitString(line, ',');
  string id = split[0];
  double mid = stod(split[1]), bidOfferSpread = stod(split[2]);

  auto bond = BondProductService::GetInstance()->GetData(id);
  return Price<Bond>(bond, mid, bidOfferSpread);
}

BondPricesConnector::BondPricesConnector(const string &filePath, Service<string, Price<Bond>> *connectedService)
    : InputFileConnector(filePath, connectedService) {}

void BondPricingService::OnMessage(Price<Bond> &data) {
  dataStore[data.GetProduct().GetProductId()] = data;
  if (dataStore.find(data.GetProduct().GetProductId()) == unordered_map::end()) {
    for (auto listener : this->GetListeners()) {
      listener->ProcessAdd(data);
    }
  } else {
    for (auto listener : this->GetListeners()) {
      listener->ProcessUpdate(data);
    }
  }
}

void BondPricingService::Subscribe(BondPricesConnector *connector) {
  connector->read();
}
#endif //BONDTRADINGSYSTEM_BONDPRICINGSERVICE_H
