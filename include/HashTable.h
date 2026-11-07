#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Facility.h"
#include <string>
#include <vector>

// The node for separate chaining buckets
struct HashNode {
    Facility facility;
    HashNode* next;

    HashNode(const Facility& f) : facility(f), next(nullptr) {}
};

// Made the custom hash table keyed by facility ID using Separate Chaining
class HashTable {
private:
    size_t capacity;
    size_t entriesCount;
    size_t collisionsCount;
    std::vector<HashNode*> buckets;

    // Custom polynomial string hash function
    size_t hashFunction(const std::string& key) const;

public:
    explicit HashTable(size_t cap = 11);
    ~HashTable();

    // This inserts facility record and rejects duplicate IDs
    bool insert(const Facility& facility);

    // The ID lookup using hash table
    Facility* search(const std::string& id) const;

    // Case-insensitive duplicate check
    bool contains(const std::string& id) const;

    // Diagnostics
    size_t getCapacity() const { return capacity; }
    size_t getEntriesCount() const { return entriesCount; }
    size_t getCollisionsCount() const { return collisionsCount; }
};

#endif // HASHTABLE_H
