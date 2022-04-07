#!/bin/bash

set -e

if [[ -z $1 ]]; then
  echo "Uso: copiar-script.sh «tp» «test»"
  exit 1
fi
if [[ -z $2 ]]; then
  echo "Uso: copiar-script.sh «tp» «test»"
  exit 1
fi

TEST=$2
DST=$1

cp "${TEST}/taskLemmingA.c" "${DST}/taskLemmingA.c"
cp "${TEST}/taskLemmingB.c" "${DST}/taskLemmingB.c"
cp "${TEST}/mapa.c" "${DST}/mapa.c"

cat "${TEST}/README.md"
