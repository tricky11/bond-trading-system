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

double convertFractionalPriceToDouble(string price) {
  auto split = splitString(price, '-');
  if (split.size() != 2) {
    //invalid input
    return 0.0;
  }
  double integerPart = stod(split[0]);
  double firstFractionalPart = stod(split[1].substr(0,2)) / 32.0;
  double secondFractionalPart = ((split[1][2] == '+') ? 4 : (split[1][2] - '0')) / 256.0;
  return integerPart + firstFractionalPart + secondFractionalPart;
}
#endif //BONDTRADINGSYSTEM_STRING_H
