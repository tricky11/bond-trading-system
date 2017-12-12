#include "bond/BondPricingService.hpp"
#include "bond/GUIService.hpp"
#include "bond/BondAlgoStreamingService.hpp"
#include "bond/BondStreamingService.hpp"
#include "bond/BondPriceStreamsHistoricalDataService.hpp"

void runStreamingProcess();

int main() {
  runStreamingProcess();
}

void runStreamingProcess() {
  auto pricingService = new BondPricingService();
  auto guiService = new GUIService(300);
  auto algoStreamingService = new BondAlgoStreamingService();
  auto streamingService = new BondStreamingService();
  auto historicalDataService = new BondPriceStreamsHistoricalDataService();

  auto guiServiceListener = new BondPriceServiceListener(guiService);
  auto algoStreamingServiceListener = new BondPricesServiceListener(algoStreamingService);
  auto streamingServiceListener = new BondAlgoStreamServiceListener(streamingService);
  auto historicalDataServiceListener = new BondPriceStreamsServiceListener(historicalDataService);

  pricingService->AddListener(guiServiceListener);
  pricingService->AddListener(algoStreamingServiceListener);
  algoStreamingService->AddListener(streamingServiceListener);
  streamingService->AddListener(historicalDataServiceListener);

  pricingService->Subscribe(
      new BondPricesConnector("prices.csv", (Service<string, Price<Bond>> *) pricingService));
}