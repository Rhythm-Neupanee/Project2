# Smart Campus Navigation and Facility Analytics System

## Project Overview
This application is a C++ menu driven interactive program which will help maintain campus facilities, study connectivity of buildings on campus through the concept of graphs, find reachability by performing Breadth First Search (BFS) and navigate the hierarchical structure of academic departments through a general tree. The application takes input from specially formatted text files and includes a custom hash table for string keys for quick search of facilities, undirected graph and a general tree.

## Folder Structure
```text
Project2/
│
├── include/                 # Header files (.h)
│   ├── Utils.h              # Formatting, trimming, and validation utilities
│   ├── Facility.h           # Facility record structure
│   ├── HashTable.h          # Custom ID Hash Table with Separate Chaining
│   ├── CampusGraph.h        # Undirected Campus Graph & BFS reachability
│   ├── DepartmentTree.h     # General Department Tree hierarchy
│   └── DataLoader.h         # File loading and parsing engine
│
├── src/                     # Source files (.cpp)
│   ├── main.cpp             # Entry point & interactive CLI menu
│   ├── Utils.cpp
│   ├── HashTable.cpp
│   ├── CampusGraph.cpp
│   ├── DepartmentTree.cpp
│   └── DataLoader.cpp
│
├── data/                    # Default data directory
│   ├── buildings.txt        # Campus building names
│   ├── connections.txt      # Walkway connections between buildings
│   ├── facilities.txt       # Facility records and categories
│   └── departments.txt      # Academic department hierarchy
│
├── testdata/                # Optional custom test datasets
├── Makefile                 # Build automation script
├── design_document.txt      # Technical design and data structure choices
├── CONTRIBUTIONS.txt        # Team member contribution breakdown
├── test_results.txt         # Executed test log and validation results
├── .gitignore               # Executable and build file exclusions
└── README.md                # System overview and build instructions
```

## Data Structures Used
* **Custom String Hash Table**: We created our own hash table that stores facilities by their uppercase ID (e.g., F001). The hashing function uses a polynomial rolling string algorithm with separate chaining and linked lists for collisions.
* **Custom Undirected Graph**: Implemented through adjacency list in std::vector<std::vector<int>>, in which campus buildings are vertices and walkways form unweighted edges. This structure maintains the original sequence in which connections are loaded from the file in order to traverse neighbors in that order.
* **Breadth-First Search (BFS)**: Uses regular std::queue along with bool array that stores vertices’ visited status in order to search for all reachable buildings from some starting vertex. All vertices are marked visited right after enqueuing them for correct order and distances.
* **Custom General Tree**: Used in case of academic departments tree. Every DepartmentNode includes a parent pointer and a dynamic std::vector<DepartmentNode*> of children pointers in order to enable each department having an unlimited amount of sub-departments.
* **std::vector**: Regular vector container used throughout the program for managing adjacency list of graph, pointers of children in trees and the final list of accepted facilities records.

## Execution Instructions
1. Open your terminal and navigate to the project root directory:
   ```bash
   cd Project2
   ```

2. Build and run the executable automatically using `make`:
   ```bash
   make
   ```
   *(Compiles all source files in `scr/` into `app` and automatically launches the executable)*


3. Manual compilation (optional alternative):
   ```bash
   g++ -std=c++11 -Iinclude scr/*.cpp -o app
   ./app
   ```

4. Run the executable with an optional custom data directory:
   ```bash
   ./app testdata/
   ```
   *(Or on Windows: `.\app.exe testdata/`)*


5. Clean build files:
   ```bash
   make clean
   ```

## Input File Format
The application will load information about the campus from four different text files that reside in the designated data folder. Any comments (# symbols), empty lines, and Windows-style line breaks (\r) will be automatically removed.
- **`buildings.txt`** (Building names):
  ```text
  # BuildingName
  Library
  Student Union
  Science Building
  Engineering Building
  Business Building
  Gym
  Observatory
  ```

- **`connections.txt`** (Undirected walkway connections):
  ```text
  # BuildingA|BuildingB
  Library|Student Union
  Library|Business Building
  Student Union|Science Building
  Student Union|Gym
  Science Building|Engineering Building
  Business Building|Gym
  ```

- **`facilities.txt`** (Facility ID, Name, Building, Category):
  ```text
  # FacilityID|FacilityName|BuildingName|Category
  F001|Main Reading Room|Library|Library
  F002|Tutoring Center|Library|Tutoring Center
  F003|Student Services Office|Student Union|Student Services Office
  F004|Computer Lab A|Science Building|Computer Lab
  F005|Room 101|Engineering Building|Classroom
  ```

- **`departments.txt`** (Department Name, Parent Name):
  ```text
  # DepartmentName|ParentName
  University|NONE
  College of Engineering|University
  College of Science|University
  Computer Science|College of Engineering
  Electrical Engineering|College of Engineering
  ```
