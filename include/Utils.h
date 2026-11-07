#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    // Trims whitespace, tabs, and carriage returns (\r)
    std::string trim(const std::string& str);

    // Converts ASCII string to uppercase
    std::string toUpper(const std::string& str);

    // Case-insensitive string comparison
    bool equalsIgnoreCase(const std::string& a, const std::string& b);

    // Validates Facility ID format: 'F' followed by 3 digits
    bool isValidFacilityID(const std::string& id);
}

#endif // UTILS_H