Matrix
===
Level 1:
Basic matrix class, ability to calculate determinant for floating points and integers

Level 2:
Class for compute optimal matrix chain multiplication order using dynamic programming

Requirements
===
The following applications have to be installed:
- CMake 3.10.2 version (or higher)
- GTest
- g++

How to build
===
To compile you need to use сmake in the directory build:
```
mkdir build
cd build
сmake ..
```
To compile all:
```
make
```

Running
===
To run tests:
```
./matrix_chain_tests
./matrix_tests
```

To run main executable:
```
./determinant
./chain_order
```

To run e2e tests:
```
cd tests/
./run_e2e.sh
```