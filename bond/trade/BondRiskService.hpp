//
// Created by Harish Reddy on 12/12/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDRISKSERVICE_H
#define BONDTRADINGSYSTEM_BONDRISKSERVICE_H

#include "../../base/soa.hpp"
#include "../../base/products.hpp"
#include "../../base/streamingservice.hpp"
#include "../../base/riskservice.hpp"

class BondRiskService : public RiskService<Bond> {
 public:
  void OnMessage(PV01<Bond> &data) override {
    // Do nothing. Since streaming service does not have a connector.
  }

  void AddPosition(Position<Bond> &position) override {
    auto product = position.GetProduct();
    PV01<Bond> risk(product, position.GetAggregatePosition(), position.GetAggregatePosition());
    if (dataStore.find(position.GetProduct().GetProductId()) == dataStore.end()) {
      dataStore.insert(make_pair(position.GetProduct().GetProductId(),risk));
      for (auto listener:this->GetListeners()) {
        listener->ProcessAdd(risk);
      }
    } else {
      dataStore.insert(make_pair(position.GetProduct().GetProductId(),risk));
      for (auto listener:this->GetListeners()) {
        listener->ProcessUpdate(risk);
      }
    }
  }

  const PV01<BucketedSector<Bond>> &GetBucketedRisk(const BucketedSector<Bond> &sector) const override {
    double totalPV01 = 0;
    long totalPosition = 0;
    for (const auto &product : sector.GetProducts()) {
      if (dataStore.find(product.GetProductId()) != dataStore.end()) {
        totalPV01 += dataStore.at(product.GetProductId()).GetPV01();
        totalPosition += dataStore.at(product.GetProductId()).GetQuantity();
      }
    }
    return PV01<BucketedSector<Bond>>(sector, totalPV01, totalPosition);
  }
};

class BondPositionRiskServiceListener : public ServiceListener<Position<Bond>> {
 public:
  explicit BondPositionRiskServiceListener(BondRiskService *listeningService) : listeningService(listeningService) {}

  void ProcessAdd(Position<Bond> &data) override {
    listeningService->AddPosition(data);
  }
  void ProcessRemove(Position<Bond> &data) override {

  }
  void ProcessUpdate(Position<Bond> &data) override {
    listeningService->AddPosition(data);
  }

 private:
  BondRiskService *listeningService;

};
#endif //BONDTRADINGSYSTEM_BONDRISKSERVICE_H
