#ifndef CCGRAPHS
#define CCGRAPHS

#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "dictionary.hpp"

int counter = 0;
const int INFINITE = 0x3f3f3f3f;

int has(std::vector<std::string> v, std::string item)
{
    int exists = 0;
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == item)
            exists = 1;
    }
    return exists;
}

int get_min(std::vector<int> v)
{
    int min = v[0];
    int ret = 0;
    for (int i = 1; i < v.size(); i++)
    {
        if (v[i] < min)
        {
            min = v[i];
            ret = i;
        }
    }
    return ret;
}

struct Vertex
{
    int x, y;
};
// A B C D E
int euclid_distance(Vertex a, Vertex b)
{
    return int(std::sqrt(std::pow(double(a.x) - double(b.x), 2) + std::pow(double(a.y) - double(b.y), 2)));
}

class Graph
{
private:
    Dictionary<Vertex> vertices;
    Dictionary<std::vector<std::string> *> connections;
    Dictionary<std::vector<std::string> *> connections_names;

    void _is_connected(std::string current, std::vector<std::string> &memory)
    {
        std::vector<std::string> adjacents = this->get_adjacents(current);
        for (std::string vertex : adjacents)
        {
            int exists = 0;
            for (std::string v : memory)
            {
                if (vertex == v)
                    exists = 1;
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

    std::vector<std::string> get_connections_names(std::string name)
    {
        return (*this->connections_names.get(name));
    }

    int remove_connection_by_name(std::string name)
    {
        int removed = 0;
        for (std::string key : this->vertices.keys)
        {
            for (int i = 0; i < (*this->connections_names.get(key)).size(); i++)
            {
                if ((*this->connections_names.get(key))[i] == name)
                {
                    std::string b;
                    b = (*this->connections.get(key))[i];
                    std::cout << "B:" << b << '\n';
                    std::cout << "Here1\n";
                    this->connections.get(key)->erase(this->connections.get(key)->begin() + i);
                    this->connections_names.get(key)->erase(this->connections_names.get(key)->begin() + i);
                    std::cout << "Here2\n";
                    this->connections.get(b)->erase(this->connections.get(b)->begin() + i);
                    this->connections_names.get(b)->erase(this->connections_names.get(b)->begin() + i);
                    removed = 1;
                    return removed;
                }
            }
            if (removed)
                return removed;
        }
        return removed;
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

    int add_vertex(std::string name, int x, int y)
    {
        for (Vertex v : this->vertices.values)
        {
            if (v.x == x && v.y == y)
            {
                return 0;
            }
        }
        Vertex vertex;
        vertex.x = x;
        vertex.y = y;
        this->vertices.set(name, vertex);
        std::vector<std::string> *conns = new std::vector<std::string>{};
        std::vector<std::string> *conns_names = new std::vector<std::string>{};
        this->connections.set(name, conns);
        this->connections_names.set(name, conns_names);
        return 1;
    }

    int remove_vertex(std::string name)
    {
        if (!has(vertices.keys, name))
        {
            return 0;
        }
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
        std::vector<std::string> extra_swapper;
        this->connections_names.get(name)->swap(extra_swapper);
        //
        delete this->connections.get(name);
        delete this->connections_names.get(name);
        this->connections.erase(name);
        this->connections_names.erase(name);
        return 1;
    }

    void add_connection(std::string a, std::string b)
    {
        Vertex x = this->vertices.get(a), y = this->vertices.get(b);
        this->connections.get(a)->push_back(b);
        this->connections.get(b)->push_back(a);
        this->connections_names.get(a)->push_back(a + b);
        this->connections_names.get(b)->push_back(a + b);
    }

    void remove_connection(std::string a, std::string b)
    {
        for (int i = 0; i < this->connections.get(a)->size(); i++)
        {
            if ((*this->connections.get(a))[i] == b)
            {
                this->connections.get(a)->erase(this->connections.get(a)->begin() + i);
                this->connections_names.get(a)->erase(this->connections_names.get(a)->begin() + i);
                break;
            }
        }
        for (int i = 0; i < this->connections.get(b)->size(); i++)
        {
            if ((*this->connections.get(b))[i] == a)
            {
                this->connections.get(b)->erase(this->connections.get(b)->begin() + i);
                this->connections_names.get(b)->erase(this->connections_names.get(b)->begin() + i);
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
                    if (vertex == v)
                        found = 1;
                }
                if (!found)
                    name = vertex;
            }
            std::vector<std::string> memory;
            memory.push_back(name);
            this->_is_connected(name, memory);
            if (memory.size())
                connected++;
            for (std::string vertex : memory)
            {
                int found = 0;
                for (std::string v : main_memory)
                {
                    if (v == vertex)
                        found = 1;
                }
                if (!found)
                    main_memory.push_back(vertex);
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
        if (this->vertices.size() == 0)
            return 1;
        if (!this->is_connected())
            return 0;
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
        case (0):
            return 1;
        case (2):
            return 2;
        default:
            return 0;
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
        for (std::string key : this->connections_names.keys)
        {
            for (std::string conn : (*this->connections_names.get(key)))
            {
                graph.connections_names.get(key)->push_back(conn);
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
        if (this->is_eulerian() != 1)
            return path;
        if (this->vertices.size() == 0)
            return path;
        path.push_back(name);
        return _get_eulerian(name, path, graph);
    }

    void _depth_search(std::string current, std::vector<std::string> &visited, std::vector<std::string> markers)
    {
        auto adjacents = this->get_adjacents(current);
        visited.push_back(current);
        for (auto adjacent : adjacents)
        {
            if (!has(visited, adjacent) && !has(markers, adjacent))
            {
                auto new_markers = markers;
                for (auto x : adjacents)
                {
                    if (x != adjacent) new_markers.push_back(x);
                }
                _depth_search(adjacent, visited, new_markers);
            }
        }
    }

    std::vector<std::string> depth_search(std::string name)
    {
        std::vector<std::string> visited, stack;
        this->_depth_search(name, visited, std::vector<std::string>{});
        return visited;
    }

    void _breadth_search(std::vector<std::string> &visited, std::vector<std::string> &queue)
    {
        for (std::string vertex : this->get_adjacents(queue[0]))
        {
            int has = 0;
            for (std::string v : visited)
            {
                if (v == vertex)
                    has = 1;
            }
            if (!has)
            {
                std::cout << "pushei " << vertex << '\n';
                visited.push_back(vertex);
                queue.push_back(vertex);
            }
        }
        queue.erase(queue.begin());
        if (queue.size() != 0)
        {
            this->_breadth_search(visited, queue);
        }
    }

    std::vector<std::string> breadth_search(std::string name)
    {
        std::vector<std::string> visited, queue;
        queue.push_back(name);
        visited.push_back(name);
        this->_breadth_search(visited, queue);
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

    // nome_vértice x y con1 con2 con3 con4...

    void save(std::string path)
    {
        std::ofstream save_file(path);
        for (std::string key : this->vertices.keys)
        {
            save_file << key << ' ' << this->get_vertex(key).x << ' ' << this->get_vertex(key).y << ' ';
            for (std::string conn : (*this->connections.get(key)))
            {
                save_file << conn << ' ';
            }
            save_file << '\n';
        }
        save_file.close();
    }

    void clear()
    {
        auto keys = this->vertices.keys;
        for (std::string vertex : keys)
        {
            this->remove_vertex(vertex);
        }
    }

    void load(std::string path)
    {
        std::vector<std::vector<std::string>> connections;
        this->clear();
        std::ifstream load_file(path);
        std::stringstream my_stream;
        std::string buffer, vertex_name;
        int x, y;
        while (!load_file.eof())
        {
            std::vector<std::string> conns;
            std::getline(load_file, buffer);
            if (buffer == "")
                break;
            my_stream.str(buffer);
            my_stream >> buffer;
            vertex_name = buffer;
            my_stream >> buffer;
            x = std::atoi(buffer.c_str());
            my_stream >> buffer;
            y = std::atoi(buffer.c_str());
            this->add_vertex(vertex_name, x, y);
            std::string conn;
            while (my_stream >> conn)
            {
                conns.push_back(conn);
            }
            connections.push_back(conns);
            my_stream.clear();
        }
        for (int i = 0; i < connections.size(); i++)
        {
            for (int j = 0; j < connections[i].size(); j++)
            {
                this->add_connection(this->vertices.keys[i], connections[i][j]);
                for (int k = 0; k < this->vertices.keys.size(); k++)
                {
                    if (this->vertices.keys[k] == connections[i][j])
                    {
                        for (int o = 0; o < connections[k].size(); o++)
                        {
                            if (connections[k][o] == this->vertices.keys[i])
                            {
                                connections[k].erase(connections[k].begin() + o);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
        load_file.close();
    }

    // void find_shortest_path(std::vector<std::string> &path, )

    std::vector<std::string> djikstra(std::string name, std::string dest)
    {
        std::string origin = name;
        Dictionary<int> visited;
        Dictionary<int> costs;
        for (std::string key : this->vertices.keys)
        {
            costs.set(key, INFINITE);
            visited.set(key, 0);
        }
        costs.set(name, 0);
        visited.set(name, 1);
        int not_visited = 1;
        while (not_visited)
        {
            std::cout << "Current name: " << name << '\n';
            auto adjacents = this->get_adjacents(name);
            // std::string lesser = adjacents[0];
            for (std::string adjacent : adjacents)
            {
                if (!visited.get(adjacent))
                {
                    // if (this->distance(name, adjacent) + costs.get(name) < this->distance(name, lesser) + costs.get(name))
                    // {
                    //     lesser = adjacent;
                    //     visited.set(lesser, 1);
                    // }

                    if (this->distance(name, adjacent) + costs.get(name) < costs.get(adjacent))
                    {
                        costs.set(adjacent, this->distance(name, adjacent) + costs.get(name));
                    }
                }
            }
            Dictionary<int> decider = costs;
            for (int i = 0; i < visited.values.size(); i++)
            {
                if (visited.values[i])
                {
                    decider.set(decider.keys[i], INFINITE);
                }
            }
            for (std::string key : visited.keys)
            {
                std::cout << "Visited " << key << ":" << visited.get(key) << '\n';
            }
            int minimum = get_min(decider.values);
            name = decider.keys[minimum];
            visited.set(name, 1);
            if (name == dest)
                break;
            not_visited = 0;
            for (int x : visited.values)
            {
                if (x == 0)
                    not_visited = 1;
            }
            counter++;
            // if (counter == 3) break;
        }
        for (std::string key : costs.keys)
        {
            std::cout << "Key " << key << ": " << costs.get(key) << '\n';
        }
        std::string current_node = dest;
        std::vector<std::string> path;
        path.push_back(dest);
        while (current_node != origin)
        {
            auto adjacents = this->get_adjacents(current_node);
            std::vector<std::string> competitors_labels;
            std::vector<int> competitors;
            for (std::string adjacent : adjacents)
            {
                if (has(costs.keys, adjacent))
                {
                    competitors_labels.push_back(adjacent);
                    competitors.push_back(costs.get(adjacent));
                }
            }
            int winnern = get_min(competitors);
            std::string winner = competitors_labels[winnern];
            path.insert(path.begin(), winner);
            current_node = winner;
        }
        return path;
    }
};

#endif