#ifndef PROJ_CLEMENTINE_UTILS_H
#define PROJ_CLEMENTINE_UTILS_H

// https://stackoverflow.com/a/50916141/18472514

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <chrono>

#define _CRT_SECURE_NO_WARNINGS

inline std::string GetCurrentTimeForFileName()
{
    std::time_t time = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%F_%T");  // ISO 8601 date & time format
    auto s = ss.str();  // e.g. 2000-01-01_12:30:00
    std::replace(s.begin(), s.end(), ':', '-');
    return s;
}

#include <filesystem> // C++17
#include <fstream>
#include <string>

namespace fs = std::filesystem;

inline fs::path AppendTimeToFileName(const fs::path& fileName)
{
    return fileName.stem().string() + "_" + GetCurrentTimeForFileName() + fileName.extension().string();
}


#endif  // PROJ_CLEMENTINE_UTILS_H
