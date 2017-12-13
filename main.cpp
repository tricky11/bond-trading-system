#include "bond/stream/BondPricingService.hpp"
#include "bond/stream/GUIService.hpp"
#include "bond/stream/BondAlgoStreamingService.hpp"
#include "bond/stream/BondStreamingService.hpp"
#include "bond/stream/BondPriceStreamsHistoricalDataService.hpp"
#include "bond/inquiry/BondInquiryService.hpp"

void runStreamingProcess();

void runInquiryFlow();
int main() {
//  runStreamingProcess();

  runInquiryFlow();
}
void runInquiryFlow() {
  auto inquiryService = new BondInquiryService();
  auto inquiryServiceListener = new BondInquiryServiceListener(inquiryService);
  inquiryService->AddListener(inquiryServiceListener);
  inquiryService->Subscribe(new BondInquirySubscriber("inquiry.csv", inquiryService));
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
      new BondPricesConnector("prices.csv", pricingService));
}