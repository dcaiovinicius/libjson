# libjson

A C++ library for simple and efficient JSON parsing.  
Provides a modern interface with `JsonValue` return type for easy error handling.

---

## Features

- Robust JSON parsing
- Easy-to-use interface: `Json::parse(const std::data&)`
- Support for static and shared library builds
- Easy integration via CMake and `find_package`

---

## Installation

### Prerequisites

- CMake >= 3.10
- C++ compiler with C++17 support

### Steps

```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DBUILD_SHARED_LIBS=ON
make
sudo make install
```

---

### How to use

```cpp
#include <stdio.h>
#include <json/json.h>

int main(void) {
    const char* input = "{\"name\": \"Alice\"}";
    auto output = Json::parse(input);

    std::cout << output["name"].asString() << "\n";
}
```