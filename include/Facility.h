#ifndef FACILITY_H
#define FACILITY_H

#include <string>

// It represents the campus facility record
struct Facility {
    std::string id;           // Uppercase is normalized ID
    std::string name;         // The facility name
    std::string buildingName; // Official building spelling from buildings.txt
    std::string category;     // The facility category
};

#endif // FACILITY_H
