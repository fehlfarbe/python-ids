#!/bin/bash

# creates wrapper and builds shared library
swig -v -c++ -python idscamera.i && python setup.py build_ext --inplace
