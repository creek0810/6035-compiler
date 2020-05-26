OBJS = cli.o parser.o tokenizer.o symbolTable.o node.o interpreter.o object.o stack.o

cli: $(OBJS) cli.h
	cc -o cli $(OBJS)

object.o: object.h

cli.o: parser.h tokenizer.h

tokenizer.o: parser.h

tokenizer.h tokenizer.c: tokenizer.l
	flex --header-file=tokenizer.h -o tokenizer.c tokenizer.l

parser.h parser.c: parser.y
	bison -d -o parser.c parser.y

.PHONY: clean test test-final
clean:
	rm *.o parser.h parser.c tokenizer.h tokenizer.c result.out tmp.out
test:
	bash test.sh
test-final:
	bash testFinal.sh