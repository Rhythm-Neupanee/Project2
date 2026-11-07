/*
 * DataLoader.cpp
 * Purpose: Reads buildings.txt, connections.txt, facilities.txt and
 *          departments.txt. Each record is checked (field count, empty
 *          fields, ID format, unknown references, duplicates). Bad records
 *          print a warning with file, line number and reason, then loading
 *          continues.
 * Author:  Patrick (Member B)
 */

#include "../include/DataLoader.h"
#include "../include/Utils.h"
#include <fstream>
#include <iostream>

namespace {

// Splits a line on '|' and trims every field (keeps empty fields so the
// caller can count them)
std::vector<std::string> splitFields(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    for (char c : line) {
        if (c == '|') {
            fields.push_back(Utils::trim(current));
            current.clear();
        } else {
            current += c;
        }
    }
    fields.push_back(Utils::trim(current));
    return fields;
}

// True if any field is empty after trimming
bool hasEmptyField(const std::vector<std::string>& fields) {
    for (const std::string& f : fields) {
        if (f.empty()) return true;
    }
    return false;
}

// Strips a Windows '\r', trims, and reports whether the line should be skipped
// (blank or comment). The cleaned text is returned through 'line'.
bool isSkippable(std::string& line) {
    if (!line.empty() && line.back() == '\r') line.pop_back();
    line = Utils::trim(line);
    return line.empty() || line[0] == '#';
}

void warn(const std::string& file, int lineNum, const std::string& reason) {
    std::cout << "Warning [" << file << ":" << lineNum << "]: " << reason << std::endl;
}

void printSummary(const std::string& file, int accepted, int rejected) {
    std::cout << file << ": " << accepted << " accepted, " << rejected << " rejected";
}

// buildings.txt: one unique, nonempty name per line
void loadBuildings(const std::string& dir, CampusGraph& graph) {
    const std::string name = "buildings.txt";
    std::ifstream file(dir + name);
    int accepted = 0, rejected = 0, lineNum = 0;

    if (!file.is_open()) {
        std::cout << "Error: could not open " << dir + name << ". Treating it as empty." << std::endl;
    } else {
        std::string line;
        while (std::getline(file, line)) {
            lineNum++;
            if (isSkippable(line)) continue;

            if (line.find('|') != std::string::npos) {
                rejected++;
                warn(name, lineNum, "Wrong field count (expected one building name)");
            } else if (graph.addBuilding(line)) {
                accepted++;
            } else {
                rejected++;
                warn(name, lineNum, "Duplicate building '" + line + "'");
            }
        }
    }
    printSummary(name, accepted, rejected);
    std::cout << std::endl;
}

// connections.txt: BuildingA|BuildingB
void loadConnections(const std::string& dir, CampusGraph& graph) {
    const std::string name = "connections.txt";
    std::ifstream file(dir + name);
    int accepted = 0, rejected = 0, lineNum = 0;

    if (!file.is_open()) {
        std::cout << "Error: could not open " << dir + name << ". Treating it as empty." << std::endl;
    } else {
        std::string line;
        while (std::getline(file, line)) {
            lineNum++;
            if (isSkippable(line)) continue;

            std::vector<std::string> f = splitFields(line);
            std::string err;
            if (f.size() != 2) {
                rejected++;
                warn(name, lineNum, "Wrong field count (expected 2, found " +
                                    std::to_string(f.size()) + ")");
            } else if (hasEmptyField(f)) {
                rejected++;
                warn(name, lineNum, "Empty building name");
            } else if (graph.addConnection(f[0], f[1], err)) {
                accepted++;
            } else {
                rejected++;
                warn(name, lineNum, err);
            }
        }
    }
    printSummary(name, accepted, rejected);
    std::cout << std::endl;
}

// facilities.txt: FacilityID|FacilityName|BuildingName|Category
void loadFacilities(const std::string& dir, CampusGraph& graph, HashTable& hashTable,
                    std::vector<Facility>& facilityList) {
    const std::string name = "facilities.txt";
    std::ifstream file(dir + name);
    int accepted = 0, rejected = 0, lineNum = 0;

    if (!file.is_open()) {
        std::cout << "Error: could not open " << dir + name << ". Treating it as empty." << std::endl;
    } else {
        std::string line;
        while (std::getline(file, line)) {
            lineNum++;
            if (isSkippable(line)) continue;

            std::vector<std::string> f = splitFields(line);
            if (f.size() != 4) {
                rejected++;
                warn(name, lineNum, "Wrong field count (expected 4, found " +
                                    std::to_string(f.size()) + ")");
                continue;
            }
            if (hasEmptyField(f)) {
                rejected++;
                warn(name, lineNum, "Empty required field");
                continue;
            }

            // Uppercase first so f004 is accepted, then check the format
            std::string id = Utils::toUpper(f[0]);
            if (!Utils::isValidFacilityID(id)) {
                rejected++;
                warn(name, lineNum, "Invalid ID format '" + f[0] + "' (needs F plus 3 digits)");
                continue;
            }
            if (!graph.hasBuilding(f[2])) {
                rejected++;
                warn(name, lineNum, "Unknown building '" + f[2] + "'");
                continue;
            }

            Facility fac;
            fac.id = id;
            fac.name = f[1];
            fac.buildingName = graph.getBuildingSpelling(f[2]);
            fac.category = f[3];

            if (hashTable.insert(fac)) {
                facilityList.push_back(fac);
                accepted++;
            } else {
                rejected++;
                warn(name, lineNum, "Duplicate facility ID '" + id + "'");
            }
        }
    }
    printSummary(name, accepted, rejected);
    std::cout << " (hash capacity " << hashTable.getCapacity()
              << ", entries " << hashTable.getEntriesCount()
              << ", collisions " << hashTable.getCollisionsCount() << ")" << std::endl;
}

// departments.txt: DepartmentName|ParentName
void loadDepartments(const std::string& dir, DepartmentTree& deptTree) {
    const std::string name = "departments.txt";
    std::ifstream file(dir + name);
    int accepted = 0, rejected = 0, lineNum = 0;

    if (!file.is_open()) {
        std::cout << "Error: could not open " << dir + name << ". Treating it as empty." << std::endl;
    } else {
        std::string line;
        while (std::getline(file, line)) {
            lineNum++;
            if (isSkippable(line)) continue;

            std::vector<std::string> f = splitFields(line);
            std::string err;
            if (f.size() != 2) {
                rejected++;
                warn(name, lineNum, "Wrong field count (expected 2, found " +
                                    std::to_string(f.size()) + ")");
            } else if (hasEmptyField(f)) {
                rejected++;
                warn(name, lineNum, "Empty department or parent name");
            } else if (deptTree.addDepartment(f[0], f[1], err)) {
                accepted++;
            } else {
                rejected++;
                warn(name, lineNum, err);
            }
        }
    }
    printSummary(name, accepted, rejected);
    std::cout << std::endl;
}

} // namespace

// Loads the files in the required order: buildings, connections, facilities, departments
void DataLoader::loadAllData(
    const std::string& dataDir,
    CampusGraph& graph,
    HashTable& hashTable,
    std::vector<Facility>& facilityList,
    DepartmentTree& deptTree
) {
    std::string base = dataDir;
    if (!base.empty() && base.back() != '/' && base.back() != '\\') {
        base += '/';
    }

    loadBuildings(base, graph);
    loadConnections(base, graph);
    loadFacilities(base, graph, hashTable, facilityList);
    loadDepartments(base, deptTree);
}
