//
// Created by Harish Reddy on 12/11/2017.
//

#include "BondAlgoStreamingService.hpp"
void BondAlgoStreamingService::PublishPrice(const AlgoStream<Bond> &priceStream) {

}

BondPricesServiceListener::BondPricesServiceListener(BondAlgoStreamingService *listeningService) : listeningService(
    listeningService) {}

void BondPricesServiceListener::ProcessAdd(Price<Bond> &data) {
}
