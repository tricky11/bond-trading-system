//
// Created by Harish Reddy on 12/13/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDEXECUTIONHISTORICALDATASERVICE_H
#define BONDTRADINGSYSTEM_BONDEXECUTIONHISTORICALDATASERVICE_H


#include "../../base/historicaldataservice.hpp"
#include "../../base/products.hpp"
#include "../../connectors/OutputFileConnector.hpp"
#include "../../base/streamingservice.hpp"
#include "../../base/executionservice.hpp"

class BondExecutionOrderServiceListener : public ServiceListener<ExecutionOrder<Bond>> {
 public:
  explicit BondExecutionOrderServiceListener(HistoricalDataService<ExecutionOrder<Bond>> *listeningService);
  void ProcessRemove(ExecutionOrder<Bond> &data) override;
  void ProcessUpdate(ExecutionOrder<Bond> &data) override;
 private:
  HistoricalDataService<ExecutionOrder<Bond>> *listeningService;
  void ProcessAdd(ExecutionOrder<Bond> &data) override;
};

class BondExecutionOrderConnector : public OutputFileConnector<ExecutionOrder<Bond>> {
 public:
  explicit BondExecutionOrderConnector(const string &filePath);
 private:
  string toCSVString(ExecutionOrder<Bond> &data) override;
  string getCSVHeader() override;
};

class BondExecutionHistoricalDataService : public HistoricalDataService<ExecutionOrder<Bond>> {
 public:
  BondExecutionHistoricalDataService();
  void PersistData(string persistKey, const ExecutionOrder<Bond> &data) override;
 private:
  void OnMessage(ExecutionOrder<Bond> &data) override;
  BondExecutionOrderConnector *connector;
};

void BondExecutionHistoricalDataService::PersistData(string persistKey, const ExecutionOrder<Bond> &data) {
  connector->Publish(const_cast<ExecutionOrder<Bond> &>(data));
}
BondExecutionHistoricalDataService::BondExecutionHistoricalDataService() {
  connector = new BondExecutionOrderConnector("execution.csv");
  connector->WriteHeader();
}

BondExecutionOrderConnector::BondExecutionOrderConnector(const string &filePath) : OutputFileConnector(filePath) {
}

string BondExecutionOrderConnector::toCSVString(ExecutionOrder<Bond> &data) {
  return "dummy string";
}
string BondExecutionOrderConnector::getCSVHeader() {
  return "execution";
}

BondExecutionOrderServiceListener::BondExecutionOrderServiceListener(HistoricalDataService<ExecutionOrder<Bond>> *listeningService)
    : listeningService(
    listeningService) {}

void BondExecutionOrderServiceListener::ProcessAdd(ExecutionOrder<Bond> &data) {
  std::cout << "ProcessAdd in BondExecutionOrderServiceListener" << std::endl;

  listeningService->PersistData("dummy key", data);
}

void BondExecutionOrderServiceListener::ProcessRemove(ExecutionOrder<Bond> &data) {

}
void BondExecutionOrderServiceListener::ProcessUpdate(ExecutionOrder<Bond> &data) {

}

void BondExecutionHistoricalDataService::OnMessage(ExecutionOrder<Bond> &data) {

}

#endif //BONDTRADINGSYSTEM_BONDEXECUTIONHISTORICALDATASERVICE_H
