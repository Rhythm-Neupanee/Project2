/*
 * CampusGraph.h
 * Purpose: Undirected, unweighted graph of campus buildings stored as an
 *          adjacency list. Supports display, degree counts, BFS reachability,
 *          shortest path, and the connectivity report.
 * Author:  Patrick (Member B)
 */

#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <string>
#include <vector>

class CampusGraph {
private:
    std::vector<std::string> buildingNames;  // buildings.txt order
    std::vector<std::vector<int>> adjList;   // adjList[i] = neighbor indices, insertion order
    size_t totalConnections;                 // each undirected edge counted once

    // Returns index of a building (case-insensitive, trimmed) or -1 if unknown
    int getBuildingIndex(const std::string& name) const;

public:
    CampusGraph();

    // Loading
    bool addBuilding(const std::string& name);
    bool addConnection(const std::string& b1, const std::string& b2, std::string& errorReason);

    // Lookups used by the loader and menu
    bool hasBuilding(const std::string& name) const;
    std::string getBuildingSpelling(const std::string& name) const;

    // Milestone 1
    void displayGraph() const;
    void displayCountsAndDegrees() const;
    void findReachableBFS(const std::string& startName) const;

    // Final
    void findShortestPath(const std::string& startName, const std::string& destName) const;
    void displayConnectivityReport() const;

    size_t getBuildingCount() const { return buildingNames.size(); }
    size_t getConnectionCount() const { return totalConnections; }
};

#endif // CAMPUSGRAPH_H
