//
// Created by Harish Reddy on 12/11/2017.
//

#ifndef BONDTRADINGSYSTEM_OUTPUTFILECONNECTOR_H
#define BONDTRADINGSYSTEM_OUTPUTFILECONNECTOR_H

#include <string>
#include <fstream>
#include <iostream>
#include "../base/soa.hpp"
/**
 * This connector writes data to output files.
 * Implementing classes have to implement a method to convert the object into string form,
 * as well as an optional header method.
 * If a file already exists, its contents are truncated.
 * If a file does not exist, a new one is created.
 *
 * @tparam V Type of the data that is being written.
 */
template<typename V>
class OutputFileConnector : public Connector<V> {
 private:
  string filePath;

  void appendLineToFile(string line, bool newFile) {
    ofstream outFile;
    outFile.open(filePath, newFile ? ios_base::trunc : ios_base::app);
    if (!outFile) {
      cerr << "Unable to open file " << filePath;
      exit(1);   // call system to stop
    }
    outFile << line << endl;
    outFile.close();
  }

 public:
  void Publish(V &data) override {
    appendLineToFile(toCSVString(data), false);
  }

  void WriteHeader() {
    appendLineToFile(getCSVHeader(), true);
  }

  virtual string toCSVString(V &data) = 0;
  virtual string getCSVHeader() = 0;

  explicit OutputFileConnector(const string &filePath) : filePath(filePath) {}
};

#endif //BONDTRADINGSYSTEM_OUTPUTFILECONNECTOR_H
