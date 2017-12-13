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
  AlgoStream(const PriceStream<T> &priceStream) : priceStream(priceStream) {}

  const PriceStream<T> &getPriceStream() const {
    return priceStream;
  }

 private:
  const PriceStream<T> &priceStream;
};

class BondAlgoStreamingService : public Service<string, AlgoStream<Bond>> {
 public:
  void PublishPriceStream(AlgoStream<Bond> &algoStream) {
    for (auto listener : this->GetListeners()) {
      listener->ProcessAdd(algoStream);
    }
  }

  void OnMessage(AlgoStream<Bond> &data) override {

  }
};

class BondPricesServiceListener : public ServiceListener<Price<Bond>> {
 public:
  explicit BondPricesServiceListener(BondAlgoStreamingService *listeningService) : listeningService(listeningService) {}

  void ProcessAdd(Price<Bond> &data) override {
    std::cout<<"ProcessAdd in BondPricesServiceListener"<<std::endl;
    auto product = Bond("id", BondIdType::CUSIP, "ticker", 2.0, boost::gregorian::date());

    PriceStreamOrder bidOrder(data.GetMid() - data.GetBidOfferSpread() / 2, 1000000, 2000000, PricingSide::BID);
    PriceStreamOrder askOrder(data.GetMid() + data.GetBidOfferSpread() / 2, 1000000, 2000000, PricingSide::OFFER);
    PriceStream<Bond> priceStream(product, bidOrder, askOrder);
    AlgoStream<Bond> algoStream(priceStream);
    listeningService->PublishPriceStream(algoStream);
  }
  void ProcessRemove(Price<Bond> &data) override {

  }
  void ProcessUpdate(Price<Bond> &data) override {

  }

 private:
  BondAlgoStreamingService *listeningService;
};

#endif //BONDTRADINGSYSTEM_BONDALGOSTREAMINGSERVICE_H
