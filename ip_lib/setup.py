#!/usr/bin/env python
#-*- coding:utf-8 -*-

from setuptools import setup, Extension

ip_lib = Extension('ip_lib', sources=["ip_lib.cpp"])
setup(ext_modules=[ip_lib])
