#include <iostream>
#include <filesystem>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <limits>

namespace fs = std::filesystem;
using namespace std;

const int INF = numeric_limits<int>::max();

void find_file(const string& filename, const fs::path& search_path, int repeat) {
    for (int i = 0; i < repeat; ++i) {
        cout << "search... (" << (i + 1) << "/" << repeat << ")" << endl;
        bool found = false;
        for (const auto& entry : fs::recursive_directory_iterator(search_path)) {
            if (entry.is_regular_file() && entry.path().filename() == filename) {
                cout << entry.path() << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "file not found" << endl;
        }
    }
}

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

void run_dijkstra(Graph& g, int numRuns) {
    for (int i = 0; i < numRuns; ++i) {
        cout << "run " << (i + 1) << ":" << endl;
        vector<int> distances = g.dijkstra(0);
        unsigned long long total_distance = 0;
        for (int j = 0; j < distances.size(); ++j) {
            if (distances[j] == INF) {
                cerr << "0 to " << j << " is unreachable" << endl;
                break;
            } else {
                total_distance += distances[j];
            }
        }
        cout << "result: " << total_distance << endl;
        cout << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <filename> <search_path> <file_repeat> <dijkstra_runs>" << endl;
        return 1;
    }

    string filename = argv[1];
    fs::path search_path = argv[2];
    int file_repeat = stoi(argv[3]);
    int dijkstra_runs = stoi(argv[4]);

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

    thread t1(find_file, filename, search_path, file_repeat);
    thread t2(run_dijkstra, ref(g), dijkstra_runs);

    t1.join();
    t2.join();

    return 0;
}
