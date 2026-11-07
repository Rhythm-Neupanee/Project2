#include "../include/DepartmentTree.h"
#include "../include/Utils.h"
#include <iostream>

// Initializes an empty department tree.
DepartmentTree::DepartmentTree() : root(nullptr) {}

// Destructor that deletes the root node and its children.
DepartmentTree::~DepartmentTree() {
    delete root; // Dynamic node cleanup
}

//searches for a department by name.
DepartmentNode* DepartmentTree::findNode(DepartmentNode* current, const std::string& name) {
    if (!current) return nullptr;
    if (Utils::equalsIgnoreCase(current->name, name)) return current;
    for (DepartmentNode* child : current->children) {
        DepartmentNode* res = findNode(child, name);
        if (res) return res;
    }
    return nullptr; //not found.
}

// Calculates how far a node is from the root of the tree
int DepartmentTree::getDepth(const DepartmentNode* node) {
    int depth = 0;
    const DepartmentNode* curr = node;

    //moves up using pointer and counts each level.
    while (curr && curr->parent) {
        depth++;
        curr = curr->parent;
    }
    return depth;
}

// Adds a new department under the specified parent department.
bool DepartmentTree::addDepartment(const std::string& deptName, const std::string& parentName, std::string& errorMsg) {
    std::string cleanName = Utils::trim(deptName); //remove spaces.

    //make sure that department name is not empty.
    if (cleanName.empty()) {
        errorMsg = "Department name cannot be empty.";
        return false;
    }
    //prevent duplicate department name.
    if (findNode(root, cleanName) != nullptr) {
        errorMsg = "Duplicate department name: " + cleanName;
        return false;
    }

    //for empty tree, create the first department as root.
    if (root == nullptr) {
        root = new DepartmentNode(cleanName, nullptr);
        return true;
    }

    // find the parent department where the new department will be added.
    DepartmentNode* parentNode = findNode(root, parentName);

    // validate that specified patent exits.
    if (!parentNode) {
        errorMsg = "Unknown parent department: " + parentName;
        return false;
    }

    //create the new department as the child of the specified patents.
    DepartmentNode* childNode = new DepartmentNode(cleanName, parentNode);
    parentNode->children.push_back(childNode);
    return true;
}

// Searches for a department and displays its details.
void DepartmentTree::searchDepartment(const std::string& name) const {
    DepartmentNode* node = findNode(root, name); //using name to find the department.

    //validates if the department exits.
    if (!node) {
        std::cout << "department '" << name << "' not found." << std::endl;
        return;
    }

    // Display the department's information.
    std::cout << "department: " << node->name << std::endl;
    std::cout << "Parent: " << (node->parent ? node->parent->name : "NONE") << std::endl;
    std::cout << "Direct children: " << node->children.size() << std::endl;
    std::cout << "Depth: " << getDepth(node) << std::endl;
}

// prints the department hierarchy with indentation.
void DepartmentTree::printHierarchyRecursive(const DepartmentNode* current, int depth) {
    if (!current) return;
    // Indent each department according to its depth in the tree.
    for (int i = 0; i < depth; ++i) {
        std::cout << "    ";
    }
    std::cout << current->name << std::endl;       // print the current department name.

    //print children name in next depth level.
    for (DepartmentNode* child : current->children) {
        printHierarchyRecursive(child, depth + 1);
    }
}

// Displays the complete department hierarchy
void DepartmentTree::displayHierarchy() const {
    //validates for department in tree.
    if (!root) {
        std::cout << "No departments available." << std::endl;
        return;
    }
    printHierarchyRecursive(root, 0); //prints the hierarchy from root.
}

//displays departments using preorder traversal.
void DepartmentTree::printPreorderRecursive(const DepartmentNode* current, bool& first) {
    if (!current) return;
    if (!first) std::cout << " -> ";
    std::cout << current->name;
    first = false;

    for (DepartmentNode* child : current->children) {
        printPreorderRecursive(child, first);
    }
}

// Displays all departments in preorder traversal order.
void DepartmentTree::displayPreorder() const {
    if (!root) {
        std::cout << "No departments available." << std::endl;
        return;
    }
    bool first = true;
    printPreorderRecursive(root, first);
    std::cout << std::endl;
}
