#include "../include/Utils.h"
#include "../include/CampusGraph.h"
#include "../include/HashTable.h"
#include "../include/DepartmentTree.h"
#include "../include/DataLoader.h"
#include <iostream>
#include <limits>

// Displays the main menu.
void displayMenu() {
    std::cout << "\n==========================================\n";
    std::cout << "  Smart Campus Navigation & Analytics (M1)\n";
    std::cout << "==========================================\n";
    std::cout << "1. View all facilities\n";
    std::cout << "2. Search facility by ID\n";
    std::cout << "3. View campus graph\n";
    std::cout << "4. Display graph counts and degrees\n";
    std::cout << "5. Find reachable buildings (BFS)\n";
    std::cout << "6. Add department\n";
    std::cout << "7. Search department\n";
    std::cout << "8. Display department hierarchy\n";
    std::cout << "9. Run preorder traversal\n";
    std::cout << "10. Exit\n";
    std::cout << "Enter your choice (1-10): ";
}

//uses default data folder unless another directory is provided
int main(int argc, char* argv[]) {
    std::string dataDir = "data/";
    if (argc > 1) {
        dataDir = argv[1];
    }

    // This part creates data structures  used to store in and organize campus information
    CampusGraph graph;
    HashTable hashTable(11);
    std::vector<Facility> facilityList;
    DepartmentTree deptTree;

    // Reads campus files and loads the information into data structures
    std::cout << "Loading campus data from directory: " << dataDir << "\n";
    DataLoader::loadAllData(dataDir, graph, hashTable, facilityList, deptTree);

    int choice = 0;
    // Shows the menu until user wants to exit
    while (true) {
        displayMenu();
        // Makes sure that the user enters a valid number 
        if (!(std::cin >> choice)) {
            std::cout << "Error: Invalid numeric input. Please enter a valid number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // ends the program when uses puts exit
        if (choice == 10 || choice == 0) {
            std::cout << "Exiting system. Goodbye!" << std::endl;
            break;
        }

        switch (choice) {

            //shows all facilities that showed successfully
            case 1: {
                if (facilityList.empty()) {
                    std::cout << "No facilities available." << std::endl;
                } else {
                    std::cout << "\n--- All Accepted Facilities (" << facilityList.size() << " total) ---" << std::endl;
                    for (const auto& f : facilityList) {
                        std::cout << f.id << " | " << f.name << " | " << f.buildingName << " | " << f.category << std::endl;
                    }
                }
                break;
            }
            // lets the user look up a facility by entering the id 
            case 2: {
                std::cout << "Enter Facility ID: ";
                std::string inputID;
                std::getline(std::cin, inputID);
                inputID = Utils::trim(inputID);

              //checks that the ID follows the required format before searching 
                if (!Utils::isValidFacilityID(inputID)) {
                    std::cout << "Invalid format: an ID is F followed by exactly three digits." << std::endl;
                } else {
                    std::string normID = Utils::toUpper(inputID);
                    Facility* found = hashTable.search(normID);
                    if (found) {
                        std::cout << "Found: " << found->id << " | " << found->name << " | "
                                  << found->buildingName << " | " << found->category << std::endl;
                    } else {
                        std::cout << "Not found: no facility with ID " << normID << std::endl;
                    }
                }
                break;
            }
            // displays the campus building and thier connections using an adjacency list
            case 3: {
                std::cout << "\n--- Campus Graph Adjacency List ---" << std::endl;
                graph.displayGraph();
                break;
            }
            //shows the total graph information, also including the degree if each vertex
            case 4: {
                std::cout << "\n--- Graph Counts & Degrees ---" << std::endl;
                graph.displayCountsAndDegrees();
                break;
            }

            // Uses bfs to find which buildings can be reached from a starting building 
            case 5: {
                std::cout << "Enter starting building: ";
                std::string startB;
                std::getline(std::cin, startB);
                startB = Utils::trim(startB);
                if (startB.empty()) {
                    std::cout << "Error: Building name cannot be empty." << std::endl;
                } else {
                    graph.findReachableBFS(startB);
                }
                break;
            }

            // Allows user to add a new department to the tree
            case 6: {
                if (deptTree.isEmpty()) {
                    std::cout << "Tree is empty. Enter root department name: ";
                    std::string rootName;
                    std::getline(std::cin, rootName);
                    rootName = Utils::trim(rootName);
                    std::string err;
                    if (!deptTree.addDepartment(rootName, "NONE", err)) {
                        std::cout << "Error: " << err << std::endl;
                    } else {
                        std::cout << "Root department '" << rootName << "' added successfully." << std::endl;
                    }
                } else {
                    std::cout << "Enter department name: ";
                    std::string deptName;
                    std::getline(std::cin, deptName);
                    deptName = Utils::trim(deptName);

                    std::cout << "Enter parent department name: ";
                    std::string parentName;
                    std::getline(std::cin, parentName);
                    parentName = Utils::trim(parentName);

                    std::string err;
                    if (!deptTree.addDepartment(deptName, parentName, err)) {
                        std::cout << "Error: " << err << std::endl;
                    } else { //user needs to enter new department and parent
                        std::cout << "Department '" << deptName << "' added successfully." << std::endl;
                    }
                }
                break;
            }
            //searches the department tree for a department with the entered name
            case 7: {
                std::cout << "Enter department name: ";
                std::string deptName;
                std::getline(std::cin, deptName);
                deptName = Utils::trim(deptName);
                if (deptName.empty()) {
                    std::cout << "Error: Department name cannot be empty." << std::endl;
                } else {
                    deptTree.searchDepartment(deptName);
                }
                break;
            }

            // Prints the department tree so the user can see how departments are organized
            case 8: {
                std::cout << "\n--- Department Hierarchy ---" << std::endl;
                deptTree.displayHierarchy();
                break;
            }
            // shows the department tree in preorder
            case 9: {
                std::cout << "\n--- Preorder Traversal Sequence ---" << std::endl;
                deptTree.displayPreorder();
                break;
            }
            // Handles the number that isnt one of the available menu options
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 10." << std::endl;
                break;
        }
    }

    return 0;
}
