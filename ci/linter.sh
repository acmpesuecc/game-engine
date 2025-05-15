#!/bin/bash

clangformat() {
  python3 ./ci/run-clang-format.py -r src/ include/engine

  find src/ include/engine/ -iname '*.h' -o -iname '*.cpp' -o -iname '*.hpp' | clang-format --style=file -i --files=/dev/stdin
}

clangtidy() {
  find src/ -iname '*.h' -o -iname '*.cpp' -o -iname '*.hpp' |
    grep -v "include/vendored" |
    xargs clang-tidy -p=build \
      -header-filter='^(src/).*' \
      -checks='cppcoreguidelines-*' \
      --quiet

  exit 0
}

case "$1" in
clangformat)
  clangformat
  ;;
clangtidy)
  clangtidy
  ;;
*)
  echo "Invalid command!"
  ;;
esac
