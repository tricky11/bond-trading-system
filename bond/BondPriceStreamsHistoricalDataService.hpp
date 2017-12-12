//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDSTREAMSHISTORICALDATASERVICE_H
#define BONDTRADINGSYSTEM_BONDSTREAMSHISTORICALDATASERVICE_H

#include "../base/historicaldataservice.hpp"
#include "../base/products.hpp"
#include "../connectors/OutputFileConnector.hpp"
#include "../base/streamingservice.hpp"

class BondPriceStreamsServiceListener : public ServiceListener<PriceStream<Bond>> {
 public:
  explicit BondPriceStreamsServiceListener(HistoricalDataService<PriceStream<Bond>> *listeningService);
 private:
  HistoricalDataService<PriceStream<Bond>> *listeningService;
  void ProcessAdd(PriceStream<Bond> &data) override;
};

class BondPriceStreamsConnector : public OutputFileConnector<PriceStream<Bond>> {
 public:
  explicit BondPriceStreamsConnector(const string &filePath);
 private:
  string toCSVString(PriceStream<Bond> &data) override;
  string getCSVHeader() override;
};

class BondPriceStreamsHistoricalDataService : public HistoricalDataService<PriceStream<Bond>> {
 public:
  BondPriceStreamsHistoricalDataService();
  void PersistData(string persistKey, const PriceStream<Bond> &data) override;
 private:
  BondPriceStreamsConnector *connector = new BondPriceStreamsConnector("streaming.csv");
};

#endif //BONDTRADINGSYSTEM_BONDSTREAMSHISTORICALDATASERVICE_H
