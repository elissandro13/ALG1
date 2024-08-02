#include <bits/stdc++.h>

using namespace std;
#define f first
#define s second
#define NORM_RADIANOS 2 * M_PI

struct Ponto {
    double x, y;
};

class Aresta;

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

double inclinacaoRelativa(const Ponto& p, const Ponto& q) {
    return atan2(q.y - p.y, q.x - p.x);
}

bool compararInclinacao(const Ponto& a, const Ponto& b, const Ponto& A, const Ponto& B) {
    double anguloA = inclinacaoRelativa(B, A);
    double angulo1 = inclinacaoRelativa(B, a) - anguloA;
    double angulo2 = inclinacaoRelativa(B, b) - anguloA;

    if (angulo1 < 0) angulo1 += NORM_RADIANOS;
    if (angulo2 < 0) angulo2 += NORM_RADIANOS;

    return angulo1 < angulo2;
}

// Função para ordenar as arestas de um vértice no sentido anti-horário
void sortAntiHorario(Vertice* A, Vertice* B) {
    vector<pair<Ponto, Aresta*>> pontosEArestdas;

    // Preenche o vetor com os pontos finais das arestas conectadas ao vértice B
    for (auto& aresta : B->getListaArestas()) {
        pontosEArestdas.push_back(make_pair(aresta->getV2()->getCord(), aresta));
    }

    // Ordena as arestas em relação ao vértice A no sentido anti-horário
    sort(pontosEArestdas.begin(), pontosEArestdas.end(), [&](const pair<Ponto, Aresta*>& a, const pair<Ponto, Aresta*>& b) {
        return compararInclinacao(a.f, b.f, A->getCord(), B->getCord());
    });

    // Atualiza a lista de arestas do vértice B de acordo com a ordenação
    for (size_t i = 0; i < B->getListaArestas().size(); i++) {
        B->getListaArestas()[i] = pontosEArestdas[i].s;
    }
}

int main() {
    int numVertices, numArestas;
    scanf("%d %d", &numVertices, &numArestas);

    vector<Vertice*> vertices;
    vector<vector<int>> listasAdjacencia(numVertices);
    vector<vector<Vertice*>> faces;
    vector<Aresta*> arestas;


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
    for (auto& inicial : arestas) {
        if (inicial->isVisitada()) continue;

        Aresta* atual = inicial;
        vector<Vertice*> face;

        face.push_back(inicial->getV1());
        while (!atual->isVisitada()) {
            face.push_back(atual->getV2());
            atual->setVisitada(true);

            sortAntiHorario(atual->getV1(), atual->getV2());

            for (auto& prox : atual->getV2()->getListaArestas()) {
                if (prox->getV2() != atual->getV1() || atual->getV2()->getListaArestas().size() < 2) {
                    atual = prox;
                    break;
                }
            }
        }

        faces.push_back(face);
    }


    // Imprimir sáida de faces
    printf("%lu\n", faces.size());
    for (const auto& face : faces) {
        printf("%lu ", face.size());
        for (const auto& vertice : face) {
            printf("%d ", vertice->getId());
        }
        printf("\n");
    }

    // Libera memória
    for (auto aresta : arestas) {
        delete aresta;
    }

    for (auto vertice : vertices) {
        delete vertice;
    }

    return 0;
}
