//
// Created by Harish Reddy on 12/11/2017.
//

#include "BondPriceStreamsHistoricalDataService.hpp"

void BondPriceStreamsHistoricalDataService::PersistData(string persistKey, const PriceStream<Bond> &data) {
  connector->Publish(const_cast<PriceStream<Bond> &>(data));
}
BondPriceStreamsHistoricalDataService::BondPriceStreamsHistoricalDataService() = default;

BondPriceStreamsConnector::BondPriceStreamsConnector(const string &filePath) : OutputFileConnector(filePath) {
}

string BondPriceStreamsConnector::toCSVString(PriceStream<Bond> &data) {
  return "dummy string";
}
string BondPriceStreamsConnector::getCSVHeader() {
  return "dummy header";
}

BondPriceStreamsServiceListener::BondPriceStreamsServiceListener(HistoricalDataService<PriceStream<Bond>> *listeningService)
    : listeningService(
    listeningService) {}

void BondPriceStreamsServiceListener::ProcessAdd(PriceStream<Bond> &data) {
  listeningService->PersistData("dummy key", data);
}
