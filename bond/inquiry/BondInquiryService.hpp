//
// Created by Harish Reddy on 12/12/2017.
//

#ifndef BONDTRADINGSYSTEM_BONDINQUIRYSERVICE_H
#define BONDTRADINGSYSTEM_BONDINQUIRYSERVICE_H

#include "../../base/products.hpp"
#include "../../base/inquiryservice.hpp"
#include "../../connectors/InputFileConnector.hpp"
#include "../../connectors/OutputFileConnector.hpp"

class BondInquirySubscriber : public InputFileConnector<string, Inquiry<Bond>> {
 public:
  BondInquirySubscriber(const string &filePath, Service<string, Inquiry<Bond>> *connectedService) : InputFileConnector(
      filePath,
      connectedService) {}

 private:
  Inquiry<Bond> parse(string line) override {
    auto split = splitString(line, ',');
    string inquiryId = split[0];
    string productId = split[1];
    Side side = (split[2].compare("0") == 0) ? BUY : SELL;
    long quantity = stol(split[3]);
    double price = stol(split[4]);
    auto bond = BondProductService::GetInstance()->GetData(productId);
    return Inquiry<Bond>(inquiryId, bond, side, quantity, price, InquiryState::RECEIVED);
  }
};

class BondInquiryPublisher : public OutputFileConnector<Inquiry<Bond>> {
 public:
  explicit BondInquiryPublisher(const string &filePath) : OutputFileConnector(filePath) {}

  string toCSVString(Inquiry<Bond> &data) override {
    std::ostringstream oss;
    oss <<
        data.GetInquiryId() << "," <<
        data.GetProduct().GetProductId() << "," <<
        data.GetSide() << "," <<
        data.GetQuantity() << "," <<
        data.GetPrice() << "," <<
        data.GetState();
    return oss.str();
  }

  string getCSVHeader() override {
    return "InquiryId,ProductId,Side,Quantity,Price,State";
  }
};

class BondInquiryService : public InquiryService<Bond> {
 public:
  BondInquiryService() {
    publishConnector = new BondInquiryPublisher("output/allinquires.csv");
    publishConnector->WriteHeader();
  }

  void OnMessage(Inquiry<Bond> &data) override {
    dataStore[data.GetProduct().GetProductId()] = data;
    if (data.GetState() == InquiryState::RECEIVED) {
      for (auto listener : this->GetListeners()) {
        listener->ProcessAdd(data);
      }
    } else if (data.GetState() == InquiryState::QUOTED) {
      data.SetState(InquiryState::DONE);
      publishConnector->Publish(data);
      for (auto listener : this->GetListeners()) {
        listener->ProcessUpdate(data);
      }
    }
  }

  void Subscribe(BondInquirySubscriber *subscribeConnector) {
    subscribeConnector->read();
  }

 private:
  BondInquiryPublisher *publishConnector;
};

class BondInquiryServiceListener : public ServiceListener<Inquiry<Bond>> {
 public:
  explicit BondInquiryServiceListener(BondInquiryService *listeningService) : listeningService(listeningService) {}

  void ProcessAdd(Inquiry<Bond> &data) override {
    data.SetState(InquiryState::QUOTED);
    listeningService->OnMessage(data);
  }

  void ProcessRemove(Inquiry<Bond> &data) override {

  }

  void ProcessUpdate(Inquiry<Bond> &data) override {

  }

 private:
  BondInquiryService *listeningService;
};
#endif //BONDTRADINGSYSTEM_BONDINQUIRYSERVICE_H
