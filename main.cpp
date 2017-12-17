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
#include "bond/BondProductService.hpp"

void setupProducts();
void runStreamingFlow();
void runInquiryFlow();
void runTradesAndExecutionFlow();

int main() {
  setupProducts();
  runStreamingFlow();
//  runInquiryFlow();
//  runTradesAndExecutionFlow();
}

void setupProducts() {
  auto productService = BondProductService::GetInstance();
  Bond T2("9128283H1", CUSIP, "T", 1.750, date(2019, Nov, 30));
  Bond T3("9128283L2", CUSIP, "T", 1.875, date(2020, Dec, 15));
  Bond T5("912828M80", CUSIP, "T", 2.0, date(2022, Nov, 30));
  Bond T7("9128283J7", CUSIP, "T", 2.125, date(2024, Nov, 30));
  Bond T10("9128283F5", CUSIP, "T", 2.25, date(2027, Dec, 15));
  Bond T30("912810RZ3", CUSIP, "T", 2.75, date(2047, Dec, 15));

  productService->Add(T2);
  productService->Add(T3);
  productService->Add(T5);
  productService->Add(T7);
  productService->Add(T10);
  productService->Add(T30);
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
  auto executionListenerFromTrade = new BondExecutionServiceListener(tradeBookingService);

  marketDataService->AddListener(marketDataListener);
  algoExecutionService->AddListener(algoExecutionListener);
  executionService->AddListener(executionListener);
  executionService->AddListener(executionListenerFromTrade);

  tradeBookingService->Subscribe(new BondTradesConnector("input/trades.csv", tradeBookingService));
  marketDataService->Subscribe(new BondMarketDataConnector("input/marketdata.csv", marketDataService));
}

void runInquiryFlow() {
  auto inquiryService = new BondInquiryService();
  auto inquiryServiceListener = new BondInquiryServiceListener(inquiryService);
  inquiryService->AddListener(inquiryServiceListener);
  inquiryService->Subscribe(new BondInquirySubscriber("input/inquiries.csv", inquiryService));
}

void runStreamingFlow() {
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
      new BondPricesConnector("input/prices.csv", pricingService));
}