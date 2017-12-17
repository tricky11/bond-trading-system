//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDALGOSTREAMINGSERVICE_H
#define BONDTRADINGSYSTEM_BONDALGOSTREAMINGSERVICE_H

#include "../../base/pricingservice.hpp"
#include "../../base/products.hpp"
#include "../../base/soa.hpp"
#include "../../base/streamingservice.hpp"

template<typename T>
class AlgoStream {
 public:
  explicit AlgoStream(const PriceStream<T> &priceStream) : priceStream(priceStream) {}

  const PriceStream<T> &getPriceStream() const {
    return priceStream;
  }

 private:
  const PriceStream<T> &priceStream;
};

class BondAlgoStreamingService : public Service<string, AlgoStream<Bond>> {
 public:
  BondAlgoStreamingService() {}

  /**
   * Publish a new price stream alternating between volumes of 1000000 & 2000000.
   * @param newPrice
   */
  void PublishPrice(Price<Bond> &newPrice) {
    auto bond = BondProductService::GetInstance()->GetData(newPrice.GetProduct().GetProductId());

    PriceStreamOrder bidOrder
        (newPrice.GetMid() - newPrice.GetBidOfferSpread() / 2,
         states[currentState],
         2 * states[currentState],
         PricingSide::BID);
    PriceStreamOrder offerOrder
        (newPrice.GetMid() + newPrice.GetBidOfferSpread() / 2,
         states[currentState],
         2 * states[currentState],
         PricingSide::OFFER);
    PriceStream<Bond> priceStream(bond, bidOrder, offerOrder);
    AlgoStream<Bond> algoStream(priceStream);

    cycleState();
    dataStore.insert(make_pair(bond.GetProductId(), algoStream));
    if (dataStore.find(bond.GetProductId()) == dataStore.end()) {
      for (auto listener : this->GetListeners()) {
        listener->ProcessAdd(algoStream);
      }
    } else {
      for (auto listener : this->GetListeners()) {
        listener->ProcessUpdate(algoStream);
      }
    }
  }

  void OnMessage(AlgoStream<Bond> &data) override {

  }

 private:
  std::array<int, 2> states = {{1000000, 2000000}};
  unsigned int currentState = 0;
  void cycleState() {
    currentState = (currentState + 1) % states.size();
  }
};

class BondPricesServiceListener : public ServiceListener<Price<Bond>> {
 public:
  explicit BondPricesServiceListener(BondAlgoStreamingService *listeningService) : listeningService(listeningService) {}

  void ProcessAdd(Price<Bond> &data) override {
    listeningService->PublishPrice(data);
  }

  void ProcessRemove(Price<Bond> &data) override {

  }

  void ProcessUpdate(Price<Bond> &data) override {
    listeningService->PublishPrice(data);
  }

 private:
  BondAlgoStreamingService *listeningService;
};

#endif //BONDTRADINGSYSTEM_BONDALGOSTREAMINGSERVICE_H
