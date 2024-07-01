# coreMQTT Client Library

This repository contains the coreMQTT library that has been optimized for a low memory footprint.  The coreMQTT library is compliant with the [MQTT 3.1.1](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/mqtt-v3.1.1.html) standard.  It has no dependencies on any additional libraries other than the standard C library, a customer-implemented network transport interface, and *optionally* a user-implemented platform time function.  This library is distributed under the [MIT Open Source License](LICENSE).

This library has gone through code quality checks including verification that no function has a [GNU Complexity](https://www.gnu.org/software/complexity/manual/complexity.html) score over 8, and checks against deviations from mandatory rules in the [MISRA coding standard](https://www.misra.org.uk/MISRAHome/MISRAC2012/tabid/196/Default.aspx).  Deviations from the MISRA C:2012 guidelines are documented under [MISRA Deviations](MISRA.md). This library has also undergone both static code analysis from [Coverity static analysis](https://scan.coverity.com/), and validation of memory safety and proof of functional correctness through the [CBMC automated reasoning tool](https://www.cprover.org/cbmc/).

## MQTT Config File

The MQTT client library exposes build configuration macros that are required for building the library.
A list of all the configurations and their default values are defined in [core_mqtt_config_defaults.h](https://github.com/FreeRTOS/coreMQTT/blob/master/source/include/core_mqtt_config_defaults.h). 
To provide custom values for the configuration macros, a custom config file named `core_mqtt_config.h` can be
provided by the application to the library.

By default, a `core_mqtt_config.h` custom config is required to build the library. To disable this requirement
and build the library with default configuration values, provide `MQTT_DO_NOT_USE_CUSTOM_CONFIG` as a compile time preprocessor macro.

**Thus, the MQTT library can be built by either**:
* Defining a `core_mqtt_config.h` file in the application, and adding it to the include directories list of the library  
**OR**
* Defining the `MQTT_DO_NOT_USE_CUSTOM_CONFIG` preprocessor macro for the library build.

## Building the Library

The [mqttFilePaths.cmake](https://github.com/FreeRTOS/coreMQTT/blob/master/mqttFilePaths.cmake) file contains the information of all source files and the header include path required to build the MQTT library.

As mentioned in the previous section, either a custom config file (i.e. `core_mqtt_config.h`) OR `MQTT_DO_NOT_USE_CUSTOM_CONFIG` macro needs to be provided to build the MQTT library.

For a CMake example of building the MQTT library with the `mqttFilePaths.cmake` file, refer to the `coverity_analysis` library target in [test/CMakeLists.txt](https://github.com/FreeRTOS/coreMQTT/blob/master/test/CMakeLists.txt) file.

## Building Unit Tests

### Checkout CMock Submodule
By default, the submodules in this repository are configured with `update=none` in [.gitmodules](.gitmodules) to avoid increasing clone time and disk space usage of other repositories (like [amazon-freertos](https://github.com/aws/amazon-freertos) that submodule this repository.

To build unit tests, the submodule dependency of CMock is required. Use the following command to clone the submodule:
```
git submodule update --checkout --init --recursive test/unit-test/CMock
```

### Platform Prerequisites

- For running unit tests
    - **C90 compiler** like gcc
    - **CMake 3.13.0 or later**
    - **Ruby 2.0.0 or later** is additionally required for the CMock test framework (that we use).
- For running the coverage target, **gcov** and **lcov** are additionally required.

### Steps to build **Unit Tests**

1. Go to the root directory of this repository. (Make sure that the **CMock** submodule is cloned as described [above](#checkout-cmock-submodule))

1. Run the *cmake* command: `cmake -S test -B build`

1. Run this command to build the library and unit tests: `make -C build all`

1. The generated test executables will be present in `build/bin/tests` folder.

1. Run `cd build && ctest` to execute all tests and view the test run summary.

## Reference examples

Please refer to the demos of the MQTT client library in the following locations for reference examples on POSIX and FreeRTOS platforms:

| Platform | Location | Transport Interface Implementation |
| :-: | :-: | :-: |
| POSIX | [AWS IoT Device SDK for Embedded C](https://github.com/aws/aws-iot-device-sdk-embedded-C/tree/master/demos/mqtt) | POSIX sockets for TCP/IP and OpenSSL for TLS stack
| FreeRTOS | [FreeRTOS/FreeRTOS](https://github.com/FreeRTOS/FreeRTOS/tree/master/FreeRTOS-Plus/Demo) | FreeRTOS+TCP for TCP/IP and mbedTLS for TLS stack |
| FreeRTOS | [FreeRTOS AWS Reference Integrations](https://github.com/aws/amazon-freertos/tree/master/demos/coreMQTT) | Based on Secure Sockets Abstraction |


## Generating documentation

The Doxygen references were created using Doxygen version 1.8.20. To generate the
Doxygen pages, please run the following command from the root of this repository:

```shell
doxygen docs/doxygen/config.doxyfile
```
