/*
 * CampusGraph.cpp
 * Purpose: Implementation of the undirected campus graph (adjacency list).
 *          Neighbors are never sorted so BFS order and shortest-path
 *          tie-breaking follow the order connections were accepted.
 * Author:  Patrick (Member B)
 */

#include "../include/CampusGraph.h"
#include "../include/Utils.h"
#include <algorithm>
#include <iostream>
#include <queue>

// Starts with an empty graph
CampusGraph::CampusGraph() : totalConnections(0) {}

// Linear search is fine here since the campus only has a handful of buildings
int CampusGraph::getBuildingIndex(const std::string& name) const {
    std::string target = Utils::trim(name);
    for (size_t i = 0; i < buildingNames.size(); ++i) {
        if (Utils::equalsIgnoreCase(buildingNames[i], target)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// True if the building was accepted earlier (ignores case)
bool CampusGraph::hasBuilding(const std::string& name) const {
    return getBuildingIndex(name) != -1;
}

// Returns the spelling from buildings.txt, or the input if it isn't found
std::string CampusGraph::getBuildingSpelling(const std::string& name) const {
    int idx = getBuildingIndex(name);
    return (idx != -1) ? buildingNames[idx] : name;
}

// Adds a new vertex; rejects empty names and case-insensitive duplicates
bool CampusGraph::addBuilding(const std::string& name) {
    std::string cleanName = Utils::trim(name);
    if (cleanName.empty() || hasBuilding(cleanName)) {
        return false;
    }
    buildingNames.push_back(cleanName);
    adjList.push_back(std::vector<int>());
    return true;
}

// Adds an undirected edge. On failure errorReason says why it was rejected.
bool CampusGraph::addConnection(const std::string& b1, const std::string& b2,
                                std::string& errorReason) {
    int idx1 = getBuildingIndex(b1);
    int idx2 = getBuildingIndex(b2);

    if (idx1 == -1) {
        errorReason = "Unknown building '" + b1 + "'";
        return false;
    }
    if (idx2 == -1) {
        errorReason = "Unknown building '" + b2 + "'";
        return false;
    }
    if (idx1 == idx2) {
        errorReason = "Self-connection not allowed (" + buildingNames[idx1] + ")";
        return false;
    }

    // Edges are stored at both ends, so checking one side also catches A|B then B|A
    if (std::find(adjList[idx1].begin(), adjList[idx1].end(), idx2) != adjList[idx1].end()) {
        errorReason = "Duplicate connection between " + buildingNames[idx1] +
                      " and " + buildingNames[idx2];
        return false;
    }

    adjList[idx1].push_back(idx2);
    adjList[idx2].push_back(idx1);
    totalConnections++;
    return true;
}

// Prints every building followed by its neighbors
void CampusGraph::displayGraph() const {
    if (buildingNames.empty()) {
        std::cout << "No campus buildings available." << std::endl;
        return;
    }
    for (size_t i = 0; i < buildingNames.size(); ++i) {
        std::cout << buildingNames[i] << " -> ";
        if (adjList[i].empty()) {
            std::cout << "(no connections)";
        } else {
            for (size_t j = 0; j < adjList[i].size(); ++j) {
                if (j > 0) std::cout << ", ";
                std::cout << buildingNames[adjList[i][j]];
            }
        }
        std::cout << std::endl;
    }
}

// Prints totals and the degree of every building
void CampusGraph::displayCountsAndDegrees() const {
    std::cout << "Total buildings: " << buildingNames.size() << std::endl;
    std::cout << "Total connections: " << totalConnections << std::endl;
    if (buildingNames.empty()) {
        std::cout << "No campus buildings available." << std::endl;
        return;
    }
    for (size_t i = 0; i < buildingNames.size(); ++i) {
        std::cout << "  " << buildingNames[i] << " (degree " << adjList[i].size() << ")" << std::endl;
    }
}

// BFS from startName; prints visit order with distances and a reachable count
void CampusGraph::findReachableBFS(const std::string& startName) const {
    int startIdx = getBuildingIndex(startName);
    if (startIdx == -1) {
        std::cout << "Error: unknown starting building '" << Utils::trim(startName) << "'." << std::endl;
        return;
    }

    std::vector<bool> visited(buildingNames.size(), false);
    std::vector<int> distance(buildingNames.size(), 0);
    std::queue<int> q;

    visited[startIdx] = true;   // mark when enqueued, not when dequeued
    q.push(startIdx);

    int reachableCount = 0;
    std::cout << "Reachable buildings from " << buildingNames[startIdx] << ":" << std::endl;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        reachableCount++;
        std::cout << "  " << buildingNames[u] << " (distance " << distance[u] << ")" << std::endl;

        for (int v : adjList[u]) {
            if (!visited[v]) {
                visited[v] = true;
                distance[v] = distance[u] + 1;
                q.push(v);
            }
        }
    }

    std::cout << "Reachable: " << reachableCount << " of " << buildingNames.size()
              << " buildings" << std::endl;
}

// BFS with parent tracking to print a path with the fewest connections
void CampusGraph::findShortestPath(const std::string& startName,
                                   const std::string& destName) const {
    int s = getBuildingIndex(startName);
    int d = getBuildingIndex(destName);

    if (s == -1) {
        std::cout << "Error: unknown start building '" << Utils::trim(startName) << "'." << std::endl;
        return;
    }
    if (d == -1) {
        std::cout << "Error: unknown destination building '" << Utils::trim(destName) << "'." << std::endl;
        return;
    }
    if (s == d) {
        std::cout << buildingNames[s] << std::endl;
        std::cout << "Number of connections: 0" << std::endl;
        return;
    }

    std::vector<bool> visited(buildingNames.size(), false);
    std::vector<int> parent(buildingNames.size(), -1);
    std::queue<int> q;

    visited[s] = true;
    q.push(s);

    bool found = false;
    while (!q.empty() && !found) {
        int u = q.front();
        q.pop();
        for (int v : adjList[u]) {
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
                if (v == d) {
                    found = true;
                    break;
                }
            }
        }
    }

    if (!found) {
        std::cout << "No path exists between " << buildingNames[s] << " and "
                  << buildingNames[d] << "." << std::endl;
        return;
    }

    // Walk back from the destination, then flip it around
    std::vector<int> path;
    for (int cur = d; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());

    for (int idx : path) {
        std::cout << buildingNames[idx] << std::endl;
    }
    std::cout << "Number of connections: " << path.size() - 1 << std::endl;
}

// Totals, highest degree (all ties), isolated buildings, connected components
void CampusGraph::displayConnectivityReport() const {
    if (buildingNames.empty()) {
        std::cout << "No campus buildings available." << std::endl;
        std::cout << "Total buildings: 0" << std::endl;
        std::cout << "Total connections: 0" << std::endl;
        return;
    }

    size_t maxDegree = 0;
    for (const auto& neighbors : adjList) {
        maxDegree = std::max(maxDegree, neighbors.size());
    }

    std::cout << "Total buildings: " << buildingNames.size() << std::endl;
    std::cout << "Total connections: " << totalConnections << std::endl;

    std::cout << "Highest degree (" << maxDegree << "): ";
    bool first = true;
    for (size_t i = 0; i < buildingNames.size(); ++i) {
        if (adjList[i].size() == maxDegree) {
            if (!first) std::cout << ", ";
            std::cout << buildingNames[i];
            first = false;
        }
    }
    std::cout << std::endl;

    std::cout << "Isolated buildings: ";
    first = true;
    for (size_t i = 0; i < buildingNames.size(); ++i) {
        if (adjList[i].empty()) {
            if (!first) std::cout << ", ";
            std::cout << buildingNames[i];
            first = false;
        }
    }
    if (first) std::cout << "None";
    std::cout << std::endl;

    // Count components by starting a fresh BFS at every unvisited building
    std::vector<bool> visited(buildingNames.size(), false);
    int components = 0;
    for (size_t start = 0; start < buildingNames.size(); ++start) {
        if (visited[start]) continue;
        components++;
        std::queue<int> q;
        visited[start] = true;
        q.push(static_cast<int>(start));
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adjList[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }
    std::cout << "Connected components: " << components << std::endl;
}
