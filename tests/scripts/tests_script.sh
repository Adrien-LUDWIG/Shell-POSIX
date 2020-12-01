#!/bin/bash

test_script()
{
    (exec "./42sh" "${args}" > "actual")
    actual_err=$?
    (exec "bash" "--posix" "${args}" > "expected")
    expected_err=$?

    if [ ${actual_err} -eq ${expected_err} ]
    then
        echo -e '\e[1;32m PASSED \e[0m' "${args}"
    else
        echo -e '\e[1;31m ERROR \e[0m' "${args}" "return values do not match"
    fi

    diff -u "actual" "expected"
}

declare -a scripts=("cmd_if_if_cmd.sh")

for i in $(seq 1 ${#scripts[@]})
do
    args="./scripts/${scripts[i-1]}"
    test_script
done