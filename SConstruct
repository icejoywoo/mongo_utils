#!/bin/env python
# encoding: utf-8

import os

# -Wunused-local-typedef 屏蔽mac编译时大量的错误
env = Environment(CXXFLAGS = "-Wall -g -O0 -Wno-unused-local-typedef")

if os.sys.platform == 'linx':
    env.Append(LIBS=['rt'])

env.SConscript(['third_party/gmock/SConscript',
                'third_party/gtest/SConscript',
                'third_party/gflags/SConscript',
                'third_party/glog/SConscript'])

env.Append(LIBS=['pthread', 'gtest', 'gtest_main', 'glog', 'gflags', 'mongoclient',
                 'boost_thread-mt', 'boost_system', 'boost_regex'])
env.Append(LIBPATH=['third_party/gtest/lib', 'third_party/gmock/lib',
                    'third_party/glog/lib', 'third_party/gflags/lib',
                    'third_party/boost/lib', 'third_party/mongo-cxx/lib'])
env.Append(CPPPATH=['third_party/gtest/include', 'third_party/gmock/include',
                    'third_party/glog/include', 'third_party/gflags/include',
                    'third_party/boost/include', 'third_party/mongo-cxx/include'])

env.Program(target='demo',
            source=['tools/demo.cpp'])

