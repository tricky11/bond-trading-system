cmake_minimum_required(VERSION 3.8)
project(BondTradingSystem)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set(SOURCE_FILES main.cpp base/executionservice.hpp base/historicaldataservice.hpp base/inquiryservice.hpp base/marketdataservice.hpp base/positionservice.hpp base/pricingservice.hpp base/products.hpp base/riskservice.hpp base/soa.hpp base/streamingservice.hpp base/tradebookingservice.hpp connectors/InputFileConnector.hpp connectors/OutputFileConnector.hpp bond/BondPriceStreamsHistoricalDataService.hpp bond/BondAlgoStreamingService.hpp bond/GUIService.hpp bond/BondPricingService.hpp bond/BondStreamingService.hpp utils/string.h)
add_executable(BondTradingSystem ${SOURCE_FILES})