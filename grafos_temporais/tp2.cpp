#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>
#include <functional>
#include <algorithm>



using namespace std;

struct Edge {
    long long int to;
    long long int year;
    long long int travelTime;
    long long int cost;
};

using Graph = vector<vector<Edge>>;

void addEdge(Graph &graph, long long int from, long long int to, long long int year, long long int travelTime, long long int cost) {
    graph[from].push_back({to, year, travelTime, cost});
    graph[to].push_back({from, year, travelTime, cost});
}

pair<vector<long long int>, long long int> dijkstra(const Graph &graph, long long int start) {
    long long int n = graph.size();
    vector<long long int> travelTimes(n, numeric_limits<long long int>::max());
    vector<long long int> maxYears(n, 0); // Vetor para rastrear o ano máximo para cada nó
    travelTimes[start] = 0;
    priority_queue<pair<long long int, long long int>, vector<pair<long long int, long long int>>, greater<pair<long long int, long long int>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        long long int currentTime = pq.top().first;
        long long int node = pq.top().second;
        pq.pop();

        if (currentTime > travelTimes[node]) continue;

        for (const auto &edge : graph[node]) {
            long long int nextNode = edge.to;
            long long int newTime = currentTime + edge.travelTime;
            long long int newYear = max(maxYears[node], edge.year); // Atualiza o ano máximo

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

    long long int overallMaxYear = *max_element(maxYears.begin(), maxYears.end());
    return {travelTimes, overallMaxYear};
}

void addEdges(const Graph &graph, long long int node, vector<bool> &inMST, priority_queue<tuple<long long int, long long int, long long int>, vector<tuple<long long int, long long int, long long int>>, greater<tuple<long long int, long long int, long long int>>> &pq) {
    inMST[node] = true; // Marca o nó como incluído na MST
    for (const auto &edge : graph[node]) { // Itera sobre todas as arestas adjacentes
        if (!inMST[edge.to]) { // Se o nó adjacente ainda não está na MST
            pq.emplace(edge.year, node, edge.to); // Adiciona a aresta à fila de prioridade
        }
    }
}

long long int primMST(const Graph &graph) {
    long long int n = graph.size();
    vector<bool> inMST(n, false);
    priority_queue<tuple<long long int, long long int, long long int>, vector<tuple<long long int, long long int, long long int>>, greater<tuple<long long int, long long int, long long int>>> pq;
    long long int maxYearInMST = 0;

    addEdges(graph, 0, inMST, pq); // Inicia a partir do nó 0

    while (!pq.empty()) {
        auto [year, from, to] = pq.top();
        pq.pop();

        if (inMST[to]) continue;

        maxYearInMST = max(maxYearInMST, year);
        addEdges(graph, to, inMST, pq); // Adiciona as arestas do novo nó incluído à MST
    }

    return maxYearInMST;
}

void addEdgesByCost(const Graph &graph, long long int node, vector<bool> &inMST, priority_queue<tuple<long long int, long long int, long long int>, vector<tuple<long long int, long long int, long long int>>, greater<tuple<long long int, long long int, long long int>>> &pq) {
    inMST[node] = true; // Marca o nó como incluído na MST
    for (const auto &edge : graph[node]) { // Itera sobre todas as arestas adjacentes
        if (!inMST[edge.to]) { // Se o nó adjacente ainda não está na MST
            pq.emplace(edge.cost, node, edge.to); // Adiciona a aresta à fila de prioridade
        }
    }
}

long long int primMSTByCost(const Graph &graph) {
    long long int n = graph.size();
    vector<bool> inMST(n, false); // Marca se o nó está na MST
    priority_queue<tuple<long long int, long long int, long long int>, vector<tuple<long long int, long long int, long long int>>, greater<tuple<long long int, long long int, long long int>>> pq; // Fila de prioridade para as arestas
    long long int totalCost = 0; // Custo total da MST

    addEdgesByCost(graph, 0, inMST, pq); // Inicia a partir do nó 0

    while (!pq.empty()) {
        auto [cost, from, to] = pq.top(); // Obtém a aresta com o menor custo
        pq.pop();

        if (inMST[to]) continue; // Se o nó já está na MST, ignora

        totalCost += cost; // Adiciona o custo da aresta ao custo total
        addEdgesByCost(graph, to, inMST, pq); // Adiciona as arestas do novo nó incluído à MST
    }

    return totalCost;
}

int main() {
    long long int n, m;
    cin >> n >> m; // Número de nós e arestas no grafo
    Graph graph(n);

    for (long long int i = 0; i < m; ++i) {
        long long int from, to, travelTime, year, cost;
        cin >> from >> to >> year >> travelTime >> cost;
        from--; // Ajusta o índice para 0-based
        to--;   // Ajusta o índice para 0-based
        addEdge(graph, from, to, year, travelTime, cost);
    }

    long long int startNode = 0; // Você pode alterar o nó inicial conforme necessário

    auto result = dijkstra(graph, startNode);
    vector<long long int> times = result.first;
    long long int maxYear = result.second;

    for (long long int i = 0; i < n; ++i) {
        cout << times[i] << endl;
    }

    cout << maxYear << endl;

    long long int minYearToConnect = primMST(graph);

    cout << minYearToConnect << endl;

    long long int minCostToConnect = primMSTByCost(graph);

    cout << minCostToConnect << endl;

    return 0;
}
