#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

class Graph
{
public:
    Graph(size_t vertices);
    void addEdge(int u, int v, int weight);
    vector<int> dijkstra(int start);

private:
    size_t vertices;
    vector<vector<pair<size_t, size_t>>> adj;
};

Graph::Graph(size_t vertices) : vertices(vertices), adj(vertices) {}

void Graph::addEdge(int u, int v, int weight)
{
    adj[u].emplace_back(weight, v);
}

vector<int> Graph::dijkstra(int start)
{
    vector<int> distance(vertices, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    distance[start] = 0;
    pq.emplace(0, start);

    while (!pq.empty())
    {
        int dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (dist > distance[u])
        {
            continue;
        }

        for (const auto &edge : adj[u])
        {
            int v = edge.second;
            int weight = edge.first;

            if (distance[u] + weight < distance[v])
            {
                distance[v] = distance[u] + weight;
                pq.emplace(distance[v], v);
            }
        }
    }

    return distance;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    int numRuns = stoi(argv[1]);

    Graph g(20);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 1, 4);
    g.addEdge(2, 3, 8);
    g.addEdge(2, 4, 2);
    g.addEdge(3, 4, 7);
    g.addEdge(4, 3, 9);
    g.addEdge(4, 5, 5);
    g.addEdge(5, 6, 6);
    g.addEdge(5, 7, 1);
    g.addEdge(6, 7, 2);
    g.addEdge(6, 8, 3);
    g.addEdge(7, 8, 4);
    g.addEdge(7, 9, 5);
    g.addEdge(8, 9, 6);
    g.addEdge(8, 10, 7);
    g.addEdge(9, 10, 8);
    g.addEdge(9, 11, 9);
    g.addEdge(10, 11, 10);
    g.addEdge(10, 12, 1);
    g.addEdge(11, 12, 2);
    g.addEdge(11, 13, 3);
    g.addEdge(12, 13, 4);
    g.addEdge(12, 14, 5);
    g.addEdge(13, 14, 6);
    g.addEdge(13, 15, 7);
    g.addEdge(14, 15, 8);
    g.addEdge(14, 16, 9);
    g.addEdge(15, 16, 10);
    g.addEdge(15, 17, 1);
    g.addEdge(16, 17, 2);
    g.addEdge(16, 18, 3);
    g.addEdge(17, 18, 4);
    g.addEdge(17, 19, 5);
    g.addEdge(18, 19, 6);

    for (int i = 0; i < numRuns; ++i)
    {
        vector<int> distances = g.dijkstra(0);
        unsigned long long total_distance = 0;
        for (int j = 0; j < distances.size(); ++j)
        {
            if (distances[j] == INF)
            {
                std::cerr << "0 to " << j << " is unreachable" << endl;
                break;
            }
            else
            {
                total_distance += distances[j];
            }
        }
    }

    return 0;
}
