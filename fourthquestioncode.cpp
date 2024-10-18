#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <set>

using namespace std;

// Define a structure to represent an edge for Kruskal's algorithm
struct Edge {
    int src, dest, weight;
};

// Define a structure to represent a subset for union-find in Kruskal's algorithm
struct Subset {
    int parent, rank;
};

// Compare function for sorting edges based on their weights in Kruskal's algorithm
bool compareEdges(const Edge &a, const Edge &b) {
    return a.weight < b.weight;
}

// Find set of an element in union-find (with path compression) for Kruskal's algorithm
int find(Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

// Perform union of two subsets (by rank) for Kruskal's algorithm
void Union(Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Kruskal's algorithm to find Minimum Spanning Tree
void Kruskal(vector<Edge> &edges, int V, vector<Edge> &mst) {
    sort(edges.begin(), edges.end(), compareEdges);

    Subset *subsets = new Subset[V];
    for (int i = 0; i < V; ++i) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }

    int edgeIndex = 0;
    while (mst.size() < V - 1) {
        Edge nextEdge = edges[edgeIndex++];

        int x = find(subsets, nextEdge.src);
        int y = find(subsets, nextEdge.dest);

        if (x != y) {
            mst.push_back(nextEdge);
            Union(subsets, x, y);
        }
    }

    cout << "Kruskal's MST edges:" << endl;
    for (const Edge &edge : mst)
        cout << edge.src << " -- " << edge.dest << " == " << edge.weight << endl;

    delete[] subsets;
}

// Prim's algorithm to find Minimum Spanning Tree
void primMST(vector<vector<int>>& graph, int V, vector<Edge> &mst) {
    vector<int> parent(V, -1);
    vector<int> key(V, INT_MAX);
    vector<bool> inMST(V, false);

    key[0] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, 0});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        inMST[u] = true;

        for (int v = 0; v < V; ++v) {
            if (!inMST[v] && graph[u][v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }

    for (int i = 1; i < V; ++i) {
        mst.push_back({parent[i], i, graph[i][parent[i]]});
    }

    cout << "Prim's MST edges:" << endl;
    for (const Edge &edge : mst) {
        cout << edge.src << " - " << edge.dest << " Weight: " << edge.weight << endl;
    }
}

// DFS to find cutsets
void dfsCutset(int u, const vector<vector<int>>& mstAdj, vector<bool>& visited, set<int>& visitedNodes) {
    visited[u] = true;
    visitedNodes.insert(u);
    for (int v : mstAdj[u]) {
        if (!visited[v]) {
            dfsCutset(v, mstAdj, visited, visitedNodes);
        }
    }
}

// Find the fundamental cutsets
void findFundamentalCutsets(const vector<Edge>& mst, int V) {
    vector<vector<int>> mstAdj(V);
    for (const Edge& edge : mst) {
        mstAdj[edge.src].push_back(edge.dest);
        mstAdj[edge.dest].push_back(edge.src);
    }

    cout << "\nFundamental Cutsets:" << endl;
    for (const Edge& edge : mst) {
        vector<bool> visited(V, false);
        set<int> visitedNodes;
        dfsCutset(edge.src, mstAdj, visited, visitedNodes);
        cout << "Cutset for edge " << edge.src << " - " << edge.dest << ": { ";
        for (int i = 0; i < V; ++i) {
            if (visitedNodes.find(i) != visitedNodes.end()) {
                cout << i << " ";
            }
        }
        cout << "}" << endl;
    }
}

// Find the fundamental circuits
void findFundamentalCircuits(const vector<Edge>& mst, const vector<Edge>& edges) {
    cout << "\nFundamental Circuits:" << endl;
    for (const Edge& edge : edges) {
        bool inMST = false;
        for (const Edge& mstEdge : mst) {
            if ((edge.src == mstEdge.src && edge.dest == mstEdge.dest) || (edge.src == mstEdge.dest && edge.dest == mstEdge.src)) {
                inMST = true;
                break;
            }
        }
        if (!inMST) {
            cout << "Circuit for non-MST edge " << edge.src << " - " << edge.dest << ": { ";
            cout << edge.src << " " << edge.dest << " }" << endl;
        }
    }
}

int main() {
    int V;
    cout << "Enter the number of vertices: ";
    cin >> V;

    // Input graph for Prim's algorithm (adjacency matrix)
    vector<vector<int>> graph(V, vector<int>(V));
    cout << "Enter the adjacency matrix of the graph:" << endl;
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            cin >> graph[i][j];
        }
    }

    // Convert adjacency matrix to edge list for Kruskal's algorithm
    vector<Edge> edges;
    for (int i = 0; i < V; ++i) {
        for (int j = i + 1; j < V; ++j) {
            if (graph[i][j] != 0) {  // If there's an edge
                edges.push_back({i, j, graph[i][j]});
            }
        }
    }

    // Create vectors to store MST edges for both algorithms
    vector<Edge> primMSTResult, kruskalMSTResult;

    // Call Prim's algorithm
    primMST(graph, V, primMSTResult);

    // Call Kruskal's algorithm
    Kruskal(edges, V, kruskalMSTResult);

    // Find and print the fundamental cutsets and circuits with respect to the MST
    findFundamentalCutsets(primMSTResult, V);
    findFundamentalCircuits(primMSTResult, edges);

    return 0;
}
