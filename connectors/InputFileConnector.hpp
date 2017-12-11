//
// Created by Harish Reddy on 12/10/2017.
//

#ifndef BONDTRADINGSYSTEM_INPUTFILECONNECTOR_H
#define BONDTRADINGSYSTEM_INPUTFILECONNECTOR_H

#include <string>
#include <fstream>
#include <iostream>
#include "../base/soa.hpp"
template<typename V>
class InputFileConnector : public Connector<V> {
 private:
  string filePath;
  Service *connectedService;

 public:
  void Publish(V &data) override {
    //do nothing since this is a subscribe only connector.
  }

  void read();
  virtual V parse(string line) = 0;

  InputFileConnector(const string &filePath, Service *connectedService)
      : filePath(filePath), connectedService(connectedService) {
    read();
  }
};

template<typename V>
void InputFileConnector<V>::read() {
  ifstream inFile;
  string line;
  inFile.open(filePath);
  if (!inFile) {
    std::cerr << "Unable to open file " << filePath;
    exit(1);   // call system to stop
  }
  inFile >> line; // skip headers

  while (inFile >> line) {
    connectedService->OnMessage(parse(line));
  }
  inFile.close();
}

#endif //BONDTRADINGSYSTEM_INPUTFILECONNECTOR_H
