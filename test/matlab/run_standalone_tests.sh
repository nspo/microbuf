#!/bin/bash
cd "${0%/*}"

octave-cli test_matlab_deserialization.m
octave-cli test_matlab_serialization.m
