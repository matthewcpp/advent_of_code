#pragma once

#include <string>
#include <string_view>
#include <vector>

template <typename StrType>
std::vector<StrType> split_str(const StrType& str, const std::string& delimiter)
{
    std::vector<StrType> results;

    typename StrType::size_type start = 0;
    typename StrType::size_type pos = str.find(delimiter.data(), start, delimiter.size());

    while (pos != StrType::npos) {
        auto result_len = pos - start;
        results.emplace_back(str.substr(start, result_len));
        start += result_len + delimiter.length();
        pos = str.find(delimiter.data(), start, delimiter.size());
    }

    results.emplace_back(str.substr(start));

    return results;
}

template <typename StrType>
std::vector<StrType> split_str(const StrType& str, char delimiter)
{
    return split_str(str, std::string(1, delimiter));
}

template <typename StrType>
std::vector<StrType> split_str_any(const StrType& str, const std::string& delimiter)
{
    std::vector<StrType> results;

    typename StrType::size_type start = 0;
    typename StrType::size_type pos = str.find_first_of(delimiter.data(), start, delimiter.size());

    while (pos != StrType::npos) {
        auto result_len = pos - start;
        results.emplace_back(str.substr(start, result_len));
        start += result_len + delimiter.length();
        pos = str.find_first_of(delimiter.data(), start, delimiter.size());
    }

    results.emplace_back(str.substr(start));

    return results;
}
