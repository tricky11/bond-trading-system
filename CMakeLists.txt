cmake_minimum_required(VERSION 3.8)
project(BondTradingSystem)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp base/executionservice.hpp base/historicaldataservice.hpp base/inquiryservice.hpp base/marketdataservice.hpp base/positionservice.hpp base/pricingservice.hpp base/products.hpp base/riskservice.hpp base/soa.hpp base/streamingservice.hpp base/tradebookingservice.hpp)
add_executable(BondTradingSystem ${SOURCE_FILES})