#!/bin/env python

import os

env = Environment(CXXFLAGS = "-Wall -g -O0")

if os.sys.platform == 'win32':
    env['OS_FAMILY'] = 'win'
else:
    env['OS_FAMILY'] = 'posix'

Export("env")

env.SConscript(['third_party/gmock/SConscript',
                'third_party/gtest/SConscript'])

env.Append(LIBS=['pthread', 'gtest', 'gtest_main', 'mongoclient',
                 'boost_thread-mt', 'boost_system', 'boost_regex'])
env.Append(LIBPATH=['third_party/gtest/lib', 'third_party/gmock/lib',
                    'third_party/boost/lib', 'third_party/mongo-cxx/lib'])
env.Append(CPPPATH=['third_party/gtest/include', 'third_party/gmock/include',
                    'third_party/boost/lib', 'third_party/mongo-cxx/include'])

env.Program(target='demo',
            source=['tools/demo.cpp'])


