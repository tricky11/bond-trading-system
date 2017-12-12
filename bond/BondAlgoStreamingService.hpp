//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDALGOSTREAMINGSERVICE_H
#define BONDTRADINGSYSTEM_BONDALGOSTREAMINGSERVICE_H

#include "../base/pricingservice.hpp"
#include "../base/products.hpp"
#include "../base/soa.hpp"
#include "../base/streamingservice.hpp"

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
  // Publish two-way prices
  void PublishPrice(const AlgoStream<Bond> &priceStream);
};

class BondPricesServiceListener : public ServiceListener<Price<Bond>> {
 public:
  explicit BondPricesServiceListener(BondAlgoStreamingService *listeningService);
 private:
  void ProcessAdd(Price<Bond> &data) override;
  BondAlgoStreamingService *listeningService;
};

#endif //BONDTRADINGSYSTEM_BONDALGOSTREAMINGSERVICE_H
