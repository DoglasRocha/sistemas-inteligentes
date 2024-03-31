#include "../include/agente.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <bits/stdc++.h>

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

    this->caminho = nullptr;
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

    int rodadas = 10'000;
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
        << " VÉRTICES: " << endl;
    cout
        << "\tminimo: " << min
        << ", média: " << med
        << ", máximo: " << max
        << ", erros: " << erros
        << endl;
}

void Agente::preenche_vetor_aleatorio(int *vetor, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
        vetor[i] = i;

    random_shuffle(vetor, vetor + tamanho);
}

int Agente::computa_caminho_vetor(int *vetor, int tamanho)
{
    int caminho = 0;
    Node *aux;

    for (int i = 0; i < tamanho - 1; i++)
    {
        aux = this->ambiente->get_cidade(vetor[i]);

        while (aux->id != vetor[i + 1])
            aux = aux->next;

        caminho += aux->weight;
    }

    // final ate a origem
    aux = this->ambiente->get_cidade(vetor[tamanho - 1]);

    while (aux->id != vetor[0])
        aux = aux->next;
    caminho += aux->weight;

    return caminho;
}

int Agente::setup_algoritmo_genetico(int tamanho_populacao, int geracoes)
{
    int *custo_caminhos = new int[tamanho_populacao];
    int **cromossomo = new int *[tamanho_populacao];

    for (int i = 0; i < tamanho_populacao; i++)
    {
        cromossomo[i] = new int[this->qtd_cidades];
        preenche_vetor_aleatorio(cromossomo[i], this->qtd_cidades);
        custo_caminhos[i] = computa_caminho_vetor(cromossomo[i], this->qtd_cidades);
    }

    int result = this->algoritmo_genetico(cromossomo, tamanho_populacao, custo_caminhos, geracoes);

    return result;
}

int Agente::funcao_adaptacao(int *custo_caminhos, int tamanho_populacao)
{
    double *probs = new double[tamanho_populacao];
    int menor = 999999999;
    double sum_caminhos = 0;
    float roleta = (float)rand() / RAND_MAX;

    for (int i = 0; i < tamanho_populacao; i++)
        if (custo_caminhos[i] < menor)
            menor = custo_caminhos[i];

    for (int i = 0; i < tamanho_populacao; i++)
        sum_caminhos += (double)1 / (custo_caminhos[i] - (menor * 0.95));

    for (int i = 0; i < tamanho_populacao; i++)
        probs[i] = ((double)1 / (custo_caminhos[i] - (menor * 0.95))) / sum_caminhos;

    float anterior = 0;
    for (int i = 0; i < tamanho_populacao; i++)
    {
        if (roleta >= anterior && roleta <= anterior + probs[i])
        {
            delete[] probs;
            return i;
        }
        anterior += probs[i];
    }

    delete[] probs;
    return -1;
}

int Agente::retorna_indice_mais_adaptado(int *custo_caminhos, int tamanho_populacao)
{
    double *probs = new double[tamanho_populacao];
    double sum_caminhos = 0;
    int index_maior = 0;

    for (int i = 0; i < tamanho_populacao; i++)
        sum_caminhos += (double)1 / custo_caminhos[i];

    for (int i = 0; i < tamanho_populacao; i++)
        probs[i] = ((double)1 / custo_caminhos[i]) / sum_caminhos;

    float anterior = 0;
    for (int i = 1; i < tamanho_populacao; i++)
    {
        if (probs[i] > probs[index_maior])
            index_maior = i;
    }

    delete[] probs;
    return index_maior;
}

int *Agente::reproduz(int *pai, int *mae, int tamanho_fita)
{
    int corte = 1 + rand() % (tamanho_fita - 2);
    int *filho = new int[tamanho_fita];
    bool *colocados = new bool[tamanho_fita];
    for (int i = 0; i < tamanho_fita; i++)
        colocados[i] = false;

    for (int i = 0; i < corte; i++)
    {
        filho[i] = pai[i];
        colocados[pai[i]] = true;
    }

    for (int i = 0; i < tamanho_fita && corte < tamanho_fita; i++)
    {
        if (colocados[mae[i]])
            continue;

        filho[corte] = mae[i];
        colocados[mae[i]] = true;
        corte++;
    }

    delete[] colocados;
    return filho;
}

void Agente::mutacao(int *fita, int tamanho_fita)
{
    int p1 = rand() % tamanho_fita;
    int p2 = rand() % tamanho_fita;
    int tmp = fita[p1];
    fita[p1] = fita[p2];
    fita[p2] = tmp;
}

int Agente::algoritmo_genetico(int **populacao, int tamanho_populacao, int *custo_caminhos, int geracoes)
{
    int **nova_populacao, *novos_caminhos;

    for (int k = 0; k < geracoes; k++)
    {
        nova_populacao = new int *[tamanho_populacao];
        novos_caminhos = new int[tamanho_populacao];
        int indice_pai, indice_mae;

        for (int i = 0; i < tamanho_populacao; i++)
        {
            indice_pai = this->funcao_adaptacao(custo_caminhos, tamanho_populacao);
            indice_mae = this->funcao_adaptacao(custo_caminhos, tamanho_populacao);
            nova_populacao[i] = this->reproduz(populacao[indice_pai], populacao[indice_mae], this->qtd_cidades);
            if ((float)rand() / RAND_MAX <= 0.10)
                this->mutacao(nova_populacao[i], this->qtd_cidades);

            novos_caminhos[i] = this->computa_caminho_vetor(nova_populacao[i], this->qtd_cidades);
        }

        delete[] custo_caminhos;
        for (int i = 0; i < tamanho_populacao; i++)
            delete[] populacao[i];
        delete[] populacao;
        populacao = nova_populacao;
        custo_caminhos = novos_caminhos;
    }

    int result = this->computa_caminho_vetor(
        populacao[this->retorna_indice_mais_adaptado(
            custo_caminhos, tamanho_populacao)],
        this->qtd_cidades);

    delete[] custo_caminhos;
    for (int i = 0; i < tamanho_populacao; i++)
        delete[] populacao[i];
    delete[] populacao;

    return result;
}

void Agente::analise_genetico()
{
    int min = 1215752191, max = 0, result;
    float med = 0;

    int rodadas = 1000;
    for (int i = 0; i < rodadas; i++)
    {
        result = this->setup_algoritmo_genetico(4, 100);
        if (result < min)
            min = result;
        if (result > max)
            max = result;
        med += result;
    }

    med /= rodadas;
    cout
        << "RESULTADOS DO ALGORITMO GENÉTICO COM " << rodadas
        << " EXECUÇÕES, NUM GRAFO COM " << this->qtd_cidades
        << " VÉRTICES: " << endl;
    cout
        << "\tminimo: " << min
        << ", média: " << med
        << ", máximo: " << max
        << endl;
}