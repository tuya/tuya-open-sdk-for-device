# coreHTTP Client Library

This repository contains a C language HTTP client library designed for embedded
platforms. It has no dependencies on any additional libraries other than the
standard C library, [http-parser](https://github.com/nodejs/http-parser), and
a customer-implemented transport interface. This library is distributed under
the [MIT Open Source License](LICENSE).

This library has gone through code quality checks including verification that no
function has a [GNU Complexity](https://www.gnu.org/software/complexity/manual/complexity.html)
score over 8. This library has also undergone both static code analysis from
[Coverity static analysis](https://scan.coverity.com/), and validation of memory
safety and data structure invariance through the
[CBMC automated reasoning tool](https://www.cprover.org/cbmc/).

See memory requirements for this library [here](https://docs.aws.amazon.com/embedded-csdk/202103.00/lib-ref/libraries/standard/coreHTTP/docs/doxygen/output/html/index.html#http_memory_requirements).

**coreHTTP v2.0.0 [source code](https://github.com/FreeRTOS/coreHTTP/tree/v2.0.0/source) is part of the [FreeRTOS 202012.00 LTS](https://github.com/FreeRTOS/FreeRTOS-LTS/tree/202012.00-LTS) release.**

## coreHTTP Config File

The HTTP client library exposes configuration macros that are required for
building the library. A list of all the configurations and their default values
are defined in [core_http_config_defaults.h](source/include/core_http_config_defaults.h).
To provide custom values for the configuration macros, a custom config file
named `core_http_config.h` can be provided by the user application to the library.

By default, a `core_http_config.h` custom config is required to build the
library. To disable this requirement and build the library with default
configuration values, provide `HTTP_DO_NOT_USE_CUSTOM_CONFIG` as a compile time
preprocessor macro.

**The HTTP client library can be built by either**:
* Defining a `core_http_config.h` file in the application, and adding it to the
include directories for the library build.
**OR**
* Defining the `HTTP_DO_NOT_USE_CUSTOM_CONFIG` preprocessor macro for the
library build.

## Building the Library

The [httpFilePaths.cmake](httpFilePaths.cmake) file contains the information of
all source files and header include paths required to build the HTTP client
library.

As mentioned in the [previous section](#coreHTTP-Config-File), either a custom
config file (i.e. `core_http_config.h`) OR `HTTP_DO_NOT_USE_CUSTOM_CONFIG` macro
needs to be provided to build the HTTP client library.

For a CMake example of building the HTTP library with the `httpFilePaths.cmake`
file, refer to the `coverity_analysis` library target in
[test/CMakeLists.txt](test/CMakeLists.txt) file.

## Building Unit Tests

### Platform Prerequisites

- For running unit tests, the following are required:
    - **C90 compiler** like gcc
    - **CMake 3.13.0 or later**
    - **Ruby 2.0.0 or later** is required for this repository's
      [CMock test framework](https://github.com/ThrowTheSwitch/CMock).
- For running the coverage target, the following are required:
    - **gcov**
    - **lcov**

### Steps to build **Unit Tests**

1. Go to the root directory of this repository.

1. Run the *cmake* command: `cmake -S test -B build -DBUILD_CLONE_SUBMODULES=ON `

1. Run this command to build the library and unit tests: `make -C build all`

1. The generated test executables will be present in `build/bin/tests` folder.

1. Run `cd build && ctest` to execute all tests and view the test run summary.

## Reference examples

The AWS IoT Device SDK for Embedded C repository contains demos of using the HTTP client
library [here](https://github.com/aws/aws-iot-device-sdk-embedded-C/tree/main/demos/http)
on a POSIX platform. These can be used as reference examples for the library API.

## Generating documentation

The Doxygen references were created using Doxygen version 1.8.20. To generate the
Doxygen pages, please run the following command from the root of this repository:

```shell
doxygen docs/doxygen/config.doxyfile
```

## Contributing

See [CONTRIBUTING.md](./.github/CONTRIBUTING.md) for information on contributing.
