#!/bin/sh

GLEW_DIR=$1
OUTPUT_DIR=Base

function usage() {
 echo "usage: $0 </path/to/glew/>"
 exit -1
}

function test_glew_path() {
 if test "x" = "x${GLEW_DIR}"; then
  usage
 fi

 if test -d "${GLEW_DIR}" -a -f "${GLEW_DIR}/auto/src/glew.rc"; then :; else
  usage
 fi

# OUTPUT_DIR=$(echo ${OUTPUT_DIR} | sed -e 's/\///g')
 OUTPUT_DIR_ESC=$(echo ${OUTPUT_DIR%/} | sed -e 's/\//\\\//g')
 if test -d "${OUTPUT_DIR}"; then :; else
   echo "$0: no valid output-directory: ${OUTPUT_DIR}"
   exit -1
 fi
}

function remake_glew() {
 # delete previously downloaded extensions
 make -C ${GLEW_DIR}/auto destroy
 # get extensions from http://oss.sgi.com/
 make -C ${GLEW_DIR}/auto
}

function gemify_glew() {
 if test -f "$1"; then
   sed -e "s/<GL\/\(.*gl.*ew.*\)>/\"${OUTPUT_DIR_ESC}\/\1\"/" $1
 fi
}

# test whether the user has provided enough information
test_glew_path

# rebuild glew
remake_glew

echo '#include "Base/config.h"' > ${OUTPUT_DIR}/glew.cpp
echo '#ifdef USE_GLEW' >> ${OUTPUT_DIR}/glew.cpp
gemify_glew ${GLEW_DIR}/src/glew.c >> ${OUTPUT_DIR}/glew.cpp
echo '#endif /* USE_GLEW */' >> ${OUTPUT_DIR}/glew.cpp

for f in glew.h glxew.h wglew.h
do
 gemify_glew ${GLEW_DIR}/include/GL/${f} > ${OUTPUT_DIR}/${f}
done




