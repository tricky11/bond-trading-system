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
    for (auto listener:this->GetListeners()) {
      auto product = Bond("id", BondIdType::CUSIP, "ticker", 2.0, boost::gregorian::date());
      PV01<Bond> risk(product, 0.0, 0l);
      listener->ProcessAdd(risk);
    }
  }

  const PV01<BucketedSector<Bond>> &GetBucketedRisk(const BucketedSector<Bond> &sector) const override {
    return PV01<BucketedSector<Bond>>(sector, 0.0, 0L);
  }
};

class BondPositionRiskServiceListener : public ServiceListener<Position<Bond>> {
 public:
  explicit BondPositionRiskServiceListener(BondRiskService *listeningService) : listeningService(listeningService) {}

  void ProcessAdd(Position<Bond> &data) override {
    std::cout << "ProcessAdd in BondPositionRiskServiceListener" << std::endl;
    listeningService->AddPosition(data);
  }
  void ProcessRemove(Position<Bond> &data) override {

  }
  void ProcessUpdate(Position<Bond> &data) override {

  }

 private:
  BondRiskService *listeningService;

};
#endif //BONDTRADINGSYSTEM_BONDRISKSERVICE_H
