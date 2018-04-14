#!/bin/bash

g++ -std=c++11 -pthread csma.cpp -o csma

if [ $? -eq 0 ]
then
    ./csma

fi