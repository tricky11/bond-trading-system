//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_GUISERVICE_H
#define BONDTRADINGSYSTEM_GUISERVICE_H

#include "../base/products.hpp"
#include "../base/pricingservice.hpp"
#include "../connectors/OutputFileConnector.hpp"

class GUIConnector : public OutputFileConnector<Price<Bond>> {
 public:
  GUIConnector(const string &filePath);
  string toCSVString(Price<Bond> &data) override;
  string getCSVHeader() override;
};

class GUIService : public Service<string, Price<Bond>> {
 public:
  GUIService(const unsigned int throttle);
  void PersistData(Price<Bond> &data);
  void OnMessage(Price<Bond> &data) override;

 private:
  // defined in milliseconds
  const unsigned int throttle = 300;
  GUIConnector *connector;
};

class BondPriceServiceListener : public ServiceListener<Price<Bond>> {
 public:
  BondPriceServiceListener(GUIService *listeningService);
 private:
  void ProcessAdd(Price<Bond> &data) override;
  void ProcessRemove(Price<Bond> &data) override;
  void ProcessUpdate(Price<Bond> &data) override;
  GUIService *listeningService;
};

BondPriceServiceListener::BondPriceServiceListener(GUIService *listeningService) : listeningService(listeningService) {}

void BondPriceServiceListener::ProcessAdd(Price<Bond> &data) {
  listeningService->PersistData(data);
}
void BondPriceServiceListener::ProcessRemove(Price<Bond> &data) {

}
void BondPriceServiceListener::ProcessUpdate(Price<Bond> &data) {

}

GUIConnector::GUIConnector(const string &filePath) : OutputFileConnector(filePath) {}

string GUIConnector::toCSVString(Price<Bond> &data) {
  return "Dummy gui string";
}
string GUIConnector::getCSVHeader() {
  return "Dummy gui header";
}
void GUIService::PersistData(Price<Bond> &data) {
  connector->Publish(data);
}
void GUIService::OnMessage(Price<Bond> &data) {

}
GUIService::GUIService(const unsigned int throttle) : throttle(throttle) {
  connector = new GUIConnector("gui.csv");
  connector->WriteHeader();
}


#endif //BONDTRADINGSYSTEM_GUISERVICE_H
