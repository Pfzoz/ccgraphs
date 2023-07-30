#ifndef CCGRAPHS
#define CCGRAPHS

#include <math.h>
#include <vector>
#include <string>
#include "dictionary.hpp"

struct Vertex
{
    int x, y;
};

int euclid_distance(Vertex a, Vertex b)
{
    return int(std::sqrt(std::pow(double(a.x) - double(b.x), 2) + std::pow(double(a.y) - double(b.y), 2)));
}

class Graph
{
private:
    Dictionary<Vertex> vertices;
    Dictionary<std::vector<std::string> *> connections;

    void _is_connected(std::string current, std::vector<std::string> &memory)
    {
        std::vector<std::string> adjacents = this->get_adjacents(current);
        for (std::string vertex : adjacents)
        {
            int exists = 0;
            for (std::string v : memory)
            {
                if (vertex == v) exists = 1;
            }
            if (!exists)
            {
                memory.push_back(vertex);
                this->_is_connected(vertex, memory);
            }
        }
    }

public:
    Graph(){};
    ~Graph(){};

    std::vector<std::string> all()
    {
        return this->vertices.keys;
    }

    std::vector<std::string> get_connections(std::string name)
    {
        return (*this->connections.get(name));
    }

    Vertex get_vertex(std::string name)
    {
        return this->vertices.get(name);
    }

    std::vector<std::string> get_adjacents(std::string name)
    {
        std::vector<std::string> adjacents;
        for (std::string vertex : *this->connections.get(name))
        {
            adjacents.push_back(vertex);
        }
        return adjacents;
    }

    void add_vertex(std::string name, int x, int y)
    {
        Vertex vertex;
        vertex.x = x;
        vertex.y = y;
        this->vertices.set(name, vertex);
        std::vector<std::string> *conns = new std::vector<std::string>{};
        this->connections.set(name, conns);
    }

    void remove_vertex(std::string name)
    {
        this->vertices.erase(name);
        for (std::string key : this->connections.keys)
        {
            for (int i = 0; i < (*this->connections.get(key)).size(); i++)
            {
                if ((*this->connections.get(key))[i] == name)
                {
                    this->connections.get(key)->erase(this->connections.get(key)->begin() + i);
                }
            }
        }
        // Guarantees clearing of memory
        std::vector<std::string> swapper;
        this->connections.get(name)->swap(swapper);
        //
        delete this->connections.get(name);
        this->connections.erase(name);
    }

    void add_connection(std::string a, std::string b)
    {
        Vertex x = this->vertices.get(a), y = this->vertices.get(b);
        this->connections.get(a)->push_back(b);
        this->connections.get(b)->push_back(a);
    }

    void remove_connection(std::string a, std::string b)
    {
        for (int i = 0; i < this->connections.get(a)->size(); i++)
        {
            if ((*this->connections.get(a))[i] == b)
            {
                this->connections.get(a)->erase(this->connections.get(a)->begin() + i);
                break;
            }
        }
        for (int i = 0; i < this->connections.get(b)->size(); i++)
        {
            if ((*this->connections.get(b))[i] == a)
            {
                this->connections.get(b)->erase(this->connections.get(b)->begin() + i);
                break;
            }
        }
    }

    int get_connected()
    {
        int connected = 0;
        std::vector<std::string> main_memory;
        while (main_memory.size() != this->vertices.size())
        {
            std::string name;
            for (std::string vertex : this->vertices.keys)
            {
                int found = 0;
                for (std::string v : main_memory)
                {
                    if (vertex == v) found = 1;
                }
                if (!found) name = vertex;
            }
            std::vector<std::string> memory;
            memory.push_back(name);
            this->_is_connected(name, memory);
            if (memory.size()) connected++;
            for (std::string vertex : memory)
            {
                int found = 0;
                for (std::string v : main_memory)
                {
                    if (v == vertex) found = 1;
                }
                if (!found) main_memory.push_back(vertex);
            }
        }
        return connected;
    }

    int is_connected()
    {
        return (this->get_connected() <= 1);
    }

    int is_eulerian()
    {
        if (this->vertices.size() == 0) return 1;
        if (!this->is_connected()) return 0;
        int odds = 0;
        for (std::string key : this->connections.keys)
        {
            if (this->connections.get(key)->size() % 2 != 0)
            {
                odds++;
            }
        }
        switch (odds)
        {
            case (0): return 1;
            case (2): return 2;
            default: return 0;
        }
    }

    int distance(std::string a, std::string b)
    {
        return euclid_distance(this->vertices.get(a), this->vertices.get(b));
    }

    Graph copy()
    {
        Graph graph;
        for (std::string key : this->vertices.keys)
        {
            graph.add_vertex(key, this->vertices.get(key).x, this->vertices.get(key).y);
        }
        for (std::string key : this->connections.keys)
        {
            for (std::string conn : (*this->connections.get(key)))
            {
                graph.connections.get(key)->push_back(conn);
            }
        }
        return graph;
    }

    std::vector<std::string> _get_eulerian(std::string current, std::vector<std::string> visited, Graph graph)
    {
        for (std::string adjacent : graph.get_adjacents(current))
        {
            
            Graph graph_copy = graph.copy();
            graph_copy.remove_connection(current, adjacent);
            graph_copy.trim();
            if (graph_copy.get_connected() != 0)
            {
                visited.push_back(adjacent);
                return this->_get_eulerian(adjacent, visited, graph_copy);
            }
        }
        return visited;
    }

    std::vector<std::string> get_eulerian(std::string name)
    {
        Graph graph = this->copy();
        std::vector<std::string> path;
        if (this->is_eulerian() != 1) return path;
        if (this->vertices.size() == 0) return path;
        path.push_back(name);
        return _get_eulerian(name, path, graph);
    }
    
    void _depth_search(std::vector<std::string> &visited, std::vector<std::string> &stack)
    {
        for (std::string vertex : this->get_adjacents(visited[visited.size()-1]))
        {
            int has = 0;
            for (std::string v : visited)
            {
                if (vertex == v) has = 1;
            }
            for (std::string v : stack)
            {
                if (vertex == v) has = 1;
            }
            if (!has) stack.push_back(vertex);
        }
        if (stack.size())
        {
            visited.push_back(stack[0]);
            stack.erase(stack.begin());
            this->_depth_search(visited, stack);
        }
    }

    std::vector<std::string> depth_search(std::string name)
    {
        std::vector<std::string> visited, stack;
        visited.push_back(name);
        this->_depth_search(visited, stack);
        return visited;
    }

    void trim()
    {
        for (std::string key : this->vertices.keys)
        {
            if (this->get_adjacents(key).size() == 0)
            {
                this->remove_vertex(key);
                this->trim();
                return;
            }
        }
    }
};

#endif