#!/bin/bash

# define test function
test() {
    folder_path=$1
    program_path=$2

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
        echo "all test case passed"
    fi
}

test "../tests/final/testCase/pa" "../tests/final/program/a"
test "../tests/final/testCase/pb" "../tests/final/program/b"
test "../tests/final/testCase/pc" "../tests/final/program/c"
test "../tests/final/testCase/pd" "../tests/final/program/d"
test "../tests/final/testCase/pe" "../tests/final/program/e"
test "../tests/final/testCase/pf" "../tests/final/program/f"
test "../tests/final/testCase/pg" "../tests/final/program/g"
test "../tests/final/testCase/ph" "../tests/final/program/h"
test "../tests/final/testCase/pi" "../tests/final/program/i"
test "../tests/final/testCase/pj" "../tests/final/program/j"
