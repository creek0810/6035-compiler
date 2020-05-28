@echo off
win_flex --header-file=tokenizer.h -o tokenizer.c tokenizer.l
win_bison -d -o parser.c parser.y

gcc -c cli.c -std=gnu99
gcc -c parser.c -std=gnu99
gcc -c tokenizer.c -std=gnu99
gcc -c symbolTable.c -std=gnu99
gcc -c node.c -std=gnu99
gcc -c interpreter.c -std=gnu99
gcc -c object.c -std=gnu99
gcc -c stack.c -std=gnu99
gcc -o cli cli.o parser.o tokenizer.o symbolTable.o node.o interpreter.o object.o stack.o
