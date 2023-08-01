// Autores: Pedro Henrique Ferreira Zoz & Felipe Fonseca Rosa
// Autor deste código fonte: Pedro Henrique Ferreira Zoz
#include <iostream>
#include <string>
#include <stdlib.h>
#include "ccgraphs.hpp"
#include <vector>

void djikstra(Graph &graph)
{
    std::string name, dest;
    std::cout << "Insira o nome do vértice origem: ";
    std::cin >> name;
    std::cout << "Insira o nome do vértice destino: ";
    std::cin >> dest;
    std::system("clear");
    std::vector<std::string> search_result = graph.breadth_search(name);
    int has = 0;
    for (std::string nearby : search_result)
    {
        if (nearby == dest)
            has = 1;
    }
    if (has)
    {
        auto path = graph.djikstra(name, dest);
        std::cout << ">> Caminho Encontrado: [ ";
        for (std::string way : path)
        {
            std::cout << way << ' ';
        }
        std::cout << "] <<\n\n";
    }
    else
    {
        std::cout << ">> Caminho impossível. Destino é desconexo à origem <<\n\n";
    }
}

void load_graph(Graph &graph)
{
    std::string name;
    std::cout << "Insira o caminho de onde de carregar o grafo: ";
    std::cin >> name;
    graph.load(name);
    std::system("clear");
    std::cout << ">> Grafo carregado de " << name << " <<\n\n";
}

void save_graph(Graph &graph)
{
    std::string name;
    std::cout << "Insira o caminho a ser salvo o grafo: ";
    std::cin >> name;
    graph.save(name);
    std::system("clear");
    std::cout << ">> Grafo salvo em " << name << " <<\n\n";
}

void breadth_search(Graph &graph)
{
    std::string name;
    std::cout << "Insira o nome do vértice para a procura: ";
    std::cin >> name;
    std::vector<std::string> path = graph.breadth_search(name);
    std::system("clear");
    std::cout << ">> [ ";
    for (std::string vertex : path)
    {
        std::cout << vertex << ' ';
    }
    std::cout << "] <<\n\n";
}

void depth_search(Graph &graph)
{
    std::string name;
    std::cout << "Insira o nome do vértice para a procura: ";
    std::cin >> name;
    std::vector<std::string> path = graph.depth_search(name);
    // std::exit(1);
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
    std::cout << "Insira posição x e y do vértice -> '%x %y': ";
    std::cin >> x >> y;
    std::cin.clear();
    std::cin.ignore(1000, '\n');
    std::system("clear");
    if (graph.add_vertex(name, x, y))
    {
        std::cout << ">> Vértice criado <<\n\n";
    }
    else
    {
        std::cout << ">> Um vértice já existe sobre essas coordenadas! <<\n\n";
    };
}

void remove_vertex(Graph &graph)
{
    std::string name;
    std::cout << "Insira o rótulo do vértice a ser removido: ";
    std::cin >> name;
    std::system("clear");
    if (graph.remove_vertex(name))
    {
        std::cout << ">> Vértice removido <<\n\n";
    }
    else
    {
        std::cout << ">> Vértice não existente <<\n\n";
    }
}

void remove_connection(Graph &graph)
{
    std::string name;
    std::cout << "Insira o nome da aresta a ser removida: ";
    std::cin >> name;
    std::system("clear");
    if (graph.remove_connection_by_name(name))
    {
        std::cout << ">> Aresta removida <<\n\n";
    }
    else
    {
        std::cout << ">> Aresta não existente <<\n\n";
    }
}

void get_eulerian(Graph &graph)
{
    std::string name;
    std::cout << "Insira o vértice inicial do caminho euleriano: ";
    std::cin >> name;
    auto path = graph.get_eulerian(name);
    std::system("clear");
    if (graph.is_eulerian() != 1)
    {
        std::cout << ">> Não possível realizar a operação. (Grafo não é Euleriano) <<\n\n";
        return;
    }
    std::cout << ">> Caminho Euleriano: [ ";
    for (std::string vertex : path)
    {
        std::cout << vertex << ' ';
    }
    std::cout << "] <<\n\n";
}

int main()
{
    int option = -1;
    Graph graph;
    std::system("clear");
    std::cout << ">> Grafo vazio inicializado <<\n\n";
    while (option != 0)
    {
        // Graph Representation
        std::cout << "=== Grafos C++ ===\n\n";
        std::cout << "Componentes Conexos: " << graph.get_connected() << '\n';
        std::cout << "Euleriano: ";
        switch (graph.is_eulerian())
        {
        case (1):
            std::cout << "Sim\n";
            break;
        case (2):
            std::cout << "Não (Semi-Euleriano)\n";
            break;
        case (0):
            std::cout << "Não\n";
            break;
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
            std::vector<std::string> connections_names = graph.get_connections_names(graph.all()[i]);
            std::vector<std::string> connections = graph.get_connections(graph.all()[i]);
            for (int j = 0; j < connections.size(); j++)
            {
                int distance = graph.distance(connections[j], graph.all()[i]);
                std::cout << connections_names[j] << " (" << distance << "), ";
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
        std::cout << "7. Procura em Largura.\n";
        std::cout << "8. Salvar grafo.\n";
        std::cout << "9. Carregar grafo.\n";
        std::cout << "10. Realizar Djikstra.\n";
        std::cout << "0. Sair.\n";
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
            breadth_search(graph);
            break;
        case (8):
            save_graph(graph);
            break;
        case (9):
            load_graph(graph);
            break;
        case (10):
            djikstra(graph);
            break;
        case (0):
            std::cout << "\n>> Saindo... <<\n\n";
            break;
        default:
            std::system("clear");
            std::cout << ">> Opção desconhecida <<\n\n";
            break;
        }
    }
}