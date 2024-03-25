#include "../include/agente.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

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
    delete[] visitados;
}

void Agente::computa_qtd_vizinhos_e_media(Node *cidade, int *qtd_vizinhos, int *media)
{
    *qtd_vizinhos = *media = 0;

    for (Node *aux = cidade; aux != nullptr; aux = aux->next)
    {
        (*qtd_vizinhos)++;
        *media += aux->weight;
    }

    *media /= *qtd_vizinhos;
}

int Agente::escolhe_aleatorio(int limite)
{
    return rand() % limite;
}

void Agente::andar()
{
    Node *proximo, *atual;
    int n_atual = this->escolhe_aleatorio(this->qtd_cidades);
    int origem = n_atual;
    atual->id = n_atual;
    caminho = new Node(n_atual, 0, nullptr);
    int a;

    int t = 75;
    for (int i = 0; i < t; i++)
    {
        atual = ambiente->get_cidade(atual->id);
        // cin >> a;
        int qtd_vizinhos, media, n_next, n;
        this->computa_qtd_vizinhos_e_media(atual, &qtd_vizinhos, &media);

        do
        {
            Node *aux;
            int j;
            n_next = this->escolhe_aleatorio(qtd_vizinhos);
            for (j = 0, aux = atual; j < n_next; j++, aux = aux->next)
                ;

            n = aux->id;
            // cout << "[ ";
            // for (int k = 0; k < qtd_cidades; k++)
            //     cout << visitados[k] << " ";
            // cout << "]" << endl;

            // cout << "n " << n << endl;
            proximo = aux;
        } while (visitados[n] && cidades_visitadas < qtd_cidades);
        if (n == origem && cidades_visitadas < qtd_cidades - 1)
            continue;

        // proximo = atual;

        cout << "media " << media << endl;
        int delta_e = -(proximo->weight - media);
        float prob = std::exp((float)delta_e / (t - i));

        cout << "delta e " << delta_e << endl;
        cout << "prob " << prob << endl;
        if (delta_e > 0)
        {
            cout << "delta e positivo" << endl;
            atual = proximo;
            visitados[proximo->id] = true;

            Node *aux = caminho;
            while (aux->next != nullptr)
            {
                aux = aux->next;
            }

            aux->next = new Node(proximo->id, proximo->weight, nullptr);
            cout << "proximo " << proximo->id << endl;
            cidades_visitadas++;
        }
        else if ((double)rand() / RAND_MAX <= prob)
        {
            cout << "delta e negativo " << endl;
            atual = proximo;
            Node *aux = caminho;
            visitados[proximo->id] = true;
            while (aux->next != nullptr)
            {
                aux = aux->next;
            }

            aux->next = new Node(proximo->id, proximo->weight, nullptr);
            cout << "proximo " << proximo->id << endl;
            cidades_visitadas++;
        }

        if (cidades_visitadas == qtd_cidades)
            return;
    }
}

void Agente::imprime_caminho()
{
    Node *aux;
    int tam = 0;
    for (aux = caminho; aux != nullptr; aux = aux->next)
    {
        cout << aux->id << " (w: " << aux->weight << ") -> ";
        tam += aux->weight;
    }
    cout << endl;

    cout << "tamanho do caminho: " << tam << endl;
}