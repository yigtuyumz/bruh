#!/usr/bin/env bash

file="./a.out"

if ! [ -f "${file}" ]; then
  echo "${file} not found."
  exit 1
fi

executable="${file} NULL"
port=1

while [ ${port} -lt 65536 ]; do
  eval "${executable} ${port}" > /dev/null
  e=$?
  if [ $e -gt 0 ]; then
    printf "port: %d, fields found: %d\n" "${port}" "$e"
  fi
  ((port++))
done

