#!/bin/bash

source=$1
type=${source##*.}
tmp=$(mktemp -d build.XXX)
cd $tmp

function cleanup() {
  echo "$1"
  cd ../
  rm -r $tmp
  exit
}

out=$(grep -o 'Output:.*' ../$1 -s | cut -f2- -d:)
if [ -z "$out" ]
then
  echo "Output filename not found"
  out="out"
fi

trap 'cleanup "BUILD ERROR"' ERR
trap 'cleanup "BRAKE"' HUP INT TERM STOP

case "$type" in
  "cpp")
    g++ -g -Wall -o ../$out ../$1
    ;;
  [cC])
    g++ -g -pedantic -Wall -o ../$out ../$1
    ;;
  *)
    cleanup "Bad programm type"
    exit
    ;;
esac
cleanup 'Successufully'
