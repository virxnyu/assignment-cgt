#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

class Graph {
    int V;  // Number of vertices
    list<int> *adj;  // Adjacency list

public:
    Graph(int V);  // Constructor
    void addEdge(int u, int v);  // Function to add an edge
    void removeEdge(int u, int v);  // Function to remove an edge
    void printEulerTour();  // Function to start Fleury's algorithm
    void fleury(int start);  // Recursive function to implement Fleury's algorithm
    bool isEulerian();  // Function to check if the graph has Eulerian path/circuit
    int DFSCount(int v, vector<bool> &visited);  // Helper for checking graph connectivity
    bool isValidNextEdge(int u, int v);  // Check if edge u-v is valid for Euler tour
};

// Constructor
Graph::Graph(int V) {
    this->V = V;
    adj = new list<int>[V];
}

// Function to add an edge
void Graph::addEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

// Function to remove an edge
void Graph::removeEdge(int u, int v) {
    adj[u].remove(v);
    adj[v].remove(u);
}

// DFS utility to count reachable vertices
int Graph::DFSCount(int v, vector<bool> &visited) {
    visited[v] = true;
    int count = 1;

    // Replace the range-based for loop with an iterator-based loop
    for (list<int>::iterator it = adj[v].begin(); it != adj[v].end(); ++it) {
        int u = *it;  // Get the adjacent vertex
        if (!visited[u]) {
            count += DFSCount(u, visited);
        }
    }

    return count;
}


// Function to check if edge u-v is a valid next edge in Euler tour
bool Graph::isValidNextEdge(int u, int v) {
    // If v is the only adjacent vertex of u
    if (adj[u].size() == 1) {
        return true;
    }

    // Count vertices reachable from u
    vector<bool> visited(V, false);
    int count1 = DFSCount(u, visited);

    // Remove edge u-v and count reachable vertices from u
    removeEdge(u, v);
    fill(visited.begin(), visited.end(), false);
    int count2 = DFSCount(u, visited);

    // Add the edge back to the graph
    addEdge(u, v);

    // If the number of reachable vertices is the same, the edge is not a bridge
    return (count1 == count2);
}

// Function to implement Fleury's algorithm
void Graph::fleury(int start) {
    // Iterate through all adjacent vertices
    for (list<int>::iterator it = adj[start].begin(); it != adj[start].end(); ++it) {
    int v = *it;  // Get the adjacent vertex

    // Check if this edge can be included in Eulerian tour
    if (isValidNextEdge(start, v)) {
        cout << start << "-" << v << " ";  // Print the edge
        removeEdge(start, v);             // Remove the edge from the graph
        fleury(v);                        // Recur for the next vertex
        break;                            // Break after finding a valid edge
    }
}

}

// Function to start Fleury's algorithm
void Graph::printEulerTour() {
    int u = 0;

    // Find a vertex with odd degree, if any
    for (int i = 0; i < V; i++) {
        if (adj[i].size() % 2 != 0) {
            u = i;
            break;
        }
    }

    // Print Eulerian tour starting from vertex u
    fleury(u);
    cout << endl;
}

// Function to check if the graph has an Eulerian Circuit or Path
bool Graph::isEulerian() {
    vector<bool> visited(V, false);

    // Find a vertex with a non-zero degree
    int i;
    for (i = 0; i < V; i++) {
        if (adj[i].size() != 0) {
            break;
        }
    }

    if (i == V) {
        return true;  // All vertices have zero degree, so Eulerian
    }

    // Start DFS from a vertex with non-zero degree
    DFSCount(i, visited);

    // Check if all vertices with non-zero degree are visited
    for (int i = 0; i < V; i++) {
        if (adj[i].size() != 0 && visited[i] == false) {
            return false;  // Graph is not connected
        }
    }

    // Count vertices with odd degree
    int odd = 0;
    for (int i = 0; i < V; i++) {
        if (adj[i].size() % 2 != 0) {
            odd++;
        }
    }

    // If count is more than 2, the graph is not Eulerian
    return (odd == 0 || odd == 2);
}

// Main function
int main() {
    int V, E;
    cout << "Enter the number of vertices: ";
    cin >> V;

    Graph g(V);

    cout << "Enter the number of edges: ";
    cin >> E;

    cout << "Enter the edges (u v) pairs:\n";
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    if (g.isEulerian()) {
        cout << "The graph has an Eulerian path or circuit.\n";
        g.printEulerTour();
    } else {
        cout << "The graph is not Eulerian.\n";
    }

    return 0;
}