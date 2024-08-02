#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct Ponto {
    double x, y;
};

double Distancia(Ponto a, Ponto b) {
    double x = (a.x - b.x), y = (a.y - b.y);
    return sqrt(x*x + y*y);
}

double Inclinacao(Ponto p) {
    return atan2(p.y, p.x);
}

double InclinacaoRelativa(Ponto p, Ponto q) {
    return atan2(q.y - p.y, q.x - p.x);
}

int TipoCurva(Ponto a, Ponto b, Ponto c) {
    double v = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
    if (v < 0) return -1; // esquerda.
    if (v > 0) return +1; // direita.
    return 0; // em frente.
}

struct Edge {
    int v, w;
    double angle;
};

bool compareEdges(const Edge& a, const Edge& b) {
    return a.angle < b.angle;
}

void findFaces(vector<vector<Edge>>& adj, vector<vector<int>>& faces, vector<bool>& visited, int u, vector<int>& currentFace) {
    visited[u] = true;
    currentFace.push_back(u);

    for (const Edge& e : adj[u]) {
        if (!visited[e.w]) {
            findFaces(adj, faces, visited, e.w, currentFace);
        } else if (find(currentFace.begin(), currentFace.end(), e.w) != currentFace.end()) {
            // Encontramos o fim da face
            faces.push_back(currentFace);
            currentFace.clear();
        }
    }
}

int main() {
    int N, M;
    cin >> N >> M;

    vector<Ponto> pontos(N);
    vector<vector<Edge>> adj(N);
    vector<bool> visited(N, false);

    for (int i = 0; i < N; ++i) {
        cin >> pontos[i].x >> pontos[i].y;
        int d;
        cin >> d;
        for (int j = 0; j < d; ++j) {
            int v;
            cin >> v;
            v--; // Ajustando para índices começando em 0
            adj[i].push_back({i, v, InclinacaoRelativa(pontos[i], pontos[v])});
        }
        sort(adj[i].begin(), adj[i].end(), compareEdges);
    }

    vector<vector<int>> faces;
    vector<int> currentFace;

    for (int i = 0; i < N; ++i) {
        if (!visited[i]) {
            findFaces(adj, faces, visited, i, currentFace);
        }
    }

    cout << faces.size() << endl;
    for (auto& face : faces) {
        cout << face.size() << " ";
        for (int v : face) {
            cout << v + 1 << " ";
        }
        cout << endl;
    }

    return 0;
}