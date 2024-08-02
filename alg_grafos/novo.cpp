#include <bits/stdc++.h>

using namespace std;
#define f first
#define s second
#define NORM_RADIANOS 2 * M_PI

// Definição da estrutura Ponto para representar coordenadas
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

    // Métodos para obter o ID e as coordenadas do vértice
    int getId() const {
        return id;
    }

    Ponto getCord() const {
        return cord;
    }

    // Método para definir as coordenadas do vértice
    void setCord(const Ponto& _cord) {
        cord = _cord;
    }

    // Método para obter a lista de arestas conectadas ao vértice
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

    // Métodos para obter os vértices da aresta
    Vertice* getV1() const {
        return v1;
    }

    Vertice* getV2() const {
        return v2;
    }

    // Método para definir o segundo vértice da aresta
    void setV2(Vertice* _v2) {
        v2 = _v2;
    }

    // Métodos para verificar e definir se a aresta foi visitada
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

// Função para comparar a inclinação entre dois pontos em relação a outros dois pontos
bool comparareInclinacao(const Ponto& a, const Ponto& b, const Ponto& A, const Ponto& B) {
    double anguloA = inclinacaoRelativa(B, A);
    double angulo1 = inclinacaoRelativa(B, a) - anguloA;
    double angulo2 = inclinacaoRelativa(B, b) - anguloA;

    // Normaliza os ângulos para o intervalo [0, 2*PI)
    const double PI = acos(-1.0);
    if (angulo1 < 0) angulo1 += NORM_RADIANOS;
    if (angulo2 < 0) angulo2 += NORM_RADIANOS;

    return angulo1 < angulo2;
}

// Função para ordenar as arestas de um vértice no sentido anti-horário
void sortAngulos(Vertice* A, Vertice* B) {
    vector<pair<Ponto, Aresta*>> pontosEArestdas;

    // Preenche o vetor com os pontos finais das arestas conectadas ao vértice B
    for (auto& aresta : B->getListaArestas()) {
        pontosEArestdas.push_back(make_pair(aresta->getV2()->getCord(), aresta));
    }

    // Ordena as arestas em relação ao vértice A no sentido anti-horário
    sort(pontosEArestdas.begin(), pontosEArestdas.end(), [&](const pair<Ponto, Aresta*>& a, const pair<Ponto, Aresta*>& b) {
        return comparareInclinacao(a.f, b.f, A->getCord(), B->getCord());
    });

    // Atualiza a lista de arestas do vértice B de acordo com a ordenação
    for (size_t i = 0; i < B->getListaArestas().size(); i++) {
        B->getListaArestas()[i] = pontosEArestdas[i].s;
    }
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    vector<Vertice*> vertices;
    vector<vector<int>> listasAdjacencia(n);
    vector<Aresta*> arestas;
    vector<vector<Vertice*>> faces;

    // Leitura dos vértices e suas adjacências
    for (int i = 0; i < n; i++) {
        double x, y;
        int grau;
        scanf("%lf %lf %d", &x, &y, &grau);
        vertices.push_back(new Vertice(i + 1, x, y));

        // Leitura dos vértices adjacentes
        for (int j = 0; j < grau; j++) {
            int adjacente;
            scanf("%d", &adjacente);
            listasAdjacencia[i].push_back(adjacente - 1);
        }
    } 

    // Criação das arestas a partir das adjacências
    for (int i = 0; i < n; i++) {
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

            sortAngulos(atual->getV1(), atual->getV2());

            // Encontra a próxima aresta não visitada na lista de adjacência do vértice atual
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

        faces.push_back(face);
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
