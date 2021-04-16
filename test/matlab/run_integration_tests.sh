#!/bin/bash
cd "${0%/*}"

MATLAB_FILE=../../build/TestMessage1_serialized_by_matlab.bin
CPP_FILE=../../build/TestMessage1_serialized_by_cpp.bin

echo ""
echo "- Running MATLAB deserialization for data serialized by MATLAB"
BINARY_DATA_OUT_FILE=$MATLAB_FILE octave-cli test_TestMessage1_serialization.m
BINARY_DATA_IN_FILE=$MATLAB_FILE octave-cli test_TestMessage1_deserialization.m

echo ""
echo "- Running MATLAB deserialization for data serialized by C++ (C++ tests must have been run!)"
BINARY_DATA_IN_FILE=$CPP_FILE octave-cli test_TestMessage1_deserialization.m

echo ""
echo "- Checking if C++ and MATLAB serialization results are equal"
diff -q $CPP_FILE $MATLAB_FILE
