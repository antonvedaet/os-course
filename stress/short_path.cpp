#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

class Graph {
public:
    Graph(size_t vertices);
    void addEdge(int u, int v, int weight);
    vector<int> dijkstra(int start);

private:
    size_t vertices;
    vector<vector<pair<size_t, size_t>>> adj;
};

Graph::Graph(size_t vertices) : vertices(vertices), adj(vertices) {}

void Graph::addEdge(int u, int v, int weight) {
    adj[u].emplace_back(weight, v);
}

vector<int> Graph::dijkstra(int start) {
    vector<int> distance(vertices, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    distance[start] = 0;
    pq.emplace(0, start);

    while (!pq.empty()) {
        int dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (dist > distance[u]) {
            continue;
        }

        for (const auto& edge : adj[u]) {
            int v = edge.second;
            int weight = edge.first;

            if (distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                pq.emplace(distance[v], v);
            }
        }
    }

    return distance;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <number_of_runs>" << endl;
        return 1;
    }

    int numRuns = stoi(argv[1]);

    Graph g(5);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 1, 4);
    g.addEdge(2, 3, 8);
    g.addEdge(2, 4, 2);
    g.addEdge(3, 4, 7);
    g.addEdge(4, 3, 9);

    for (int i = 0; i < numRuns; ++i) {
        cout << "run " << (i + 1) << ":" << endl;
        vector<int> distances = g.dijkstra(0);
        unsigned long long total_distance = 0;
        for (int j = 0; j < distances.size(); ++j) {
            if (distances[j] == INF) {
                std::cerr << "0 to " << j << " is unreachable" << endl;
                break;
            } else {
                total_distance += distances[j];
            }
        }
        std::cout << "result: " << total_distance << endl;
        cout << endl;
    }

    return 0;
}
