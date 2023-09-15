## Running Tests

In order to get started making changes to Blocky, you need to be able to run the unit tests and check that they are passing. ```runTests.sh``` has example code to run the tests, but in general, building and running the tests is similar to building Blocky itself.

If you are on Linux, the following commands will build and run the tests.

```
cmake -S . -B build
cmake --build build
( cd build ; ctest
```

If you are on Windows, the following commands will work instead:

```
cmake -S . -B build -DMAKE_EXE=ON
cmake --build build
( cd build ; ctest
```
