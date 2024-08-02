#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <queue> // Adiciona a inclusão da biblioteca <queue>

using namespace std;

struct Edge {
    long long from;
    long long to;
    long long year;
    long long travelTime;
    long long cost;
};

using Graph = vector<Edge>;

// Union-Find structure
struct UnionFind {
    vector<long long> parent, rank;

    UnionFind(long long n) : parent(n), rank(n, 0) {
        iota(parent.begin(), parent.end(), 0);
    }

    long long find(long long u) {
        if (parent[u] != u)
            parent[u] = find(parent[u]);
        return parent[u];
    }

    bool unite(long long u, long long v) {
        long long rootU = find(u);
        long long rootV = find(v);
        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
            return true;
        }
        return false;
    }
};

void addEdge(Graph &graph, long long from, long long to, long long year, long long travelTime, long long cost) {
    graph.push_back({from, to, year, travelTime, cost});
}

pair<vector<long long>, long long> dijkstra(const vector<vector<Edge>> &graph, long long start) {
    long long n = graph.size();
    vector<long long> travelTimes(n, numeric_limits<long long>::max());
    vector<long long> maxYears(n, 0); // Vetor para rastrear o ano máximo para cada nó
    travelTimes[start] = 0;
    priority_queue<pair<long long, long long>, vector<pair<long long, long long>>, greater<pair<long long, long long>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        long long currentTime = pq.top().first;
        long long node = pq.top().second;
        pq.pop();

        if (currentTime > travelTimes[node]) continue;

        for (const auto &edge : graph[node]) {
            long long nextNode = edge.to;
            long long newTime = currentTime + edge.travelTime;
            long long newYear = max(maxYears[node], edge.year); // Atualiza o ano máximo

            if (newTime < travelTimes[nextNode]) {
                travelTimes[nextNode] = newTime;
                maxYears[nextNode] = newYear;
                pq.push({newTime, nextNode});
            } else if (newTime == travelTimes[nextNode] && newYear > maxYears[nextNode]) {
                // Se o tempo for igual, mas o ano é maior, atualiza o ano
                maxYears[nextNode] = newYear;
            }
        }
    }

    long long overallMaxYear = *max_element(maxYears.begin(), maxYears.end());
    return {travelTimes, overallMaxYear};
}

long long kruskalMST(Graph &graph, long long n, bool byYear) {
    UnionFind uf(n);
    if (byYear) {
        sort(graph.begin(), graph.end(), [](const Edge &a, const Edge &b) {
            return a.year < b.year;
        });
    } else {
        sort(graph.begin(), graph.end(), [](const Edge &a, const Edge &b) {
            return a.cost < b.cost;
        });
    }

    long long totalCostOrYear = 0;
    for (const auto &edge : graph) {
        if (uf.unite(edge.from, edge.to)) {
            totalCostOrYear = byYear ? edge.year : totalCostOrYear + edge.cost;
        }
    }

    return totalCostOrYear;
}

int main() {
    long long n, m;
    cin >> n >> m; // Número de nós e arestas no grafo
    vector<vector<Edge>> adjList(n);
    Graph edges;

    for (long long i = 0; i < m; ++i) {
        long long from, to, travelTime, year, cost;
        cin >> from >> to >> year >> travelTime >> cost;
        from--; // Ajusta o índice para 0-based
        to--;   // Ajusta o índice para 0-based
        addEdge(edges, from, to, year, travelTime, cost);
        adjList[from].push_back({from, to, year, travelTime, cost});
        adjList[to].push_back({to, from, year, travelTime, cost});
    }

    long long startNode = 0; // Você pode alterar o nó inicial conforme necessário

    auto result = dijkstra(adjList, startNode);
    vector<long long> times = result.first;
    long long maxYear = result.second;

    for (long long i = 0; i < n; ++i) {
        cout << times[i] << endl;
    }

    cout << maxYear << endl;

    long long minYearToConnect = kruskalMST(edges, n, true);

    cout << minYearToConnect << endl;

    long long minCostToConnect = kruskalMST(edges, n, false);

    cout << minCostToConnect << endl;

    return 0;
}
