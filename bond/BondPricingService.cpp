//
// Created by Harish Reddy on 12/11/2017.
//

#include "BondPricingService.hpp"

Price<Bond> BondPricesConnector::parse(string line) {
  // TODO: Parse line to create Price object.
  return Price<Bond>(Bond(), 100.0, 0.1);
}
BondPricesConnector::BondPricesConnector(const string &filePath, Service<string, Price<Bond>> *connectedService)
    : InputFileConnector(filePath, connectedService) {}

void BondPricingService::OnMessage(Price<Bond> &data) {
  dataStore.insert({data.GetProduct().GetProductId(), data});
  for (auto listener : this->GetListeners()) {
    listener->ProcessAdd(data);
  }
}
void BondPricingService::Subscribe(BondPricesConnector *connector) {
  connector->read();
}
