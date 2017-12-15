cmake_minimum_required(VERSION 3.8)
project(BondTradingSystem)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set(SOURCE_FILES main.cpp base/executionservice.hpp base/historicaldataservice.hpp base/inquiryservice.hpp base/marketdataservice.hpp base/positionservice.hpp base/pricingservice.hpp base/products.hpp base/riskservice.hpp base/soa.hpp base/streamingservice.hpp base/tradebookingservice.hpp connectors/InputFileConnector.hpp connectors/OutputFileConnector.hpp bond/stream/BondPriceStreamsHistoricalDataService.hpp bond/stream/BondAlgoStreamingService.hpp bond/stream/GUIService.hpp bond/stream/BondPricingService.hpp bond/stream/BondStreamingService.hpp utils/string.h bond/inquiry/BondInquiryService.hpp bond/trade/BondTradeBookingService.hpp bond/trade/BondPositionHistoricalDataService.hpp bond/trade/BondPositionService.hpp bond/trade/BondRiskHistoricalDataService.hpp bond/trade/BondRiskService.hpp bond/execution/BondMarketDataService.hpp bond/execution/BondAlgoExecutionService.hpp bond/execution/BondExecutionService.hpp bond/execution/BondExecutionHistoricalDataService.h bond/BondProductService.hpp)
add_executable(BondTradingSystem ${SOURCE_FILES})