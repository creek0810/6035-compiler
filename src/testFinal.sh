#!/bin/bash

# define test function
test() {
    problem_name=$1
    program_path=$2
    folder_path="../tests/final/testCase/$problem_name"

    flag=0
    for file_name in `ls $folder_path/in`
    do

        in_path="$folder_path/in/$file_name"
        out_path="$folder_path/out/${file_name%in.txt}out.txt"
        `./cli $program_path < $in_path > tmp.out`
        # write result
        `echo -e "\n$in_path" >> result.out`
        `diff -w tmp.out $out_path >> result.out`
        # show result
        ret_code=$?
        if [ $ret_code == 1 ]; then
            echo "$in_path: failed"
            flag=1
        fi
    done

    if [ $flag == 0 ]; then
        echo "problem $problem_name passed"
    fi
}

test "pa" "../tests/final/program/a"
test "pb" "../tests/final/program/b"
test "pc" "../tests/final/program/c"
test "pd" "../tests/final/program/d"
test "pe" "../tests/final/program/e"
test "pf" "../tests/final/program/f"
test "pg" "../tests/final/program/g"
test "ph" "../tests/final/program/h"
test "pi" "../tests/final/program/i"
test "pj" "../tests/final/program/j"
test "p93" "../tests/final/program/93"
test "p81" "../tests/final/program/81"
