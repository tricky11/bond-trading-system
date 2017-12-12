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

#endif //BONDTRADINGSYSTEM_GUISERVICE_H
