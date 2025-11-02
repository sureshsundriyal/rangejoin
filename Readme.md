# RangeJoin — C++ Range-Based String Join Utility

This header-only utility provides a set of `join` functions for concatenating
elements of ranges into a single `std::string`. It supports multiple ranges and
transformation functions C++20 features like concepts and ranges.

## Features

- Join elements of a single range into a string.
- Join multiple ranges into a single string.
- Apply a transformation to elements before joining.
- Supports any range whose elements are convertible to `std::string`.

## Dependencies

- C++20 (for concepts and ranges)

## Usage

### 1. Join a Single Range

```cpp
std::vector<std::string> words = {"hello", "world"};
std::string result = rangejoin::join(", ", words); // "hello, world"
```

### 2. Join Multiple Ranges

```cpp
std::list<std::string> list1 = {"foo", "bar"};
std::set<std::string> set1 = {"baz", "qux"};
std::string result = rangejoin::join(" | ", list1, set1); // "foo | bar | baz | qux"
```

### 3. Join with Transformation

```cpp
std::vector<int> nums = {1, 2, 3};
std::string result = rangejoin::join("-", [](int n) { return std::to_string(n); }, nums); // "1-2-3"
```

## Example

```cpp
#include <vector>
#include <string>
#include <iostream>

#include "rangejoin.hpp"

int main() {
    std::vector<std::string> names = {"Alice", "Bob", "Charlie"};
    std::cout << rangejoin::join(", ", names) << std::endl;

    std::vector<int> scores = {85, 90, 95};
    std::cout << rangejoin::join(" - ", [](int score) { return std::to_string(score); }, scores) << std::endl;
}
```

## License

Creative Commons Zero v1.0 Universal