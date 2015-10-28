#!/bin/bash

set -e

python setup.py build

cp build/lib.linux-x86_64-2.7/ip_lib.so .

python test.py
