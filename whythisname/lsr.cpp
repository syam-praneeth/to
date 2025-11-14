#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

// Find the vertex with the minimum distance value, from the set of vertices not yet included in the shortest path tree
int minDistance(const vector<int> &dist, const vector<bool> &visited) {
    int minVal = INF;
    int idx = -1;
    for (size_t v = 0; v < dist.size(); ++v) {
        if (!visited[v] && dist[v] <= minVal) {
            minVal = dist[v];
            idx = (int)v;
        }
    }
    return idx;
}

int main() {
    int n;
    cout << "Enter number of nodes: ";
    cin >> n;
    if (n <= 0) {
        return 0;
    }

    vector<vector<int>> graph(n, vector<int>(n));
    cout << "Enter adjacency matrix (0 if no direct link):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> graph[i][j];
        }
    }

    int src;
    cout << "Enter the source node (0 to " << (n - 1) << "): ";
    cin >> src;
    if (src < 0 || src >= n) {
        cout << "Invalid source\n";
        return 1;
    }

    vector<int> dist(n, INF), prev(n, -1);
    vector<bool> visited(n, false);
    dist[src] = 0;

    for (int iter = 0; iter < n - 1; ++iter) {
        int u = minDistance(dist, visited);
        if (u == -1) {
            break;
        }
        visited[u] = true;
        for (int v = 0; v < n; ++v) {
            if (!visited[v] && graph[u][v] != 0 && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
        }
    }

    cout << "\nNode\tDistance from Source\tPath\n";
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF) {
            cout << i << "\t\tINF\t\tNo path\n";
            continue;
        }
        cout << i << "\t\t" << dist[i] << "\t\t";
        vector<int> path;
        for (int crawl = i; crawl != -1; crawl = prev[crawl]) {
            path.push_back(crawl);
        }
        for (int j = (int)path.size() - 1; j >= 0; --j) {
            cout << path[j] << (j > 0 ? " " : "");
        }
        cout << "\n";
    }
    return 0;
}
