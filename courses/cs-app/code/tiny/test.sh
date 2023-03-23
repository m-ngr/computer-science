#!/bin/bash

if [ "$1" == "-h" ]; then
  echo "Usage: `basename $0` [process-name] [port] [n-requests]"
  exit 0
fi

for ((i = $3; i < $4; i++))
do
	curl -s 127.0.0.1:$2 > /dev/null &
done

sleep 2 # give some time so that all threads are running

nthreads=$(ps -a -L | grep -o -i $1 | wc -l)
nthreads=$((nthreads - 1))

echo "threads = $nthreads"

if [ $4 == $nthreads ]; 
then
  echo "Test succeeded"
else
  echo "Test failed"
fi