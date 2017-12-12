#include "bond/BondPricingService.hpp"
#include "bond/GUIService.hpp"

int main() {
  auto pricingService = new BondPricingService();
  auto guiService = new GUIService(300);
  auto guiServicePriceListener = new BondPriceServiceListener(guiService);
  pricingService->AddListener(guiServicePriceListener);
  pricingService->Subscribe(
      new BondPricesConnector("prices.csv", (Service<string, Price<Bond>> *) pricingService));
}