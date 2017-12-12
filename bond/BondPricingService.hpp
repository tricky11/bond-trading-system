//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDPRICINGSERVICE_H
#define BONDTRADINGSYSTEM_BONDPRICINGSERVICE_H

#include <sstream>
#include "../base/products.hpp"
#include "../base/pricingservice.hpp"
#include "../connectors/InputFileConnector.hpp"

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

#endif //BONDTRADINGSYSTEM_BONDPRICINGSERVICE_H
