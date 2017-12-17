//
// Created by Harish Reddy on 12/13/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDEXECUTIONSERVICE_H
#define BONDTRADINGSYSTEM_BONDEXECUTIONSERVICE_H

#include "BondAlgoExecutionService.hpp"
#include "../../base/soa.hpp"
#include "../../base/products.hpp"
#include "../../base/executionservice.hpp"

class BondExecutionService : public ExecutionService<Bond> {
 public:
  BondExecutionService() {}
  void OnMessage(ExecutionOrder<Bond> &data) override {
    // Do nothing. Since streaming service does not have a connector.
  }
  void ExecuteOrder(const ExecutionOrder<Bond> &order, Market market) override {
    for (auto listener:this->GetListeners()) {
      listener->ProcessAdd(const_cast<ExecutionOrder<Bond> &>(order));
    }
  }
};

class BondAlgoExecutionServiceListener : public ServiceListener<AlgoExecution<Bond>> {
 public:
  explicit BondAlgoExecutionServiceListener(BondExecutionService *listeningService)
      : listeningService(listeningService) {}

  void ProcessAdd(AlgoExecution<Bond> &data) override {
    listeningService->ExecuteOrder(data.getExecutionOrder(), Market::CME);
  }
  void ProcessRemove(AlgoExecution<Bond> &data) override {

  }
  void ProcessUpdate(AlgoExecution<Bond> &data) override {

  }

 private:
  BondExecutionService *listeningService;

};

#endif //BONDTRADINGSYSTEM_BONDEXECUTIONSERVICE_H
