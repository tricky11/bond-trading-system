//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_OUTPUTFILECONNECTOR_H
#define BONDTRADINGSYSTEM_OUTPUTFILECONNECTOR_H

#include <string>
#include <fstream>
#include <iostream>
#include "../base/soa.hpp"
template<typename V>
class OutputFileConnector : public Connector<V> {
 private:
  string filePath;

 public:
  void Publish(V &data) override {
    appendLineToFile(toCSVString(data));
  }
  void appendLineToFile(string line) {
    ofstream outFile;
    outFile.open(filePath, ios_base::ate);
    if (!outFile) {
      cerr << "Unable to open file " << filePath;
      exit(1);   // call system to stop
    }
    outFile << line << endl;
    outFile.close();
  }

  virtual string toCSVString(V &data) = 0;
  virtual void writeHeader() = 0;

  OutputFileConnector(const string &filePath, Service *connectedService)
      : filePath(filePath) {
  }
};

#endif //BONDTRADINGSYSTEM_OUTPUTFILECONNECTOR_H
