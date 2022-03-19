# mrt
C++ Tools  

# Overwriew
The library mainly consists of:  
 - Thread Utils (ThreadPool, Executors, Futures, etc)
 - Test Framework
 - String Utils
 - Command Line Argument Parser
 - Config File Parser
 - Optional
 - Template String Formatter
 - Container Utils
 - Logger
 - Wrappers (File, DynamicLibrary)
 - Console IO Utilities
 - HTTP Request Support (in progress)
 - Proccess Wrapper (in progress)

# How to install:
```sh
git clone https://github.com/maxrt101/mrt.git
cd mrt
make
```
The install copies headers to an installation location.  
You can specify install folder, by specifying `PREFIX`, e.g.   
```sh
make PREFIX=/path/to/installation/folder
```
The header files will be installed into `/path/to/installation/folder/include/mrt`  
Default `PREFIX` is `build/`  

# Tests
Tests are located in `tests/` folder.  
Every `test_*.cc` file is a test for some `mrt` component.  
Tests are built into `build/test/test_*.so` files, which can then be run separately using `build/bin/mrt_test`.
```sh
make test # to build & run tests
make test RUN=0 # to build tests
./build/bin/mrt_test TESTS # to run individual tests (TESTS is one or more .so files from build/test)
```
