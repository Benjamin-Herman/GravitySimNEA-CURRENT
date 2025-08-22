#pragma once
//python style string found on internet dont remember where, just using now

#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace pystr {

    //split a string by a single character
    inline std::vector<std::string> split(const std::string& s, const std::vector<char>& delimiters) {
        std::vector<std::string> result;
        std::string current;

        auto isDelimiter = [&](char c) {
            for (char d : delimiters) {
                if (c == d) return true;
            }
            return false;
            };

        for (char c : s) {
            if (isDelimiter(c)) {
                if (!current.empty()) {
                    result.push_back(current);
                    current.clear();
                }
                if (c != ' ') {          // ignore space as a token
                    result.push_back(std::string(1, c));
                }
            }
            else {
                current += c;
            }
        }

        if (!current.empty())
            result.push_back(current);

        return result;
    }


    //join a vector of strings into a single string with a separator
    inline std::string join(const std::vector<std::string>& vec, const std::string& sep) {
        std::string result;
        for (size_t i = 0; i < vec.size(); ++i) {
            result += vec[i];
            if (i != vec.size() - 1)
                result += sep;
        }
        return result;
    }

    //trim leading and trailing whitespace
    inline std::string trim(const std::string& s) {
        auto start = std::find_if_not(s.begin(), s.end(), ::isspace);
        auto end = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();
        if (start >= end) return "";
        return std::string(start, end);
    }

}
