#include <iostream>
#include <string>
#include <stdlib.h>
#include "ccgraphs.hpp"
#include <vector>
void depth_search(Graph &graph)
{
    std::string name;
    std::cout << "Insira o nome do vértice para a procura: ";
    std::cin >> name;
    std::vector<std::string> path = graph.depth_search(name);
    std::system("clear");
    std::cout << ">> [ ";
    for (std::string vertex : path)
    {
        std::cout << vertex << ' ';
    }
    std::cout << "] <<\n\n";
}

void create_connection(Graph &graph)
{
    std::string a, b;
    std::cout << "Insira o nome do vértice A: ";
    std::cin >> a;
    std::cout << "Insira o nome do vértice B: ";
    std::cin >> b;
    graph.add_connection(a, b);
    std::system("clear");
    std::cout << ">> Aresta criada <<\n\n";
}

void create_vertex(Graph &graph)
{
    std::string name;
    int x, y;
    std::cout << "Insira rótulo do vértice: ";
    std::cin >> name;
    std::cout << "Insira posição x e y do vértice: ";
    std::cin >> x >> y;
    graph.add_vertex(name, x, y);
    std::system("clear");
    std::cout << ">> Vértice criado <<\n\n";
}

void remove_vertex(Graph &graph)
{
    std::string name;
    std::cout << "Insira o rótulo do vértice a ser removido: ";
    std::cin >> name;
    graph.remove_vertex(name);
    std::system("clear");
    std::cout << ">> Vértice removido <<\n\n";
}

void remove_connection(Graph &graph)
{
    std::string a, b;
    std::cout << "Insira o nome do vértice A: ";
    std::cin >> a;
    std::cout << "Insira o nome do vértice B: ";
    std::cin >> b;
    graph.remove_connection(a, b);
    std::system("clear");
    std::cout <<">> Aresta removida <<\n\n";
}

void get_eulerian(Graph &graph)
{
    std::string name;
    std::cout << "Insira o vértice inicial do caminho euleriano: ";
    std::cin >> name;
    std::vector<std::string> path = graph.get_eulerian(name);
    std::system("clear");
    std::cout << ">> Caminho Euleriano: [ ";
    for (std::string vertex : path)
    {
        std::cout << vertex << ' ';
    }
    std::cout << "] <<\n\n";
}

int main()
{
    int option = 0;
    Graph graph;
    std::system("clear");
    std::cout << ">> Grafo vazio inicializado <<\n\n";
    while (option != 7)
    {
        // Graph Representation
        std::cout << "=== Grafos C++ ===\n\n";
        std::cout << "Componentes Conexos: " << graph.get_connected() << '\n';
        std::cout << "Euleriano: ";
        switch (graph.is_eulerian())
        {
            case (1): std::cout << "Sim\n"; break;
            case (2): std::cout << "Não (Semi-Euleriano)\n"; break;
            case (0): std::cout << "Não\n"; break;
        }
        std::cout << "Vértices: [ ";
        for (int i = 0; i < graph.all().size(); i++)
        {
            std::cout << graph.all()[i] << '(' << graph.get_vertex(graph.all()[i]).x << ',' << graph.get_vertex(graph.all()[i]).y << ") ";
        }
        std::cout << "]\nConexões:\n";
        for (int i = 0; i < graph.all().size(); i++)
        {
            std::cout << graph.all()[i] << ": [ ";
            std::vector<std::string> connections = graph.get_connections(graph.all()[i]);
            for (int j = 0; j < connections.size(); j++)
            {
                int distance = graph.distance(connections[j], graph.all()[i]);
                std::cout << connections[j] << " (" << distance << "), ";
            }
            std::cout << " ]\n";
        }
        // Menu
        std::cout << "\nDigite o número de uma opção:\n";
        std::cout << "1. Adicionar vértice.\n";
        std::cout << "2. Remover vértice.\n";
        std::cout << "3. Criar aresta.\n";
        std::cout << "4. Remover aresta.\n";
        std::cout << "5. Criar Caminho Euleriano.\n";
        std::cout << "6. Procura em Profundidade.\n";
        std::cout << "7. Sair.\n";
        std::cin >> option;
        // Menu Options Handling
        switch (option)
        {
        case (1):
            create_vertex(graph);
            break;
        case (2):
            remove_vertex(graph);
            break;
        case (3):
            create_connection(graph);
            break;
        case (4):
            remove_connection(graph);
            break;
        case (5):
            get_eulerian(graph);
            break;
        case (6):
            depth_search(graph);
            break;
        case (7):
            std::cout << "\n>> Saindo... <<\n\n";
            break;
        default:
            std::system("clear");
            std::cout << ">> Opção desconhecida <<\n\n";
            break;
        }
    }
}