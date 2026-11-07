#include "../include/HashTable.h"
#include "../include/Utils.h"
#include <iostream>

//this initializes the hash table with the given capacity
HashTable::HashTable(size_t cap)
    : capacity(cap), entriesCount(0), collisionsCount(0) {
    buckets.resize(capacity, nullptr);
}

// goes through the buckets and deletes nodes to free up memory
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

// uses the characters in the ID to cal culate which bucket it goes in
size_t HashTable::hashFunction(const std::string& key) const {
    size_t hash = 0;
    for (char c : key) {
        hash = (hash * 31 + static_cast<unsigned char>(c)) % capacity;
    }
    return hash;
}
// chechks if a facility with the ID is already in the table
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

// this adds a new facility to the hash table 
bool HashTable::insert(const Facility& facility) {
    std::string normID = Utils::toUpper(Utils::trim(facility.id));
    if (contains(normID)) {
        return false; 
    }

    size_t index = hashFunction(normID);
    HashNode* newNode = new HashNode(facility);
    newNode->facility.id = normID;

    // keeps track if theres any collisions when I bucket already has something in it
    if (buckets[index] != nullptr) {
        collisionsCount++; 
    }

    //This adds a new facility to the front of the linked list
    newNode->next = buckets[index];
    buckets[index] = newNode;
    entriesCount++;
    return true;
}

// this searches for the facility using its id
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
    // returns nullptr if no matching facility was found
    return nullptr;
}
