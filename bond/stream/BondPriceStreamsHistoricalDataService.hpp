//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDSTREAMSHISTORICALDATASERVICE_H
#define BONDTRADINGSYSTEM_BONDSTREAMSHISTORICALDATASERVICE_H

#include "../../base/historicaldataservice.hpp"
#include "../../base/products.hpp"
#include "../../connectors/OutputFileConnector.hpp"
#include "../../base/streamingservice.hpp"

class BondPriceStreamsServiceListener : public ServiceListener<PriceStream<Bond>> {
 public:
  explicit BondPriceStreamsServiceListener(HistoricalDataService<PriceStream<Bond>> *listeningService);
  void ProcessRemove(PriceStream<Bond> &data) override;
  void ProcessUpdate(PriceStream<Bond> &data) override;
 private:
  HistoricalDataService<PriceStream<Bond>> *listeningService;
  void ProcessAdd(PriceStream<Bond> &data) override;
};

class BondPriceStreamsConnector : public OutputFileConnector<PriceStream<Bond>> {
 public:
  explicit BondPriceStreamsConnector(const string &filePath);
 private:
  string toCSVString(PriceStream<Bond> &data) override;
  string getCSVHeader() override;
};

class BondPriceStreamsHistoricalDataService : public HistoricalDataService<PriceStream<Bond>> {
 public:
  BondPriceStreamsHistoricalDataService();
  void PersistData(string persistKey, const PriceStream<Bond> &data) override;
 private:
  void OnMessage(PriceStream<Bond> &data) override;
  BondPriceStreamsConnector *connector;
};

void BondPriceStreamsHistoricalDataService::PersistData(string persistKey, const PriceStream<Bond> &data) {
  connector->Publish(const_cast<PriceStream<Bond> &>(data));
}
BondPriceStreamsHistoricalDataService::BondPriceStreamsHistoricalDataService() {
  connector = new BondPriceStreamsConnector("output/streaming.csv");
  connector->WriteHeader();
}

BondPriceStreamsConnector::BondPriceStreamsConnector(const string &filePath) : OutputFileConnector(filePath) {
}

string BondPriceStreamsConnector::toCSVString(PriceStream<Bond> &data) {
  std::ostringstream oss;
  oss << data.GetProduct().GetProductId() << "," <<
      data.GetBidOrder().GetPrice() << "," <<
      data.GetBidOrder().GetVisibleQuantity() << "," <<
      data.GetBidOrder().GetHiddenQuantity() << "," <<
      data.GetOfferOrder().GetPrice() << "," <<
      data.GetOfferOrder().GetVisibleQuantity() << "," <<
      data.GetOfferOrder().GetHiddenQuantity();
  return oss.str();
}
string BondPriceStreamsConnector::getCSVHeader() {
  return "CUSIP,BidPrice,BidVisibleQuantity,BidHiddenQuantity,OfferPrice,OfferVisibleQuantity,OfferHiddenQuantity";
}

BondPriceStreamsServiceListener::BondPriceStreamsServiceListener(HistoricalDataService<PriceStream<Bond>> *listeningService)
    : listeningService(
    listeningService) {}

void BondPriceStreamsServiceListener::ProcessAdd(PriceStream<Bond> &data) {
  listeningService->PersistData(data.GetProduct().GetProductId(), data);
}

void BondPriceStreamsServiceListener::ProcessRemove(PriceStream<Bond> &data) {

}

void BondPriceStreamsServiceListener::ProcessUpdate(PriceStream<Bond> &data) {
  listeningService->PersistData(data.GetProduct().GetProductId(), data);
}

void BondPriceStreamsHistoricalDataService::OnMessage(PriceStream<Bond> &data) {

}
#endif //BONDTRADINGSYSTEM_BONDSTREAMSHISTORICALDATASERVICE_H
