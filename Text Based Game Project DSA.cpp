#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <conio.h>
#include <thread>
#include <chrono>
#define CLEAR_SCREEN "cls"

using namespace std;

const int MAX_INT = 2147483647;

void typewriterOutput(const string& message, int delayMs = 20) {
    static bool skipTypewriter = false;
    if (skipTypewriter) {
        cout << message << endl;
        return;
    }

    for (char ch : message) {
        cout << ch << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
    this_thread::sleep_for(chrono::milliseconds(888));
    cout << endl;
}

void waitForKeyPress(const string& message) {
    typewriterOutput(message);
    _getch();
}

struct Edge { // Graph structure with weighted edges
    int to, weight;
};

struct Node {
    int id;
    vector<Edge> edges;
};

class Graph {
  private:
    vector<Node> nodes;

  public:
    Graph(int size) {
        nodes.resize(size + 1);
        for (int i = 1; i <= size; ++i) {
            nodes[i].id = i;
        }
    }

    void addEdge(int from, int to, int weight) {
        nodes[from].edges.push_back({to, weight});
    }

    vector<Edge> getEdges(int nodeId) {
        return nodes[nodeId].edges;
    }

    bool hasPathTo(int nodeId) {
        for (const auto& node : nodes) {
            for (const auto& edge : node.edges) {
                if (edge.to == nodeId) return true;
            }
        }
        return false;
    }
};

struct PriorityQueue { // Custom priority queue for Dijkstra-like algorithm
    vector<pair<int, int>> elements; // {distance, node}

    void push(pair<int, int> element) {
        elements.push_back(element);
        int i = elements.size() - 1;
        while (i > 0 && elements[i].first < elements[(i - 1) / 2].first) {
            swap(elements[i], elements[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    pair<int, int> pop() {
        pair<int, int> top = elements[0];
        elements[0] = elements.back();
        elements.pop_back();

        int i = 0;
        while (true) {
            int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
            if (left < elements.size() && elements[left].first < elements[smallest].first) {
                smallest = left;
            }
            if (right < elements.size() && elements[right].first < elements[smallest].first) {
                smallest = right;
            }
            if (smallest == i) break;
            swap(elements[i], elements[smallest]);
            i = smallest;
        }
        return top;
    }

    bool empty() {
        return elements.empty();
    }
};

void generateRandomGraph(Graph& graph, int numNodes, int numEdges) {
    srand(time(0));
    for (int i = 0; i < numEdges; ++i) {
        int from = rand() % numNodes + 1;
        int to = rand() % numNodes + 1;
        int weight = rand() % 20 + 1; // Weights between 1 and 20
        if (from != to) {
            graph.addEdge(from, to, weight);
        }
    }

    if (graph.getEdges(1).empty()) { // Ensure Node 1 has at least one connection
        int to = rand() % numNodes + 2; // Avoid self-loop
        graph.addEdge(1, to, rand() % 20 + 1);
    }

    if (!graph.hasPathTo(numNodes)) { // Ensure at least one path reaches the final node
        int from = rand() % (numNodes - 1) + 1;
        graph.addEdge(from, numNodes, rand() % 20 + 1);
    }
}

void randomEvent(int& health, map<string, int>& inventory) {
    int eventType = rand() % 3; // Random event type
    switch (eventType) {
        case 0:
            typewriterOutput("You found a health potion! (+20 health)");
            inventory["Health Potion"]++;
            break;
        case 1:
            typewriterOutput("You triggered a trap! (-15 health)");
            health -= 15;
            break;
        case 2:
            typewriterOutput("You encounter a wise sage who offers advice. No effect on health.");
            break;
    }
    if (health > 100) health = 100;
}

void useItem(map<string, int>& inventory, int& health) {
    if (inventory.empty()) {
        typewriterOutput("Your inventory is empty! Nothing to use.");
        return;
    }

    typewriterOutput("Inventory:");
    int index = 1;
    for (const auto& item : inventory) {
        typewriterOutput(to_string(index) + ". " + item.first + " (x" + to_string(item.second) + ")");
        index++;
    }

    typewriterOutput("Enter the number of the item to use (or 0 to cancel): ", 0);
    int choice;
    cin >> choice;

    if (choice == 0) {
        typewriterOutput("You chose not to use any item.");
        return;
    }

    if (choice < 1 || choice > inventory.size()) {
        typewriterOutput("Invalid choice. Try again.");
        return;
    }

    auto it = inventory.begin();
    advance(it, choice - 1);

    if (it->first == "Health Potion") {
        health += 20;
        if (health > 100) health = 100;
        typewriterOutput("You used a Health Potion. (+20 health)");
    }

    if (--it->second == 0) {
        inventory.erase(it);
    }
}

int main() {
    int numNodes = 10;
    int numEdges = 20;

    Graph gameWorld(numNodes);
    generateRandomGraph(gameWorld, numNodes, numEdges);

    this_thread::sleep_for(chrono::milliseconds(888));
    system(CLEAR_SCREEN);
    typewriterOutput("Welcome to the Adventure of the Sacred Artifact!\n\n");
    typewriterOutput("STORY:\n");
    typewriterOutput("Long ago, a sacred artifact was stolen from the Temple of Eternity. The kingdom is in peril, and only you, the chosen explorer, can return it to its rightful place.\n");
    typewriterOutput("Your journey will take you through treacherous terrains, dark forests, and perilous caverns. Every step you take will drain your strength, but you must persevere to save the kingdom.\n\n");

    typewriterOutput("GOAL:\n");
    typewriterOutput("Reach the Temple of Eternity (Node " + to_string(numNodes) + ") and restore the artifact, while managing your health points.\n\n");

    typewriterOutput("MECHANICS:\n");
    typewriterOutput("1. You start at Node 1 with 100 health points.\n");
    typewriterOutput("2. Each path between nodes has a weight representing the health cost to travel.\n");
    typewriterOutput("3. Choose a valid path to move to the next area.\n");
    typewriterOutput("4. If your health drops to 0 or below, you lose the game.\n");
    typewriterOutput("5. Successfully reach Node " + to_string(numNodes) + " to win!\n\n");

    waitForKeyPress("Press any key to start the game...");
    system(CLEAR_SCREEN);

    int start = 1;
    int playerHealth = 100;
    int currentNode = start;
    map<string, int> inventory;

    while (true) {
        typewriterOutput("You are at node " + to_string(currentNode) + ". Your health: " + to_string(playerHealth) + "\n");
        typewriterOutput("Connections from here: \n");

        auto edges = gameWorld.getEdges(currentNode);
        if (edges.empty()) {
            typewriterOutput("No more paths. You are trapped!\n");
            break;
        }

        for (size_t i = 0; i < edges.size(); ++i) {
            typewriterOutput(to_string(i + 1) + ". Node " + to_string(edges[i].to) + " (Cost: " + to_string(edges[i].weight) + ")\n");
        }

        typewriterOutput("Choose a path by entering the number (or 0 to quit, -1 to use an item): ", 5);
        int choice;
        if (!(cin >> choice)) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            typewriterOutput("Invalid input detected!");

            // Trigger a random health deduction
            if (rand() % 2 == 0) { // 50% chance to reduce health
                typewriterOutput("You feel a strange force draining your energy. (-5 health)");
                playerHealth -= 5;
            }

            if (playerHealth <= 0) {
                typewriterOutput("You ran out of health and lost the game. The artifact remains lost.\n");
                waitForKeyPress("Press any key to exit the game...");
                break;
            }
            continue;
        }

        if (choice == 0) {
            typewriterOutput("Game over. You quit the adventure!\n");
            waitForKeyPress("Press any key to exit the game...");
            break;
        }

        if (choice == -1) {
            useItem(inventory, playerHealth);
            continue;
        }

        if (choice < 1 || choice > edges.size()) {
            typewriterOutput("Invalid choice. Try again.\n");
            continue;
        }

        Edge chosenEdge = edges[choice - 1];
        currentNode = chosenEdge.to;
        playerHealth -= chosenEdge.weight;

        system(CLEAR_SCREEN);
        typewriterOutput("\nAs you traverse the path, you find yourself in a new area:\n");
        switch (currentNode) {
            case 2:
                typewriterOutput("A dense jungle with the sound of mysterious creatures all around.\n");
                break;
            case 3:
                typewriterOutput("A dark cavern echoing with the sound of dripping water.\n");
                break;
            case 4:
                typewriterOutput("A windswept cliff overlooking a vast ocean.\n");
                break;
            case 5:
                typewriterOutput("An abandoned village where shadows seem to move on their own.\n");
                break;
            case 6:
                typewriterOutput("A fiery volcanic path with molten lava flows nearby.\n");
                break;
            case 7:
                typewriterOutput("A serene meadow offering a moment of respite.\n");
                break;
            case 8:
                typewriterOutput("An ancient battlefield strewn with rusted weapons.\n");
                break;
            case 9:
                typewriterOutput("The outskirts of the Temple of Eternity, shrouded in mist.\n");
                break;
            case 10:
                typewriterOutput("The Temple of Eternity! You can feel the artifact resonating with the surroundings.\n");
                break;
            default:
                typewriterOutput("An unknown and eerie location.\n");
        }

        randomEvent(playerHealth, inventory);

        if (playerHealth <= 0) {
            typewriterOutput("You ran out of health and lost the game. The artifact remains lost.\n");
            waitForKeyPress("Press any key to exit the game...");
            break;
        }

        if (currentNode == numNodes) {
            typewriterOutput("Congratulations! You reached the Temple of Eternity and restored the artifact. The kingdom is saved!\n");
            waitForKeyPress("Press any key to celebrate your victory...");
            break;
        }
    }

    return 0;
}
