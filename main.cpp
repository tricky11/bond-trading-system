#include "bond/stream/BondPricingService.hpp"
#include "bond/stream/GUIService.hpp"
#include "bond/stream/BondAlgoStreamingService.hpp"
#include "bond/stream/BondStreamingService.hpp"
#include "bond/stream/BondPriceStreamsHistoricalDataService.hpp"
#include "bond/inquiry/BondInquiryService.hpp"
#include "bond/trade/BondTradeBookingService.hpp"
#include "bond/trade/BondPositionService.hpp"
#include "bond/trade/BondPositionHistoricalDataService.hpp"
#include "bond/trade/BondRiskHistoricalDataService.hpp"
#include "bond/trade/BondRiskService.hpp"
#include "bond/execution/BondMarketDataService.hpp"
#include "bond/execution/BondAlgoExecutionService.hpp"
#include "bond/execution/BondExecutionService.hpp"
#include "bond/execution/BondExecutionHistoricalDataService.h"

void runStreamingProcess();
void runInquiryFlow();

void runTradesAndExecutionFlow();
int main() {
//  runStreamingProcess();
//  runInquiryFlow();
  runTradesAndExecutionFlow();

}
void runTradesAndExecutionFlow() {
  auto tradeBookingService = new BondTradeBookingService();
  auto positionService = new BondPositionService();
  auto riskService = new BondRiskService();
  auto positionHistoricalDataService = new BondPositionHistoricalDataService();
  auto riskHistoricalDataService = new BondRiskHistoricalDataService();

  auto tradeListener = new BondTradesServiceListener(positionService);
  auto positionListener = new BondPositionServiceListener(positionHistoricalDataService);
  auto positionListenerFromRisk = new BondPositionRiskServiceListener(riskService);
  auto riskListener = new BondRiskServiceListener(riskHistoricalDataService);

  tradeBookingService->AddListener(tradeListener);
  positionService->AddListener(positionListener);
  positionService->AddListener(positionListenerFromRisk);
  riskService->AddListener(riskListener);

  auto marketDataService = new BondMarketDataService();
  auto algoExecutionService = new BondAlgoExecutionService();
  auto executionService = new BondExecutionService();
  auto executionHistoricalDataService = new BondExecutionHistoricalDataService();

  auto marketDataListener = new BondMarketDataServiceListener(algoExecutionService);
  auto algoExecutionListener = new BondAlgoExecutionServiceListener(executionService);
  auto executionListener = new BondExecutionOrderServiceListener(executionHistoricalDataService);

  marketDataService->AddListener(marketDataListener);
  algoExecutionService->AddListener(algoExecutionListener);
  executionService->AddListener(executionListener);

  tradeBookingService->Subscribe(new BondTradesConnector("trades.csv", tradeBookingService));
  marketDataService->Subscribe(new BondMarketDataConnector("marketdata.csv", marketDataService));
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