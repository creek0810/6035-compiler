#!/bin/bash

# define test function
test() {
    test_type=$1
    cli_arg=$2
    folder_path="../tests/$test_type"

    flag=0
    for file_name in `ls $folder_path/in`
    do
        in_path="$folder_path/in/$file_name"
        out_path="$folder_path/out/${file_name%.in}.out"
        `./cli $cli_arg $in_path > tmp.out`
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
        echo "$test_type test passed"
    fi
}

# tokenizer test
test "tokenizer" "-t"

# parser test
test "parser" "-p"

# interpreter test
test "interpreter" " "
