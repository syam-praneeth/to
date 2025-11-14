#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

const int INF = 9999;

struct RouteEntry {
    vector<int> dist;
    vector<int> nextHop;
};

int main() {
    int nodes;
    cout << "Enter number of nodes: ";
    cin >> nodes;
    if (nodes <= 0) {
        return 0;
    }

    vector<vector<int>> cost(nodes, vector<int>(nodes));
    cout << "\nEnter the cost matrix (use -1 for no direct link):\n";
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            cin >> cost[i][j];
            if (cost[i][j] == -1 && i != j) {
                cost[i][j] = INF;
            }
            if (i == j) {
                cost[i][j] = 0;
            }
        }
    }

    vector<RouteEntry> table(nodes);
    for (int i = 0; i < nodes; i++) {
        table[i].dist.resize(nodes);
        table[i].nextHop.resize(nodes);
        for (int j = 0; j < nodes; j++) {
            table[i].dist[j] = cost[i][j];
            table[i].nextHop[j] = j;
        }
    }

    bool updated;
    do {
        updated = false;
        for (int i = 0; i < nodes; i++) {
            for (int j = 0; j < nodes; j++) {
                for (int k = 0; k < nodes; k++) {
                    if (table[i].dist[j] > cost[i][k] + table[k].dist[j]) {
                        table[i].dist[j] = cost[i][k] + table[k].dist[j];
                        table[i].nextHop[j] = k;
                        updated = true;
                    }
                }
            }
        }
    } while (updated);

    for (int i = 0; i < nodes; i++) {
        cout << "\nRouting table for node " << char('A' + i) << ":\n";
        cout << "Destination\tDistance\tNext Hop\n";
        for (int j = 0; j < nodes; j++) {
            if (table[i].dist[j] >= INF) {
                cout << char('A' + j) << "\t\tINF\t\t-\n";
            } else {
                cout << char('A' + j) << "\t\t" << table[i].dist[j] << "\t\t" << char('A' + table[i].nextHop[j]) << "\n";
            }
        }
    }
    return 0;
}
