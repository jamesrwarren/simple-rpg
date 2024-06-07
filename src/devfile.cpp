#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

struct Node {
    int x, y; // Node position
    double cost; // Cost from the start node to this node
    double heuristic; // Heuristic (estimated cost to the goal)
    Node* parent; // Parent node in the path

    Node(int x, int y) : x(x), y(y), cost(0), heuristic(0), parent(nullptr) {}

    // Total cost (cost + h)
    double f() const {
        return cost + heuristic;
    }
};

// Calculate Manhattan distance heuristic
double calculateHeuristic(int x, int y, int targetX, int targetY) {
    return std::abs(x - targetX) + std::abs(y - targetY);
}

// A* pathfinding function
std::vector<Node*> findPath(std::vector<std::vector<Node>>& grid, int startX, int startY, int targetX, int targetY, std::vector<std::pair<int, int>> blockedSquares) {
    int numRows = grid.size();
    int numCols = grid[0].size();


    std::cout << "Grid Rows: " << numRows << "Grid Cols: " << numCols << " ---- ";

    // Check if the start and target positions are valid
    if (startX < 0 || startY < 0 || startX >= numCols || startY >= numRows ||
        targetX < 0 || targetY < 0 || targetX >= numCols || targetY >= numRows) {
        std::cout << "Positions not valid";
        return std::vector<Node*>();
    }

    struct NodeComparator {
        bool operator()(Node* a, Node* b) const {
            return a->f() > b->f();
        }
    };

    // Declare the priority queue using the comparator
    std::priority_queue<Node*, std::vector<Node*>, NodeComparator> openSet;

    // Initialize start node
    Node* startNode = &grid[startY][startX];
    startNode->cost = 0;
    startNode->heuristic = calculateHeuristic(startX, startY, targetX, targetY);
    
    // To avoid it selecting itself as first path
    startNode->parent = startNode;
    openSet.push(startNode);

    while (!openSet.empty()) {
        // Get the node with the lowest f value from the open set
        Node* currentNode = openSet.top();
        std::cout << "\n\nCurrent Nodes: " << currentNode->x << ", " << currentNode->y << " ---- \n";
        openSet.pop();

        // Check if we've reached the target node
        if (currentNode->x == targetX && currentNode->y == targetY) {
            // Reconstruct the path from the target node to the start node
            std::vector<Node*> path;
            Node* current = currentNode;
            std::cout << "LASTNODE: " << current->x << ", " << current->y << " " << current->parent->x << ", " << current->parent->y << " " << startNode->x << ", " << startNode->y << "\n";
            while (current != nullptr) {
                if (current->x == startNode->x && current->y == startNode->y) {
                    break;
                }
                std::cout << "CURRENT XY " << current->x << ", " << current->y << " " <<  startNode->x << ", " << startNode->y << "\n";
                path.push_back(current);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        double tentativeCost = currentNode->cost + 1; // Assuming each step has a cost of 1

        // Generate neighboring nodes. Path finding with diagonals
        int minX = std::max(0, currentNode->x - 1);
        int maxX = std::min(4, currentNode->x + 1);
        int minY = std::max(0, currentNode->y - 1);
        int maxY = std::min(4, currentNode->y + 1);

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                // Get rid of neighboring diagonals
                if (x != currentNode->x && y != currentNode->y) {
                    continue;
                }

                int nextX = x;
                int nextY = y;

                if (nextX >= 0 && nextX < numCols && nextY >= 0 && nextY < numRows) {
                    // carry on
                } else {
                    continue;
                }
                
                Node* neighbor = &grid[nextY][nextX];


                std::cout << "\nXY: " << nextX << ", " << nextY << " ---- tent G: " << tentativeCost << " neighbour G: " << neighbor->cost << "\n";

                // Check if (x, y) is in the vector
                bool found = false;
                for (const auto& coords : blockedSquares) {
                    if (coords.first == nextX && coords.second == nextY) {
                        found = true;
                        std::cout << "X: " << nextX << " Blocker X: " << coords.first << " Y: " << nextY << " Blocker Y: " << coords.second << " Found: " << found << "\n";
                        break; // No need to continue searching
                    }
                }

                // Check if the neighbor node is not in the closed set or has a lower g value
                if (tentativeCost < neighbor->cost || !neighbor->parent) {
                    if (!found) {       
                        neighbor->cost = tentativeCost;
                        neighbor->heuristic = calculateHeuristic(nextX, nextY, targetX, targetY);
                        neighbor->parent = currentNode;

                        // Add the neighbor to the open set
                        openSet.push(neighbor);
                    }
                }
            }
        }
    }

    // No path found
    return std::vector<Node*>();
}

int main() {
    // Create a grid (2D vector of nodes)
    const int numRows = 5;
    const int numCols = 5;
    std::vector<std::vector<Node>> grid(numRows, std::vector<Node>(numCols, Node(0, 0)));

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            grid[row][col].x = col;
            grid[row][col].y = row;
        }
    }
    
    // Example start and target positions
    int startX = 2;
    int startY = 4;
    int targetX = 2;
    int targetY = 0;

    std::vector<std::pair<int, int>> blockedSquares;
    blockedSquares.push_back(std::make_pair(2, 3));
    blockedSquares.push_back(std::make_pair(1, 3));
    blockedSquares.push_back(std::make_pair(3, 3));

    // Find the path
    std::vector<Node*> path = findPath(grid, startX, startY, targetX, targetY, blockedSquares);

    // Output the path
    if (!path.empty()) {
        std::cout << "Path found:" << std::endl;
        for (Node* node : path) {
            std::cout << "(" << node->x << ", " << node->y << ") ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No path found." << std::endl;
    }

    return 0;
}


// 0,4 1,4 2,4 3,4 4,4
// 0,3 1,3 2,3 3,3 4,3
// 0,2 1,2 2,2 3,2 4,2
// 0,1 1,1 2,1 3,1 4,1
// 0,0 1,0 2,0 3,0 4,0


// Current Nodes: 2, 4 ---- 
// Current Nodes: 1, 4 ---- 
// Current Nodes: 3, 4 ---- 
// Current Nodes: 0, 3 ---- 
// Current Nodes: 1, 2 ---- 
// Current Nodes: 2, 1 ---- 
// Current Nodes: 2, 0 ---- 