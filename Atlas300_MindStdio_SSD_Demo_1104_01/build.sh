#!/bin/sh
export LD_LIBRARY_PATH=/home/HwHiAiUser/tools/che/ddk/ddk/uihost/lib/
TOP_DIR=${PWD}
BUILD_DIR=${TOP_DIR}/build
LOCAL_OBJ_PATH=${TOP_DIR}/out
cd ${TOP_DIR} && make
if [ $? != 0 ]
then
  rm ${LOCAL_OBJ_PATH}/*.o ${LOCAL_OBJ_PATH}/main
  exit
fi
cp graph.config ./out/
cd ${BUILD_DIR}
if [ -d "host" ];then
  if [ -f "host/Makefile" ];then
    cd host && make install
    if [ $? != 0 ]
    then
      rm ${LOCAL_OBJ_PATH}/*.o ${LOCAL_OBJ_PATH}/main
      rm ${LOCAL_OBJ_PATH}/*.so
      exit
    fi
  fi
fi
cd ${BUILD_DIR}
if [ -d "device" ];then
  if [ -f "device/Makefile" ];then
    cd device && make install
    if [ $? != 0 ]
    then
      rm ${LOCAL_OBJ_PATH}/*.o ${LOCAL_OBJ_PATH}/main
      rm ${LOCAL_OBJ_PATH}/*.so
      exit
    fi
  fi
fi
echo "============ build success ! ============"
