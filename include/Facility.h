#ifndef FACILITY_H
#define FACILITY_H

#include <string>

// Represents a campus facility record[cite: 3]
struct Facility {
    std::string id;           // Uppercase normalized ID (e.g., F001)[cite: 4]
    std::string name;         // Facility name[cite: 3]
    std::string buildingName; // Official building spelling from buildings.txt[cite: 4]
    std::string category;     // Facility category[cite: 3]
};

#endif // FACILITY_H
