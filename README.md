# Creepiler
An interpreter for mixing of python and c.

## Build

### Windows
```
cd src
make.bat
```

### macOS
```
cd src
make
```

## Test(only for macOS)
Test basic tokenizer, parser and interpreter behavior.
```
cd src
make test
```

Use some simple programs to test interpreter.
```
cd src
make test-final
```

## About language

### Comment style
The comment style is same as c.
```
// this is a inline comment

/*
    this is a block comment
    this is a block comment
    this is a block comment
*/
```

### Type
- integer (-9,223,372,036,854,775,808 ~ 9,223,372,036,854,775,807)
- double (1.7E +/- 308)
- string
- list (element can be integer, double or string)

### Assign and declare
This part is same as python.
```
// integer
a = 10;
// double
b = 10.5;
// string
c = "123";
// array
d = [1, "123", 4.3];

```

### For
```
for(i=0; i<10; i=i+1) {
    print("hello\n");
}
```

### If
```
a = 1;
if(a == 1) {
    print("a == 1");
} else {
    print("a != 1");
}
```

### Built-in function
```
print(a); // print variable a
len(a); // get length of variable a
str(a); // convert a to string type
int(a); // convert a to integer type
double(a); // convert a to double type
a = input(); // read input
append(a, 123) // append 123 into list a
```

### Variable lifetime and scope
Same as c.
```
a = 0;
for(i=0; i<10; i=i+1) {
    a = a + 1;
    c = 10;
}
print(a); // 10
print(c); // error
```
