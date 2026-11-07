/*
 * DataLoader.h
 * Purpose: Loads the four campus data files (buildings, connections,
 *          facilities, departments) in the required order, validating each
 *          record and printing warnings and load summaries.
 * Author:  Patrick (Member B)
 */

#ifndef DATALOADER_H
#define DATALOADER_H

#include "CampusGraph.h"
#include "HashTable.h"
#include "DepartmentTree.h"
#include "Facility.h"
#include <string>
#include <vector>

class DataLoader {
public:
    // Loads all four files from dataDir into the given structures
    static void loadAllData(
        const std::string& dataDir,
        CampusGraph& graph,
        HashTable& hashTable,
        std::vector<Facility>& facilityList,
        DepartmentTree& deptTree
    );
};

#endif // DATALOADER_H
