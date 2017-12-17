# MTH 9815 Final Project : Bond Trading System

This project demonstrates the use of Service Oriented Architecture and implements a Bond Trading System which can trade six US Treasuries: 2Y, 3Y, 5Y, 7Y, 10Y, and 30Y.
ServiceListeners are used to pass data between services. Connectors are used for I/O.
## Generating data
* Use the provided scripts/input_data_generator.py to generate trades, prices, inquiries and market data.
* Run input_data_generator.py -h for help and more details on how to generate data set. 
* By default, it generates all datasets and full size (WARNING: Takes too long to run!)

## Prerequisites
* Boost 1.58.0 (uses dates for maturities), CMake 3.8 for building
* Input is expected to be in the input directory (default for above script)
* Application needs to have write access to directory 'output' inside the root of the project

## Building and running
* cmake .
* make
* ./BondTradingSystem

## Highlights
* Uses OOP to abstract common code and follow DRY principle. (Ex. InputFileConnector)
* Customizable input generation script through flags & help manual.
* Uses CMake for easy building.
* Human readable CSV file format for input and output.
* Clean organization of code : base, implementation, scripts, utils, input, output.

## Owner
Harishchandra Reddy (harish.7000@gmail.com)
