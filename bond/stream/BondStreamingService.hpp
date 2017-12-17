//
// Created by Harish Reddy on 12/12/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDSTREAMINGSERVICE_H
#define BONDTRADINGSYSTEM_BONDSTREAMINGSERVICE_H

#include "BondAlgoStreamingService.hpp"
#include "../../base/soa.hpp"
#include "../../base/products.hpp"
#include "../../base/streamingservice.hpp"

class BondStreamingService : public StreamingService<Bond> {
 public:
  BondStreamingService() {}
  void OnMessage(PriceStream<Bond> &data) override {
    // Do nothing. Since this service does not have a connector.
  }

  void PublishPrice(const PriceStream<Bond> &priceStream) override {
    dataStore.insert(make_pair(priceStream.GetProduct().GetProductId(), priceStream));
    if (dataStore.find(priceStream.GetProduct().GetProductId()) == dataStore.end()) {
      for (auto listener : this->GetListeners()) {
        listener->ProcessAdd(const_cast<PriceStream<Bond> &>(priceStream));
      }
    } else {
      for (auto listener : this->GetListeners()) {
        listener->ProcessUpdate(const_cast<PriceStream<Bond> &>(priceStream));
      }
    }
  }
};

class BondAlgoStreamServiceListener : public ServiceListener<AlgoStream<Bond>> {
 public:
  explicit BondAlgoStreamServiceListener(BondStreamingService *listeningService) : listeningService(listeningService) {}

  void ProcessAdd(AlgoStream<Bond> &data) override {
    listeningService->PublishPrice(data.getPriceStream());
  }
  void ProcessRemove(AlgoStream<Bond> &data) override {

  }
  void ProcessUpdate(AlgoStream<Bond> &data) override {
    listeningService->PublishPrice(data.getPriceStream());
  }

 private:
  BondStreamingService *listeningService;

};
#endif //BONDTRADINGSYSTEM_BONDSTREAMINGSERVICE_H
