#!/bin/bash

set -e

CURRENT_DIR=$(cd $(dirname ${BASH_SOURCE[0]}); pwd)

ENV_PATH="${CURRENT_DIR}/env"

PARALLEL=32

cd ${ENV_PATH}

function build_boost()
{
    FLAG="${CURRENT_DIR}/.build_boost"
    if [ -f "${FLAG}" ]; then
        echo "boost is already build."
        return 0
    fi

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

    cd ${BOOST_PATH}

    ${BOOST_PATH}/bootstrap.sh --prefix=${BOOST_RELEASE_PATH}

    ${BOOST_PATH}/b2 --build-type=complete --layout=tagged -j ${PARALLEL} link=static install

    touch ${FLAG}
    return 0
}

function build_mongo_cxx_driver()
{
    FLAG="${CURRENT_DIR}/.build_mongo_cxx_driver"
    if [ -f "${FLAG}" ]; then
        echo "mongo_cxx_driver is already build."
        return 0
    fi

    # mongo cxx driver
    MONGO_CXX_PATH="${ENV_PATH}/mongo-cxx-driver-legacy-1.0.5"
    if [ -d "${MONGO_CXX_PATH}" ]; then
        rm -rf ${MONGO_CXX_PATH}
    fi

    MONGO_CXX_RELEASE_PATH="${CURRENT_DIR}/third_party/mongo-cxx"
    if [ -d "${MONGO_CXX_RELEASE_PATH}" ]; then
        rm -rf ${MONGO_CXX_RELEASE_PATH}
    fi

    tar xzf "${ENV_PATH}/mongo-cxx-driver-legacy-1.0.5.tar.gz" -C "${ENV_PATH}"

    cd ${MONGO_CXX_PATH}
    scons -j ${PARALLEL} --prefix=${MONGO_CXX_RELEASE_PATH} --dbg=on --opt=on --cpppath=${BOOST_RELEASE_PATH}/include --libpath=${BOOST_RELEASE_PATH}/lib install

    touch ${FLAG}
    return 0
}

function build_libevent()
{
    FLAG="${CURRENT_DIR}/.build_libevent"
    if [ -f "${FLAG}" ]; then
        echo "libevent is already build."
        return 0
    fi
    # libevent
    LIBEVENT_PATH="${ENV_PATH}/libevent-2.0.22-stable"
    if [ -d "${LIBEVENT_PATH}" ]; then
        rm -rf ${LIBEVENT_PATH}
    fi

    LIBEVENT_RELEASE_PATH="${CURRENT_DIR}/third_party/libevent"
    if [ -d "${LIBEVENT_RELEASE_PATH}" ]; then
        rm -rf ${LIBEVENT_RELEASE_PATH}
    fi

    tar xzf "${ENV_PATH}/libevent-2.0.22-stable.tar.gz" -C "${ENV_PATH}"

    cd ${LIBEVENT_PATH}
    ./configure --prefix=${LIBEVENT_RELEASE_PATH} --disable-shared --enable-static --disable-openssl
    make -j ${PARALLEL}
    make install

    touch ${FLAG}
    return 0
}

function build_libev()
{
    FLAG="${CURRENT_DIR}/.build_libev"
    if [ -f "${FLAG}" ]; then
        echo "libev is already build."
        return 0
    fi

    BUILD_PATH="${ENV_PATH}/libev-4.20"
    if [ -d "${BUILD_PATH}" ]; then
        rm -rf ${BUILD_PATH}
    fi

    INSTALL_PATH="${CURRENT_DIR}/third_party/libev"
    if [ -d "${INSTALL_PATH}" ]; then
        rm -rf ${INSTALL_PATH}
    fi

    tar xzf "${ENV_PATH}/libev-4.20.tar.gz" -C "${ENV_PATH}"

    cd ${BUILD_PATH}
    ./configure --prefix=${INSTALL_PATH} --disable-shared --enable-static
    make -j ${PARALLEL}
    make install

    touch ${FLAG}
    return 0
}

function build_libevent1()
{
    # 为了thrift的TNonblockingServer
    FLAG="${CURRENT_DIR}/.build_libevent1"
    if [ -f "${FLAG}" ]; then
        echo "libevent1 is already build."
        return 0
    fi
    # libevent
    LIBEVENT_PATH="${ENV_PATH}/libevent-1.4.14b-stable"
    if [ -d "${LIBEVENT_PATH}" ]; then
        rm -rf ${LIBEVENT_PATH}
    fi

    LIBEVENT_RELEASE_PATH="${CURRENT_DIR}/third_party/libevent1"
    if [ -d "${LIBEVENT_RELEASE_PATH}" ]; then
        rm -rf ${LIBEVENT_RELEASE_PATH}
    fi

    tar xzf "${ENV_PATH}/libevent-1.4.14b-stable.tar.gz" -C "${ENV_PATH}"

    cd ${LIBEVENT_PATH}
    ./configure --prefix=${LIBEVENT_RELEASE_PATH} --disable-shared --enable-static --disable-openssl
    make -j ${PARALLEL}
    make install

    touch ${FLAG}
    return 0
}

function build_thrift()
{
    # mac 下无法编译
    NAME="thrift"
    VERSION="0.9.3"
    FLAG="${CURRENT_DIR}/.build_${NAME}"
    if [ -f "${FLAG}" ]; then
        echo "${NAME} is already build."
        return 0
    fi

    BUILD_PATH="${ENV_PATH}/thrift-${VERSION}"
    if [ -d "${BUILD_PATH}" ]; then
        rm -rf ${BUILD_PATH}
    fi

    INSTALL_PATH="${CURRENT_DIR}/third_party/thrift"
    if [ -d "${INSTALL_PATH}" ]; then
        rm -rf ${INSTALL_PATH}
    fi

    tar xzf "${ENV_PATH}/thrift-${VERSION}.tar.gz" -C "${ENV_PATH}"

    cd ${BUILD_PATH}
    ./bootstrap.sh
    ./configure --prefix=${INSTALL_PATH} --with-boost=${CURRENT_DIR}/third_party/boost/ --with-libevent=${CURRENT_DIR}/third_party/libevent1/ \
        --without-go --without-nodejs --with-cpp --without-tests --enable-static --disable-shared
    make -j ${PARALLEL}
    make install

    touch ${FLAG}
    return 0
}

function build_poco()
{
    NAME="poco"
    VERSION="1.6.1"
    FLAG="${CURRENT_DIR}/.build_${NAME}"
    if [ -f "${FLAG}" ]; then
        echo "${NAME} is already build."
        return 0
    fi

    BUILD_PATH="${ENV_PATH}/${NAME}-${VERSION}"
    if [ -d "${BUILD_PATH}" ]; then
        rm -rf ${BUILD_PATH}
    fi

    INSTALL_PATH="${CURRENT_DIR}/third_party/${NAME}"
    if [ -d "${INSTALL_PATH}" ]; then
        rm -rf ${INSTALL_PATH}
    fi

    tar xzf "${ENV_PATH}/${NAME}-${VERSION}.tar.gz" -C "${ENV_PATH}"

    cd ${BUILD_PATH}
    ./configure --prefix=${INSTALL_PATH} --static --no-tests --no-samples
    make -j ${PARALLEL}
    make install

    touch ${FLAG}
    return 0
}

build_boost
build_mongo_cxx_driver
build_libevent
build_libev
build_libevent1
build_thrift
build_poco

