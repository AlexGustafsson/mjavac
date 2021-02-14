#!/usr/bin/env bash

DIRECTORY="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

mjavac="$1"
if [[ -z "$mjavac" ]]; then
  echo "Usage: $0 ./path/to/mjavac"
  exit 1
fi

function test_files() {
  files="$*"
  for file in $files; do
    if "$mjavac" "$file" &> /dev/null; then
      echo -e "✔ \e[32m$file\e[0m"
    else
      echo -e "✗ \e[31m$file\e[0m"
    fi
  done
}

# TODO: Add test headers to each file?
# mjavac test header
# parse: succeed
# semantics: fail
# compile fail:
# output: 10
# hello world
#
# Code...

test_files "$DIRECTORY/correct/"*.java
test_files "$DIRECTORY/examples/"*.java
test_files "$DIRECTORY/incorrect/"*.java
