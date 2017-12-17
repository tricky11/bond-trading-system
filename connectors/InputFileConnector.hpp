//
// Created by Harish Reddy on 12/10/2017.
//

#ifndef BONDTRADINGSYSTEM_INPUTFILECONNECTOR_H
#define BONDTRADINGSYSTEM_INPUTFILECONNECTOR_H

#include <string>
#include <fstream>
#include <iostream>
#include "../base/soa.hpp"

/**
 * This class is used to read data from files into services. Implementing classes should override the parse method.
 * Once the read method is called, it calls parse for each line of the input.
 *
 * @tparam K Key type of the connected service.
 * @tparam V Value type of the connected service.
 */
template<typename K, typename V>
class InputFileConnector : public Connector<V> {
 private:
  string filePath;

 protected:
  Service<K, V> *connectedService;

 public:
  virtual void parse(string line) = 0;

  void Publish(V &data) override {
    //do nothing since this is a subscribe only connector.
  }

  void read() {
    ifstream inFile;
    string line;
    inFile.open(filePath);
    if (!inFile) {
      std::cerr << "Unable to open file " << filePath;
      exit(1);   // call system to stop
    }
    inFile >> line; // skip headers

    while (inFile >> line) {
      parse(line);
    }
    inFile.close();
  }

  InputFileConnector(const string &filePath, Service<K, V> *connectedService)
      : filePath(filePath), connectedService(connectedService) {
  }
};

#endif //BONDTRADINGSYSTEM_INPUTFILECONNECTOR_H
