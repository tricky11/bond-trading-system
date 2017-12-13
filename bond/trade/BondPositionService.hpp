//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDPOSITIONSERVICE_H
#define BONDTRADINGSYSTEM_BONDPOSITIONSERVICE_H

#include "../../base/positionservice.hpp"
#include "../../base/products.hpp"
#include "../../base/soa.hpp"

class BondPositionService : public PositionService<Bond> {
 public:
  void AddTrade(const Trade<Bond> &trade) override {
    for (auto listener : this->GetListeners()) {
      listener->ProcessAdd((Position<Bond> &) trade);
    }
  }

  void OnMessage(Position<Bond> &data) override {

  }
};

class BondTradesServiceListener : public ServiceListener<Trade<Bond>> {
 public:
  explicit BondTradesServiceListener(BondPositionService *listeningService) : listeningService(listeningService) {}

  void ProcessAdd(Trade<Bond> &data) override {
    std::cout << "ProcessAdd in BondTradesServiceListener" << std::endl;
    listeningService->AddTrade(data);
  }

  void ProcessRemove(Trade<Bond> &data) override {

  }
  void ProcessUpdate(Trade<Bond> &data) override {

  }

 private:
  BondPositionService *listeningService;
};

#endif //BONDTRADINGSYSTEM_BONDPOSITIONSERVICE_H
