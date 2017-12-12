//
// Created by Harish Reddy on 12/12/2017.
//

#ifndef BONDTRADINGSYSTEM_STRING_H
#define BONDTRADINGSYSTEM_STRING_H

#include<vector>
#include<string>
#include<sstream>

using namespace std;
vector<string> splitString(string input, char delimiter) {
  stringstream inputstream(input);
  string element;
  vector<string> result;
  while (getline(inputstream, element, delimiter)) {
    result.push_back(element);
  }
  return result;
}
#endif //BONDTRADINGSYSTEM_STRING_H
