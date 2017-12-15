//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_GUISERVICE_H
#define BONDTRADINGSYSTEM_GUISERVICE_H

#include "../../base/products.hpp"
#include "../../base/pricingservice.hpp"
#include "../../connectors/OutputFileConnector.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>

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
  boost::posix_time::ptime lastTick = boost::posix_time::microsec_clock::universal_time();
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
  listeningService->PersistData(data);
}

GUIConnector::GUIConnector(const string &filePath) : OutputFileConnector(filePath) {}

string GUIConnector::toCSVString(Price<Bond> &data) {
  std::ostringstream oss;
  oss << data.GetProduct().GetProductId() << "," <<
      (data.GetMid() - data.GetBidOfferSpread() / 2) << "," <<
      (data.GetMid() + data.GetBidOfferSpread() / 2);
  return oss.str();
}
string GUIConnector::getCSVHeader() {
  return "CUSIP,BidPrice,OfferPrice";
}

void GUIService::PersistData(Price<Bond> &data) {
  auto currentTick = boost::posix_time::microsec_clock::universal_time();
  boost::posix_time::time_duration diff = currentTick - lastTick;
  if (diff.total_milliseconds() > 300) {
    connector->Publish(data);
    lastTick = currentTick;
  }
}

void GUIService::OnMessage(Price<Bond> &data) {

}

GUIService::GUIService(const unsigned int throttle) : throttle(throttle) {
  connector = new GUIConnector("gui.csv");
  connector->WriteHeader();
}

#endif //BONDTRADINGSYSTEM_GUISERVICE_H
