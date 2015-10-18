#!/bin/env python
# encoding: utf-8

import os

# --implicit-cache
SetOption('implicit_cache', 1)

# -Wunused-local-typedef 屏蔽mac编译时大量的错误
env = Environment(CXXFLAGS=['-Wall', '-g', '-O0', '-Wno-unused-local-typedef'])

if 'linux' in os.sys.platform:
    env.Append(LIBS=['rt'])

Export("env")

env.SConscript(['third_party/gmock/SConscript',
                'third_party/gtest/SConscript',
                'third_party/gflags/SConscript',
                'third_party/glog/SConscript',
                'third_party/yaml-cpp-0.5.1/SConscript'])

env.Append(LIBS=['pthread', 'gtest', 'gtest_main', 'glog', 'gflags', 'mongoclient', 'yaml',
                 'boost_thread-mt', 'boost_system', 'boost_regex'])
env.Append(LIBPATH=['third_party/gtest/lib', 'third_party/gmock/lib',
                    'third_party/glog/lib', 'third_party/gflags/lib',
                    'third_party/boost/lib', 'third_party/mongo-cxx/lib',
                    'third_party/yaml-cpp-0.5.1/lib'])
env.Append(CPPPATH=['third_party/gtest/include', 'third_party/gmock/include',
                    'third_party/glog/include', 'third_party/gflags/include',
                    'third_party/boost/include', 'third_party/mongo-cxx/include',
                    'third_party/yaml-cpp-0.5.1/include', 'base'])

for source in Glob('tools/*.cpp'):
    env.Program(target=''.join(os.path.basename(str(source)).split('.')[:-1]),
                source=[source])

