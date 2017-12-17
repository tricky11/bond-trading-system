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
  BondPositionService() {}

  /**
   * Add a new position or update a given position if it already exists.
   * @param trade a new trade that has been recently executed
   */
  void AddTrade(const Trade<Bond> &trade) override {
    if (dataStore.find(trade.GetProduct().GetProductId()) == dataStore.end()) {
      Position<Bond> newPosition = Position<Bond>(trade.GetProduct());
      newPosition.UpdatePosition(trade); // TODO: Move this to ctor?
      dataStore.insert(make_pair(trade.GetProduct().GetProductId(), newPosition));
      for (auto listener : this->GetListeners()) {
        listener->ProcessAdd(newPosition);
      }
    } else {
      auto position = dataStore.at(trade.GetProduct().GetProductId());
      position.UpdatePosition(trade);
      for (auto listener : this->GetListeners()) {
        listener->ProcessUpdate(position);
      }
    }
  }

  void OnMessage(Position<Bond> &data) override {

  }
};

class BondTradesServiceListener : public ServiceListener<Trade<Bond>> {
 public:
  explicit BondTradesServiceListener(BondPositionService *listeningService) : listeningService(listeningService) {}

  void ProcessAdd(Trade<Bond> &data) override {
    listeningService->AddTrade(data);
  }

  void ProcessRemove(Trade<Bond> &data) override {
    // NO-OP : Trades are never removed in this project.
  }
  void ProcessUpdate(Trade<Bond> &data) override {
    // NO-OP : Trades are never updated in this project.
  }

 private:
  BondPositionService *listeningService;
};

#endif //BONDTRADINGSYSTEM_BONDPOSITIONSERVICE_H
