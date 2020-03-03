OBJS = compiler.o parser.o tokenizer.o symbolTable.o node.o

compiler: $(OBJS) compiler.h
	cc -o compiler $(OBJS)

compiler.o: parser.h tokenizer.h

tokenizer.o: parser.h

tokenizer.h tokenizer.c: tokenizer.l
	flex --header-file=tokenizer.h -o tokenizer.c tokenizer.l

parser.h parser.c: parser.y
	bison -d -o parser.c parser.y

.PHONY: clean test
test:
	bash test.sh
clean:
	rm *.o parser.h parser.c tokenizer.h tokenizer.c result.out tmp.out