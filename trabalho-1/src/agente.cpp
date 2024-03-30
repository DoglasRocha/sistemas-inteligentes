#include "../include/agente.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

// #define DEBUG

using namespace std;

Agente::Agente(Ambiente *ambiente)
{
    srand(time(NULL));
    this->ambiente = ambiente;
    this->qtd_cidades = this->ambiente->get_qtd_cidades();
    this->cidades_visitadas = 0;

    this->visitados = new bool[this->qtd_cidades];
    for (int i = 0; i < this->qtd_cidades; i++)
        this->visitados[i] = false;
}

Agente::~Agente()
{
    if (visitados != nullptr)
        delete[] visitados;
    if (caminho != nullptr)
        delete caminho;
}

void Agente::reset()
{
    this->cidades_visitadas = 0;

    delete[] this->visitados;
    this->visitados = nullptr;
    this->visitados = new bool[this->qtd_cidades];
    for (int i = 0; i < this->qtd_cidades; i++)
        this->visitados[i] = false;

    delete caminho;
    caminho = nullptr;
}

int Agente::computa_media_caminhos_vizinhos(Node *cidade)
{
    int sum = 0, qtd_vizinhos = 0;

    for (Node *aux = cidade; aux != nullptr; aux = aux->next)
    {
        qtd_vizinhos++;
        sum += aux->weight;
    }

    return sum / qtd_vizinhos;
}

int Agente::computa_qtd_vizinhos(Node *cidade)
{
    int qtd_vizinhos = 0;

    for (Node *aux = cidade; aux != nullptr; aux = aux->next)
    {
        qtd_vizinhos++;
    }

    return qtd_vizinhos;
}

int Agente::escolhe_aleatorio(int limite)
{
    return rand() % limite;
}

int Agente::tempera_simulada(int *erro)
{
    State atualS;
    Node *proximo, *atual, *aux;
    int origem, qtd_vizinhos, t, tam_caminho = 0;

    // init
    origem = this->escolhe_aleatorio(this->qtd_cidades);
    atual = ambiente->get_cidade(origem);
    caminho = new Node(origem, 0, nullptr);
    qtd_vizinhos = this->computa_qtd_vizinhos(atual);
    atualS.id = origem;
    atualS.weight = this->computa_media_caminhos_vizinhos(atual);

#ifdef DEBUG
    cout << "t " << t << endl;
#endif

    t = ambiente->get_tam_mapa() * 1.5;
    for (int i = 0; i < t; i++)
    {
        int proximo_vizinho = 0;
        atual = ambiente->get_cidade(atualS.id);
        qtd_vizinhos = this->computa_qtd_vizinhos(atual);

        do
        {
            proximo_vizinho = this->escolhe_aleatorio(qtd_vizinhos);
            int j;
            for (j = 0, aux = atual; j < proximo_vizinho; j++)
                aux = aux->next;

            proximo = aux;

#ifdef DEBUG
            cout << "id proximo " << proximo->id << endl;
            cout << "[ ";
            for (int k = 0; k < qtd_cidades; k++)
                cout << visitados[k] << " ";
            cout << "]" << endl;
#endif

        } while (visitados[proximo->id] && cidades_visitadas < qtd_cidades);
        if (proximo->id == origem && cidades_visitadas < qtd_cidades - 1)
            continue;

#ifdef DEBUG
        cout << "atual weight " << atualS.weight << endl;
#endif

        int delta_e = atualS.weight - proximo->weight;
        float prob = std::exp((float)delta_e / (t - i));

#ifdef DEBUG
        cout << "prob " << prob << endl;
#endif

        if (delta_e > 0 || (double)rand() / RAND_MAX <= prob)
        {
#ifdef DEBUG
            cout << "delta e " << delta_e << endl;
#endif

            atualS.id = proximo->id;
            atualS.weight = proximo->weight;
            visitados[proximo->id] = true;
            tam_caminho += atualS.weight;

            aux = caminho;
            while (aux->next != nullptr)
            {
                aux = aux->next;
            }

            aux->next = new Node(proximo->id, proximo->weight, nullptr);
            cidades_visitadas++;
        }

#ifdef DEBUG
        cout << endl;
#endif

        if (cidades_visitadas == qtd_cidades)
        {
            *erro = 0;
            return tam_caminho;
        }
    }

    *erro = 1;
    return tam_caminho;
}

void Agente::imprime_caminho()
{
    Node *aux;
    int tam = 0;

#ifdef DEBUG
    cout << "[ ";
    for (int k = 0; k < qtd_cidades; k++)
        cout << visitados[k] << " ";
    cout << "]" << endl;
#endif
    for (aux = caminho; aux != nullptr; aux = aux->next)
    {
        cout << aux->id << " (w: " << aux->weight << ") -> ";
        tam += aux->weight;
    }
    cout << endl;

    cout << "tamanho do caminho: " << tam << endl;
}

void Agente::analise_tempera()
{
    int min = 1215752191, max = 0, result;
    int erro, erros = 0;
    float med = 0;

    int rodadas = 1'000'00;
    for (int i = 0; i < rodadas; i++)
    {
        result = this->tempera_simulada(&erro);
        if (result < min)
            min = result;
        if (result > max)
            max = result;
        if (erro)
            erros++;
        med += result;
        this->reset();
        delete this->caminho;
    }

    med /= rodadas;
    cout
        << "RESULTADOS DA TÊMPERA SIMULADA COM " << rodadas
        << " EXECUÇÕES, NUM GRAFO COM " << this->qtd_cidades
        << " VÉRTICES : " << endl;
    cout
        << "\tminimo: " << min
        << ", média: " << med
        << ", máximo: " << max
        << ", erros: " << erros
        << endl;
}

int Agente::setup_algoritmo_genetico(int *erro)
{
    int tam_pop = 8;
    int erro_ = 0;
    Node *aux;
    int *custo_caminhos = new int[tam_pop];
    int **cromossomo = new int *[tam_pop];

    for (int i = 0; i < tam_pop; i++)
    {
        custo_caminhos[i] = this->tempera_simulada(&erro_);

        if (erro_ == 1)
        {
            i--;
            continue;
        }

        aux = this->caminho;

        cromossomo[i] = new int[this->qtd_cidades];
        for (int j = 0; j < this->qtd_cidades; j++)
        {
            cromossomo[i][j] = aux->id;
            aux = aux->next;
        }

        this->reset();
    }

    int result = this->algoritmo_genetico(cromossomo, tam_pop, custo_caminhos);
    // População construída, faltam:
    // Tratamento de crossover
    // Tratamento de mutações

    return 0;
}

int Agente::funcao_adaptacao(int *custo_caminhos, int tamanho_populacao)
{
    float *probs_provisorias = new float[tamanho_populacao];
    float *probs_normalizadas = new float[tamanho_populacao];
    int sum_caminhos = 0;
    float sum_probs = 0;
    float roleta = (float)rand() / RAND_MAX;

    for (int i = 0; i < tamanho_populacao; i++)
        sum_caminhos += custo_caminhos[i];

    for (int i = 0; i < tamanho_populacao; i++)
        probs_provisorias[i] = (sum_caminhos - custo_caminhos[i]) / (float)sum_caminhos;

    for (int i = 0; i < tamanho_populacao; i++)
        sum_probs += probs_provisorias[i];

    for (int i = 0; i < tamanho_populacao; i++)
        probs_normalizadas[i] = probs_provisorias[i] / sum_probs;

    float anterior = 0;
    for (int i = 0; i < tamanho_populacao; i++)
    {
        if (roleta >= anterior && roleta <= anterior + probs_normalizadas[i])
            return i;
        anterior += probs_normalizadas[i];
    }

    return -1;
}

int Agente::algoritmo_genetico(int **populacao, int tamanho_populacao, int *custo_caminhos)
{
    for (int i = 0; i < tamanho_populacao; i++)
    {
        cout << this->funcao_adaptacao(custo_caminhos, tamanho_populacao) << endl;
    }

    return 0;
}

void Agente::analise_genetico()
{
    // int min = 1215752191, max = 0, result;
    // int erro, erros = 0;
    // float med = 0;

    // int rodadas = 10'000;
    // for (int i = 0; i < rodadas; i++)
    // {
    //     result = this->algoritmo_genetico(&erro);
    //     if (result < min)
    //         min = result;
    //     if (result > max)
    //         max = result;
    //     if (erro)
    //         erros++;
    //     med += result;
    //     this->reset();
    //     delete this->caminho;
    // }

    // med /= rodadas;
    // cout
    //     << "RESULTADOS DO ALGORITMO GENÉTICO COM " << rodadas
    //     << " EXECUÇÕES, NUM GRAFO COM " << this->qtd_cidades
    //     << " VÉRTICES : " << endl;
    // cout
    //     << "\tminimo: " << min
    //     << ", média: " << med
    //     << ", máximo: " << max
    //     << endl;
}