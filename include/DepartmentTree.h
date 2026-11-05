#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <string>
#include <utility>
#include <vector>

// Node for academic department general tree
struct DepartmentNode {
    std::string name;                       // name of the department.
    DepartmentNode* parent;                 // pointer to the parent department.
    std::vector<DepartmentNode*> children; // list of child department.

    // Initializes a department node with its name and parent.
    explicit DepartmentNode(std::string  deptName, DepartmentNode* parentPtr = nullptr)
        : name(std::move(deptName)), parent(parentPtr) {}

    // Destructor that recursively deletes all child nodes to prevent memory leaks when the tree is destroyed.
    ~DepartmentNode() {
        for (DepartmentNode* child : children) {
            delete child; // Recursive node cleanup
        }
    }
};

// General tree for academic departments
class DepartmentTree {
private:
    DepartmentNode* root;           // pointer to the root department.

    //searches the tree for a department with the given name.
    static DepartmentNode* findNode(DepartmentNode* current, const std::string& name);

    int getDepth(const DepartmentNode *node);   // find the depth of a node form the root.
    int getDepth(const DepartmentNode* node) const;

    // Recursively prints the department hierarchy with indentation based on each department's depth.
    static void printHierarchyRecursive(const DepartmentNode* current, int depth);

    // prints departments in preorder traversal.
    static void printPreorderRecursive(const DepartmentNode* current, bool& first);

public:
    DepartmentTree();  //create an empty department tree.
    ~DepartmentTree(); //destroys the tree.

    //Adds a new department under the specified parent department.
    //Returns true if successful, otherwise false and provides an error message.
    bool addDepartment(const std::string& deptName, const std::string& parentName, std::string& errorMsg);

    void searchDepartment(const std::string& name) const; //search and display the info of department by name.
    void displayHierarchy() const;       // display the complete department hierarchy.
    void displayPreorder() const;    // Displays all departments using preorder traversal.

    bool isEmpty() const {              //check whether the tree is empty.
        return root == nullptr;
    }
};

#endif