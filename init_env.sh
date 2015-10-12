#!/bin/bash

CURRENT_DIR=$(cd $(dirname ${BASH_SOURCE[0]}); pwd)

ENV_PATH="${CURRENT_DIR}/env"

cd ${ENV_PATH}

# build boost
BOOST_PATH="${ENV_PATH}/boost_1_59_0"
if [ -d "${BOOST_PATH}" ]; then
    rm -rf ${BOOST_PATH}
fi

BOOST_RELEASE_PATH="${CURRENT_DIR}/third_party/boost"
if [ -d "${BOOST_RELEASE_PATH}" ]; then
    rm -rf ${BOOST_RELEASE_PATH}
fi

tar xzf "${ENV_PATH}/boost_1_59_0.tar.gz" -C "${ENV_PATH}"

${BOOST_PATH}/bootstrap.sh --prefix=${BOOST_RELEASE_PATH}

${BOOST_PATH}/b2 -j 32 install

# mongo cxx driver
MONGO_CXX_PATH="${ENV_PATH}/mongo-cxx-driver-legacy-1.0.5"
if [ -d "${MONGO_CXX_PATH}" ]; then
    rm -rf ${MONGO_CXX_PATH}
fi

MONGO_CXX_RELEASE_PATH="${CURRENT_DIR}/third_party/mongo_cxx"
if [ -d "${MONGO_CXX_RELEASE_PATH}" ]; then
    rm -rf ${MONGO_CXX_RELEASE_PATH}
fi

tar xzf "${ENV_PATH}/mongo-cxx-driver-legacy-1.0.5.tar.gz" -C "${ENV_PATH}"

cd ${MONGO_CXX_PATH}
scons -j 32 --prefix=${MONGO_CXX_RELEASE_PATH} --dbg=on --opt=on  install
