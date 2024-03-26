#include "../include/ambiente.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
using namespace std;

Ambiente::Ambiente()
{
    ifstream arquivoGrafo("grafo.txt"); // Primeira leitura, contagem de vértices
    int numVertices = 1;

    if (arquivoGrafo.is_open())
    {
        string aux;

        while (getline(arquivoGrafo, aux))
            ++numVertices; // Conta quantidade de linhas do arquivo para determinar a quantidade de vértices.
        // Cada linha contém pesos das arestas, sem incluir o próprio vértice ou arestas já adicionadas.

        // cout << numVertices << endl;

        mapa = new Grafo(numVertices);
    }

    arquivoGrafo.close();

    arquivoGrafo.open("grafo.txt"); // Segunda leitura, construção das arestas

    if (arquivoGrafo.is_open())
    {
        string buffer;
        int i = 0, j = 1;

        while (getline(arquivoGrafo, buffer))
        { // Pega linha inteira (NÃO ENTRA AQUI)

            char *token = strtok((char *)buffer.data(), ",");
            j = i + 1;
            while (token)
            { // Divide linha conforme virgulas

                const int cstr = atoi(token);
                mapa->insere_aresta(i, j, cstr);
                token = strtok(NULL, ",");
                j++;
            }
            i++;
        }
    }
}

// instancia manual de grafo
/*Ambiente::Ambiente()
{
    mapa = new Grafo(4);

    mapa->insere_aresta(0, 1, 20)->insere_aresta(0, 2, 42)->insere_aresta(0, 3, 35);

    mapa->insere_aresta(1, 2, 30)->insere_aresta(1, 3, 34);

    mapa->insere_aresta(2, 3, 12);
}*/

Ambiente::~Ambiente()
{
    delete mapa;
}

void Ambiente::imprime_mapa()
{
    cout << "Grafo: " << endl
         << endl;

    mapa->imprime();

    cout << endl
         << endl;
}

int Ambiente::get_qtd_cidades()
{
    return mapa->V;
}

int Ambiente::get_tam_mapa()
{
    return mapa->V + mapa->E;
}

Node *Ambiente::get_cidade(int id)
{
    return mapa->Adj[id];
}