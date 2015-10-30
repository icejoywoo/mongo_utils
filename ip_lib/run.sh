#!/bin/bash

set -e

python setup.py build

cp build/lib.*/ip_lib.so .

python test.py
