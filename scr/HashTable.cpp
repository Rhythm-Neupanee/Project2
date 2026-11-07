#include "../include/HashTable.h"
#include "../include/Utils.h"
#include <iostream>

HashTable::HashTable(size_t cap)
    : capacity(cap), entriesCount(0), collisionsCount(0) {
    buckets.resize(capacity, nullptr);
}

HashTable::~HashTable() {
    for (size_t i = 0; i < capacity; ++i) {
        HashNode* current = buckets[i];
        while (current) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

// Polynomial rolling string hash[cite: 10]
size_t HashTable::hashFunction(const std::string& key) const {
    size_t hash = 0;
    for (char c : key) {
        hash = (hash * 31 + static_cast<unsigned char>(c)) % capacity;
    }
    return hash;
}

bool HashTable::contains(const std::string& id) const {
    std::string normID = Utils::toUpper(Utils::trim(id));
    size_t index = hashFunction(normID);
    HashNode* current = buckets[index];
    while (current) {
        if (current->facility.id == normID) {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool HashTable::insert(const Facility& facility) {
    std::string normID = Utils::toUpper(Utils::trim(facility.id));
    if (contains(normID)) {
        return false; // Reject duplicate key without incrementing collision count[cite: 10]
    }

    size_t index = hashFunction(normID);
    HashNode* newNode = new HashNode(facility);
    newNode->facility.id = normID;

    if (buckets[index] != nullptr) {
        collisionsCount++; // Collision occurs when inserting into non-empty bucket[cite: 10]
    }

    newNode->next = buckets[index];
    buckets[index] = newNode;
    entriesCount++;
    return true;
}

Facility* HashTable::search(const std::string& id) const {
    std::string normID = Utils::toUpper(Utils::trim(id));
    size_t index = hashFunction(normID);
    HashNode* current = buckets[index];
    while (current) {
        if (current->facility.id == normID) {
            return &(current->facility);
        }
        current = current->next;
    }
    return nullptr;
}
