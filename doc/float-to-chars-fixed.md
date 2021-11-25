# C++ 浮点数转为字符串

C++ 如何将浮点数转为字符串，并保留一定小数位数呢？比如有一个数 `25.56789`，只想保留小数点后两位。

`sprintf`、`snprintf` 是比较 C 语言的函数，用法也比较简单，本文不再过多介绍，接下来主要介绍一些 C++ 的写法。

先放参考链接：[c++ - How to use std::to_string function to format float as “x.0” - Stack Overflow](https://stackoverflow.com/questions/59817048/how-to-use-stdto-string-function-to-format-float-as-x-0)



## 1. std::stringstream

直接上代码：

```cpp
#include <sstream>

auto formatDobleValue(double val, int fixed) {
    std::ostringstream oss;
    oss << std::setprecision(fixed) << val;
    return oss.str();
}
```



## 2. std::to_string + 求子串

虽然 `std::to_string` 不支持指定格式化小数的位数，但是我们可以用求子串的方式得到我们想要的结果。

```cpp
#include <string>

auto formatDobleValue(double val, int fixed) {
    auto str = std::to_string(val);
    return str.substr(0, str.find(".") + fixed + 1);
}
```



## 3. std::to_chars (C++ 17)

[std::to_chars - cppreference.com](https://en.cppreference.com/w/cpp/utility/to_chars)

[Bartek's coding blog: How to Convert Numbers into Text with std::to_char in C++17 (bfilipek.com)](https://www.bfilipek.com/2019/11/tochars.html)

```cpp
#include <charconv>
#include <system_error>
#include <string>
#include <array>

auto formatDobleValue(double val, int fixed) {
    std::array<char, 10> str; // char str[10];
    auto [ptr, ec] = std::to_chars(str.data(), str.data() + str.size(), val, 
        std::chars_format::fixed, fixed);
    if (ec == std::errc()) {
        return std::string(str.data(), ptr - str.data());
    }
    else return std::string();
}

#include <iostream>

int main() {
    std::cout << formatDobleValue(0.12345, 3) << "\n";
}
```

但是很遗憾，截至目前，貌似只有 MSVC 支持除整型以外的重载。估计大家觉得，既然有了 `std::format`，`std::to_chars` 就显得比较鸡肋了……

[c++ - What is the correct way to call std::to_chars? - Stack Overflow](https://stackoverflow.com/questions/63963961/what-is-the-correct-way-to-call-stdto-chars)

## 4. std::format (C++ 20)

此外，还可以采用 C++ 20  的 `std::format`！

```cpp
#include <iostream>
#include <format>

int main() {
	std::cout << "Hello World!" << std::endl;
	std::cout << std::format("{:.3f}", 22.1234345) << std::endl;
}
```

是不是简单很多？最新的 Visual Studio Preview 已经支持了，快去试试吧！

需要指定 C++ 版本为 20，推荐用建议用 CMake ，方便多了。

```cmake
cmake_minimum_required (VERSION 3.8)
project ("CProgramming")
set(CMAKE_CXX_STANDARD 20)
add_executable(hello hello.cpp)
```

