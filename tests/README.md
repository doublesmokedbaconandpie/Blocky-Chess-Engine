## Running Tests

In order to get started making changes to Blocky, you need to be able to run the unit tests and check that they are passing. Similar to how CMake is run with the release binary, tests can be run like so:

```
cmake -S . -B build
cmake --build build
( cd build ; ctest )
```

CTest has a variety of options to customize output. If you want to use console logging for debugging, you need to add an output on failure flag to the ctest command. For example:

```
ctest --output-on-failure
```
