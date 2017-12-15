//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDPOSITIONHISTORICALDATASERVICE_H
#define BONDTRADINGSYSTEM_BONDPOSITIONHISTORICALDATASERVICE_H

#include "../../base/historicaldataservice.hpp"
#include "../../base/products.hpp"
#include "../../connectors/OutputFileConnector.hpp"
#include "../../base/positionservice.hpp"

class BondPositionServiceListener : public ServiceListener<Position<Bond>> {
 public:
  explicit BondPositionServiceListener(HistoricalDataService<Position<Bond>> *listeningService);
  void ProcessRemove(Position<Bond> &data) override;
  void ProcessUpdate(Position<Bond> &data) override;
 private:
  HistoricalDataService<Position<Bond>> *listeningService;
  void ProcessAdd(Position<Bond> &data) override;
};

class BondPositionConnector : public OutputFileConnector<Position<Bond>> {
 public:
  explicit BondPositionConnector(const string &filePath);
 private:
  string toCSVString(Position<Bond> &data) override;
  string getCSVHeader() override;
};

class BondPositionHistoricalDataService : public HistoricalDataService<Position<Bond>> {
 public:
  BondPositionHistoricalDataService();
  void PersistData(string persistKey, const Position<Bond> &data) override;
 private:
  void OnMessage(Position<Bond> &data) override;
  BondPositionConnector *connector;
};

void BondPositionHistoricalDataService::PersistData(string persistKey, const Position<Bond> &data) {
  connector->Publish(const_cast<Position<Bond> &>(data));
}
BondPositionHistoricalDataService::BondPositionHistoricalDataService() {
  connector = new BondPositionConnector("output/positions.csv");
  connector->WriteHeader();
}

BondPositionConnector::BondPositionConnector(const string &filePath) : OutputFileConnector(filePath) {
}

string BondPositionConnector::toCSVString(Position<Bond> &data) {
  std::ostringstream oss;
  oss << data.GetProduct().GetProductId() << "," <<
      data.GetAggregatePosition();
  return oss.str();;
}
string BondPositionConnector::getCSVHeader() {
  return "CUSIP,Position";
}

BondPositionServiceListener::BondPositionServiceListener(HistoricalDataService<Position<Bond>> *listeningService)
    : listeningService(
    listeningService) {}

void BondPositionServiceListener::ProcessAdd(Position<Bond> &data) {
  listeningService->PersistData(data.GetProduct().GetProductId(), data);
}

void BondPositionServiceListener::ProcessRemove(Position<Bond> &data) {

}
void BondPositionServiceListener::ProcessUpdate(Position<Bond> &data) {
  listeningService->PersistData(data.GetProduct().GetProductId(), data);
}

void BondPositionHistoricalDataService::OnMessage(Position<Bond> &data) {

}
#endif //BONDTRADINGSYSTEM_BONDPOSITIONHISTORICALDATASERVICE_H
