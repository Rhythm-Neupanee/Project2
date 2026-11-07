#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Facility.h"
#include <string>
#include <vector>

// Node for separate chaining buckets[cite: 10]
struct HashNode {
    Facility facility;
    HashNode* next;

    HashNode(const Facility& f) : facility(f), next(nullptr) {}
};

// Custom Hash Table keyed by facility ID using Separate Chaining[cite: 1, 10]
class HashTable {
private:
    size_t capacity;
    size_t entriesCount;
    size_t collisionsCount;
    std::vector<HashNode*> buckets;

    // Custom polynomial string hash function[cite: 10]
    size_t hashFunction(const std::string& key) const;

public:
    explicit HashTable(size_t cap = 11);
    ~HashTable();

    // Inserts facility record; rejects duplicate IDs[cite: 9, 10]
    bool insert(const Facility& facility);

    // ID Lookup using hash table[cite: 9, 10]
    Facility* search(const std::string& id) const;

    // Case-insensitive duplicate check[cite: 10]
    bool contains(const std::string& id) const;

    // Diagnostics[cite: 10]
    size_t getCapacity() const { return capacity; }
    size_t getEntriesCount() const { return entriesCount; }
    size_t getCollisionsCount() const { return collisionsCount; }
};

#endif // HASHTABLE_H
HashTable.h
