//
// Created by Harish Reddy on 12/11/2017.
//

#include "GUIService.hpp"

using namespace std;

BondPriceServiceListener::BondPriceServiceListener(GUIService *listeningService) : listeningService(listeningService) {}

void BondPriceServiceListener::ProcessAdd(Price<Bond> &data) {
  listeningService->PersistData(data);
}
void BondPriceServiceListener::ProcessRemove(Price<Bond> &data) {

}
void BondPriceServiceListener::ProcessUpdate(Price<Bond> &data) {

}

GUIConnector::GUIConnector(const string &filePath) : OutputFileConnector(filePath) {}

string GUIConnector::toCSVString(Price<Bond> &data) {
  return "Dummy gui string";
}
string GUIConnector::getCSVHeader() {
  return "Dummy gui header";
}
void GUIService::PersistData(Price<Bond> &data) {
  connector->Publish(data);
}
void GUIService::OnMessage(Price<Bond> &data) {

}
GUIService::GUIService(const unsigned int throttle) : throttle(throttle) {
  connector = new GUIConnector("gui.csv");
  connector->WriteHeader();
}
