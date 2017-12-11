//
// Created by Harish Reddy on 12/11/2017.
//

#include <sstream>
#include "../base/products.hpp"
#include "../base/pricingservice.hpp"
#include "../connectors/InputFileConnector.hpp"

class BondPricesConnector : InputFileConnector<Price<Bond>> {
 public:
  BondPricesConnector(const string &filePath, Service *connectedService) : InputFileConnector(filePath,
                                                                                              connectedService) {}

 private:
  Price<Bond> parse(string line) override {
    // TODO: Parse line to create Price object.
    return Price<Bond>(Bond(), 100.0, 0.1);
  }
};

class BondPricingService : public PricingService<Bond> {

 public:
  BondPricingService() {
    new BondPricesConnector("prices.txt", (Service *) this);
  }
};

