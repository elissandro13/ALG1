#include <bits/stdc++.h>

using namespace std;
#define f first
#define s second

// Definição da estrutura Ponto
struct Ponto {
    double x, y;
};

// Declaração antecipada da classe Aresta
class Aresta;

// Definição da classe Vertice
class Vertice {
private:
    int id;
    Ponto cord;
    vector<Aresta*> lista_arestas;

public:
    Vertice(int _id = 0, double _x = 0, double _y = 0) : id(_id), cord{_x, _y} {}

    int getId() const {
        return id;
    }

    Ponto getCord() const {
        return cord;
    }

    void setCord(const Ponto& _cord) {
        cord = _cord;
    }

    vector<Aresta*>& getListaArestas() {
        return lista_arestas;
    }
};

// Definição da classe Aresta
class Aresta {
private:
    Vertice* v1;
    Vertice* v2;
    bool visitada;

public:
    Aresta(Vertice* _v1 = nullptr, Vertice* _v2 = nullptr) : v1(_v1), v2(_v2), visitada(false) {}

    Vertice* getV1() const {
        return v1;
    }

    Vertice* getV2() const {
        return v2;
    }

    void setV2(Vertice* _v2) {
        v2 = _v2;
    }

    bool isVisitada() const {
        return visitada;
    }

    void setVisitada(bool _visitada) {
        visitada = _visitada;
    }
};

// Função para calcular a inclinação relativa entre dois pontos
double inclinacaoRelativa(const Ponto& p, const Ponto& q) {
    return atan2(q.y - p.y, q.x - p.x);
}

// Função para determinar o tipo de curva formada por três pontos
int TipoCurva(const Ponto& a, const Ponto& b, const Ponto& c) {
    double v = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
    if (v < 0) return -1; // a está à esquerda de b
    if (v > 0) return +1; // a está à direita de b
    return 0; // a, b, c são colineares (em frente)
}

// Função para comparar a inclinação entre dois pontos em relação a outros dois pontos
bool comparareInclinacao(const Ponto& a, const Ponto& b, const Ponto& A, const Ponto& B) {
    double anguloA = inclinacaoRelativa(B, A);
    double angulo1 = inclinacaoRelativa(B, a) - anguloA;
    double angulo2 = inclinacaoRelativa(B, b) - anguloA;

    // Normalizando os ângulos para o intervalo [0, 2*PI)
    const double PI = acos(-1.0);
    if (angulo1 < 0) angulo1 += 2 * PI;
    if (angulo2 < 0) angulo2 += 2 * PI;

    // Usando TipoCurva para determinar a orientação relativa
    int tipo = TipoCurva(a, b, A);
    if (tipo == -1) return false; // a está à esquerda de b
    if (tipo == +1) return true;  // a está à direita de b

    // Caso estejam em frente ou na mesma linha
    return angulo1 < angulo2;
}

// Função para ordenar as arestas de um vértice no sentido anti-horário
void sortAngulos(Vertice* A, Vertice* B) {
    vector<pair<Ponto, Aresta*>> pontosEArestdas;

    for (auto& aresta : B->getListaArestas()) {
        pontosEArestdas.push_back(make_pair(aresta->getV2()->getCord(), aresta));
    }

    sort(pontosEArestdas.begin(), pontosEArestdas.end(), [&](const pair<Ponto, Aresta*>& a, const pair<Ponto, Aresta*>& b) {
        return comparareInclinacao(a.f, b.f, A->getCord(), B->getCord());
    });

    for (size_t i = 0; i < B->getListaArestas().size(); i++) {
        B->getListaArestas()[i] = pontosEArestdas[i].s;
    }
}

// Função para percorrer uma face a partir de uma aresta inicial
vector<Vertice*> percorrerFace(Aresta* inicial) {
    Aresta* atual = inicial;
    vector<Vertice*> face;

    face.push_back(inicial->getV1());
    while (!atual->isVisitada()) {
        face.push_back(atual->getV2());
        atual->setVisitada(true);

        sortAngulos(atual->getV1(), atual->getV2());

        // Encontrar a próxima aresta não visitada
        Aresta* prox = nullptr;
        for (auto& aresta : atual->getV2()->getListaArestas()) {
            if (!aresta->isVisitada()) {
                prox = aresta;
                break;
            }
        }
        if (prox == nullptr) break; // Caso não encontre próxima aresta não visitada

        atual = prox;
    }

    return face;
}

int main() {
    int numVertices, numArestas;
    
    scanf("%d %d", &numVertices, &numArestas);

    vector<Vertice*> vertices;
    vector<vector<int>> listasAdjacencia(numVertices);
    vector<Aresta*> arestas;
    vector<vector<Vertice*>> faces;

    // Leitura dos vértices e suas adjacências
    for (int i = 0; i < numVertices; i++) {
        double x, y;
        int grau;
        scanf("%lf %lf %d", &x, &y, &grau);
        vertices.push_back(new Vertice(i + 1, x, y));

        for (int j = 0; j < grau; j++) {
            int adjacente;
            scanf("%d", &adjacente);
            listasAdjacencia[i].push_back(adjacente - 1);
        }
    }

    // Criação das arestas a partir das adjacências
    for (int i = 0; i < numVertices; i++) {
        for (int adj : listasAdjacencia[i]) {
            Aresta* nova = new Aresta(vertices[i], vertices[adj]);
            arestas.push_back(nova);
            vertices[i]->getListaArestas().push_back(nova);
        }
    }

    // Construção das faces usando o algoritmo modificado de busca em profundidade
    for (auto& aresta : arestas) {
        if (aresta->isVisitada()) continue;

        vector<Vertice*> face = percorrerFace(aresta);
        if (face.size() > 0) {
            faces.push_back(face);
        }
    }

    // Saída do número de faces e seus vértices
    printf("%lu\n", faces.size());
    for (const auto& face : faces) {
        printf("%lu ", face.size());
        for (const auto& vertice : face) {
            printf("%d ", vertice->getId());
        }
        printf("\n");
    }

    // Liberação de memória
    for (auto aresta : arestas) {
        delete aresta;
    }

    for (auto vertice : vertices) {
        delete vertice;
    }

    return 0;
}
