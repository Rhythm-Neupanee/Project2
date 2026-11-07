#include "../include/Utils.h"
#include <algorithm>
#include <cctype>

namespace Utils {

    // removes whitespace and carriage return characters from both ends of a string.
    std::string trim(const std::string& str) {
        size_t start = 0;

        // Find the first character that is not whitespace.
        while (start < str.size() && (std::isspace(static_cast<unsigned char>(str[start])) || str[start] == '\r')) {
            start++;
        }
        // return an empty string if the input contains only whitespace.
        if (start == str.size()) return "";
        size_t end = str.size() - 1;

        // find the last character that is not whitespace.
        while (end > start && (std::isspace(static_cast<unsigned char>(str[end])) || str[end] == '\r')) {
            end--;
        }

        // returns string with leading and trailing whitespaces.
        return str.substr(start, end - start + 1);
    }

    // Converts all characters in a string to uppercase.
    std::string toUpper(const std::string& str) {
        std::string res = str;
        std::transform(res.begin(), res.end(), res.begin(), [](unsigned char c) {
            return std::toupper(c);
        });
        return res;
    }

    // Compares two strings without considering letter case or extra spaces.
    bool equalsIgnoreCase(const std::string& a, const std::string& b) {
        return toUpper(trim(a)) == toUpper(trim(b));
    }

    //validates if the facility id follows the required format.(F000)
    bool isValidFacilityID(const std::string& id) {
        std::string norm = toUpper(trim(id));   //removing extra spaces and converting it to uppercase.
        if (norm.length() != 4) return false;
        if (norm[0] != 'F') return false;
        return std::isdigit(norm[1]) && std::isdigit(norm[2]) && std::isdigit(norm[3]);
    }
}