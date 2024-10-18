#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>

using namespace std;

// Graph structure
class Graph {
private:
    int V; // Number of vertices
    vector<vector<int> > adj; // Adjacency list

public:
    Graph(int V); // Constructor
    void addEdge(int u, int v); // Add an edge
    void havelHakimi(vector<int>& degree); // Build graph from graphic sequence
    int edgeConnectivity(); // Find edge connectivity
    int vertexConnectivity(); // Find vertex connectivity
    bool bfs(int s, vector<bool>& visited); // BFS for connectivity
};

// Constructor
Graph::Graph(int V) : V(V) {
    adj.resize(V);
}

// Add an edge to the graph
void Graph::addEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u); // Undirected graph
}

// Build the graph using Havel-Hakimi algorithm
void Graph::havelHakimi(vector<int>& degree) {
    while (true) {
        sort(degree.rbegin(), degree.rend()); // Sort in descending order
        while (!degree.empty() && degree.back() == 0) {
            degree.pop_back(); // Remove zeros
        }
        if (degree.empty()) break; // All degrees satisfied

        int d = degree.front();
        degree.erase(degree.begin()); // Remove the first element

        if (d > degree.size()) {
            cout << "The degree sequence cannot form a simple graph." << endl;
            return;
        }

        for (int i = 0; i < d; i++) {
            degree[i]--;
            if (degree[i] < 0) {
                cout << "The degree sequence cannot form a simple graph." << endl;
                return;
            }
        }

        // Add edges based on the modified degree sequence
        for (int i = 0; i < d; i++) {
            addEdge(0, i + 1); // Connect first vertex to the next d vertices
        }
    }
}

// Check connectivity using BFS
bool Graph::bfs(int s, vector<bool>& visited) {
    queue<int> q;
    visited[s] = true;
    q.push(s);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
for (vector<int>::iterator it = adj[u].begin(); it != adj[u].end(); ++it) {
    int v = *it;  // Get the value from the iterator
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    // Check if all vertices are visited
    for (int i = 0; i < V; i++) {
        if (!visited[i]) return false;
    }
    return true;
}

// Find edge connectivity
int Graph::edgeConnectivity() {
    int minCut = INT_MAX;

    for (int u = 0; u < V; u++) {
        vector<bool> visited(V, false);
        visited[u] = true; // Mark vertex u as removed
        int cutSize = 0;

        for (int v = 0; v < V; v++) {
            if (!visited[v]) {
                vector<bool> bfsVisited(V, false);
                bfs(v, bfsVisited);
                int count = 0;
                for (int w = 0; w < V; w++) {
                    if (bfsVisited[w]) {
                        count++;
                    }
                }
                cutSize = max(cutSize, count);
            }
        }
        minCut = min(minCut, cutSize);
    }
    return minCut;
}

// Find vertex connectivity
int Graph::vertexConnectivity() {
    int minCut = INT_MAX;

    for (int u = 0; u < V; u++) {
        vector<bool> visited(V, false);
        visited[u] = true; // Mark vertex u as removed
        int count = 0;

        for (int v = 0; v < V; v++) {
            if (!visited[v]) {
                vector<bool> bfsVisited(V, false);
                if (bfs(v, bfsVisited)) {
                    count++;
                }
            }
        }
        minCut = min(minCut, count);
    }
    return minCut;
}

int main() {
    int V;
    cout << "Enter the number of vertices: ";
    cin >> V;

    vector<int> degreeSequence(V);
    cout << "Enter the degree sequence: ";
    for (int i = 0; i < V; i++) {
        cin >> degreeSequence[i];
    }

    Graph g(V);
    g.havelHakimi(degreeSequence);

    // Find connectivity
    int edgeConn = g.edgeConnectivity();
    int vertexConn = g.vertexConnectivity();

    cout << "Edge Connectivity: " << edgeConn << endl;
    cout << "Vertex Connectivity: " << vertexConn << endl;

    cout << "The value of K for which the graph is K-connected is: " << min(edgeConn, vertexConn) << endl;

    return 0;
}