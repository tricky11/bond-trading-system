/**
 * positionservice.hpp
 * Defines the data types and Service for positions.
 *
 * @author Breman Thuraisingham
 */
#ifndef POSITION_SERVICE_HPP
#define POSITION_SERVICE_HPP

#include <string>
#include <map>
#include "soa.hpp"
#include "tradebookingservice.hpp"

using namespace std;

/**
 * Position class in a particular book.
 * Type T is the product type.
 */
template<typename T>
class Position {

 public:

  // ctor for a position
  Position(const T &_product);

  // Get the product
  const T &GetProduct() const;

  // Get the position quantity
  long GetPosition(string &book);

  // Get the aggregate position
  long GetAggregatePosition();

  // Updates the position after a new trade.
  void UpdatePosition(const Trade<T> &trade);
 private:
  T product;
  map<string, long> positions;

};

/**
 * Position Service to manage positions across multiple books and secruties.
 * Keyed on product identifier.
 * Type T is the product type.
 */
template<typename T>
class PositionService : public Service<string, Position<T> > {

 public:

  // Add a trade to the service
  virtual void AddTrade(const Trade<T> &trade) = 0;

};

template<typename T>
Position<T>::Position(const T &_product) :
    product(_product) {
}

template<typename T>
const T &Position<T>::GetProduct() const {
  return product;
}

template<typename T>
long Position<T>::GetPosition(string &book) {
  return positions[book];
}

template<typename T>
long Position<T>::GetAggregatePosition() {
  long totalPosition = 0;
  for (const auto &bookEntry : positions) {
    totalPosition += bookEntry.second;
  }
  return totalPosition;
}

template<typename T>
void Position<T>::UpdatePosition(const Trade<T> &trade) {
  long quantity = (trade.GetSide() == BUY ? 1 : -1) * trade.GetQuantity();
  if (positions.find(trade.GetBook()) == positions.end()) {
    positions[trade.GetBook()] = quantity;
  } else {
    positions[trade.GetBook()] += quantity;
  }
}

#endif
