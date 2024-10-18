#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

// Graph structure using adjacency list
class Graph
{
    int V;                       // Number of vertices
    vector<pair<int, int>> *adj; // Adjacency list: (destination, weight)

public:
    Graph(int V);                      // Constructor
    void addEdge(int u, int v, int w); // Add edge with weight
    void dijkstra(int src);            // Dijkstra's Algorithm
    void bellmanFord(int src);         // Bellman-Ford Algorithm
};

// Constructor
Graph::Graph(int V)
{
    this->V = V;
    adj = new vector<pair<int, int>>[V];
}

// Add edge to the graph with weight
void Graph::addEdge(int u, int v, int w)
{
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w)); // For undirected graph
}

// Dijkstra's algorithm for shortest paths from src
void Graph::dijkstra(int src)
{
    // Priority queue to store (distance, vertex)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // Initialize distances to infinity
    vector<int> dist(V, INT_MAX);
    dist[src] = 0; // Distance to source is 0
    pq.push(make_pair(0, src));

    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        // Explore all neighbors of u
        for (vector<pair<int, int>>::iterator it = adj[u].begin(); it != adj[u].end(); ++it)
        {
            pair<int, int> neighbor = *it;

            int v = neighbor.first;
            int weight = neighbor.second;

            // Relax the edge
            if (dist[v] > dist[u] + weight)
            {
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    // Print the shortest distances
    cout << "Dijkstra's Shortest Paths from vertex " << src << ":\n";
    for (int i = 0; i < V; i++)
    {
        if (dist[i] == INT_MAX)
            cout << "Vertex " << i << ": Unreachable\n";
        else
            cout << "Vertex " << i << ": " << dist[i] << "\n";
    }
}

// Bellman-Ford algorithm for shortest paths from src
void Graph::bellmanFord(int src)
{
    vector<int> dist(V, INT_MAX);
    dist[src] = 0;

    // Relax all edges |V|-1 times
    for (int i = 1; i <= V - 1; i++)
    {
        for (int u = 0; u < V; u++)
        {
            for (vector<pair<int, int>>::iterator it = adj[u].begin(); it != adj[u].end(); ++it)
            {
                pair<int, int> neighbor = *it; // Get the neighbor as a pair
                                               // Process neighbor here

                int v = neighbor.first;
                int weight = neighbor.second;

                if (dist[u] != INT_MAX && dist[v] > dist[u] + weight)
                {
                    dist[v] = dist[u] + weight;
                }
            }
        }
    }

    // Check for negative-weight cycles
    for (int u = 0; u < V; u++)
    {
        for (vector<pair<int, int>>::iterator it = adj[u].begin(); it != adj[u].end(); ++it)
        {
            pair<int, int> neighbor = *it; // Get the neighbor as a pair
                                           // Process neighbor here

            int v = neighbor.first;
            int weight = neighbor.second;
            if (dist[u] != INT_MAX && dist[v] > dist[u] + weight)
            {
                cout << "Graph contains a negative-weight cycle\n";
                return;
            }
        }
    }

    // Print the shortest distances
    cout << "Bellman-Ford Shortest Paths from vertex " << src << ":\n";
    for (int i = 0; i < V; i++)
    {
        if (dist[i] == INT_MAX)
            cout << "Vertex " << i << ": Unreachable\n";
        else
            cout << "Vertex " << i << ": " << dist[i] << "\n";
    }
}

int main()
{
    srand(time(0));

    int V = 5; // Number of vertices
    Graph g(V);

    // Adding random edges with random weights
    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            int weight = rand() % 20 + 1; // Random weight between 1 and 20
            g.addEdge(i, j, weight);
            cout << "Weight from " << i << " to " << j << " is " << weight << endl;
        }
    }

    int src;
    cout << "Enter the source vertex (0 to " << V - 1 << "): ";
    cin >> src;

    cout << "\nApplying Dijkstra's Algorithm:\n";
    g.dijkstra(src);

    cout << "\nApplying Bellman-Ford Algorithm:\n";
    g.bellmanFord(src);

    return 0;
}