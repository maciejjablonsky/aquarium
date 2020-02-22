#!/bin/bash
INIT=$(pwd)
mkdir -p build
rm -r build/*
cd build
cmake ..
mkdir -p bin
make
mv test_* bin

red=$'\e[1;31m'
grn=$'\e[1;32m'
yel=$'\e[1;33m'
blu=$'\e[1;34m'
mag=$'\e[1;35m'
cyn=$'\e[1;36m'
end=$'\e[0m'

PASSED=0
ALL=0
FAILED_TESTS=""
printf "\n"
cd "$INIT"
for FILE in build/bin/* ; do
    printf "${yel} Running: $(echo ${FILE} | sed 's#.*/##g') ${end}\n"
    ./"$FILE"
    if (( $? == 0 ))  ; then
        PASSED=$((PASSED + 1))
    else
        FAILED_TESTS="${FAILED_TESTS} ${red}- $(echo ${FILE} | sed 's#.*/##g')${end}\n"
    fi
    ALL=$((ALL + 1))
done

printf "\n${mag}===========================================${end}\n"
if [[ "$PASSED" == "$ALL" ]]; then
printf "${grn}"
else
printf "${cyn}"
fi
printf "\n$PASSED/$ALL tests passed.\n${end}"
if [[ -n ${FAILED_TESTS} ]]; then
    printf "${red}\nFailed tests:\n${end}"
    printf "${FAILED_TESTS}"
else
    printf "\n${grn}All tests PASSED${end}.\n"
fi