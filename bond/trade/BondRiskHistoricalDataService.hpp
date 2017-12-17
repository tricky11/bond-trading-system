//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDRISKHISTORICALDATASERVICE_H
#define BONDTRADINGSYSTEM_BONDRISKHISTORICALDATASERVICE_H

#include "../../base/historicaldataservice.hpp"
#include "../../base/products.hpp"
#include "../../connectors/OutputFileConnector.hpp"
#include "../../base/riskservice.hpp"

/**
 * Listens to updates from BondRiskService.
 */
class BondRiskServiceListener : public ServiceListener<PV01<Bond>> {
 public:
  explicit BondRiskServiceListener(HistoricalDataService<PV01<Bond>> *listeningService);
  void ProcessRemove(PV01<Bond> &data) override;
  void ProcessUpdate(PV01<Bond> &data) override;
 private:
  HistoricalDataService<PV01<Bond>> *listeningService;
  void ProcessAdd(PV01<Bond> &data) override;
};

/**
 * Writes output to risk.csv
 */
class BondRiskConnector : public OutputFileConnector<PV01<Bond>> {
 public:
  explicit BondRiskConnector(const string &filePath);
 private:
  string toCSVString(PV01<Bond> &data) override;
  string getCSVHeader() override;
};

class BondRiskHistoricalDataService : public HistoricalDataService<PV01<Bond>> {
 public:
  BondRiskHistoricalDataService();
  void PersistData(string persistKey, const PV01<Bond> &data) override;
 private:
  void OnMessage(PV01<Bond> &data) override;
  BondRiskConnector *connector;
};

void BondRiskHistoricalDataService::PersistData(string persistKey, const PV01<Bond> &data) {
  connector->Publish(const_cast<PV01<Bond> &>(data));
}
BondRiskHistoricalDataService::BondRiskHistoricalDataService() {
  connector = new BondRiskConnector("output/risk.csv");
  connector->WriteHeader();
}

BondRiskConnector::BondRiskConnector(const string &filePath) : OutputFileConnector(filePath) {
}

string BondRiskConnector::toCSVString(PV01<Bond> &data) {
  std::ostringstream oss;
  oss << data.GetProduct().GetProductId() << "," <<
      data.GetQuantity() << "," <<
      data.GetPV01();
  return oss.str();;
}
string BondRiskConnector::getCSVHeader() {
  return "CUSIP,Quantity,PV01";
}

BondRiskServiceListener::BondRiskServiceListener(HistoricalDataService<PV01<Bond>> *listeningService)
    : listeningService(
    listeningService) {}

void BondRiskServiceListener::ProcessAdd(PV01<Bond> &data) {
  listeningService->PersistData(data.GetProduct().GetProductId(), data);
}

void BondRiskServiceListener::ProcessRemove(PV01<Bond> &data) {

}
void BondRiskServiceListener::ProcessUpdate(PV01<Bond> &data) {
  listeningService->PersistData(data.GetProduct().GetProductId(), data);
}

void BondRiskHistoricalDataService::OnMessage(PV01<Bond> &data) {

}
#endif //BONDTRADINGSYSTEM_BONDRISKHISTORICALDATASERVICE_H
