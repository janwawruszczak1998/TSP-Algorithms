//
// Created by Jan on 2019-10-07.
//

#ifndef PEA_GRAPH_H
#define PEA_GRAPH_H

//
// Created by Jan on 2019-05-15.
//

#ifndef GRAPHS_GRAPH_H
#define GRAPHS_GRAPH_H

#include <vector>
#include <string>


class Graph {
private:
    int order;
    std::vector<std::vector<int>> adjacency_matrix;

public:
    explicit Graph(std::string &&name);

    explicit Graph(int order_);

    void display();

    int get_graph_order();

    std::vector<std::vector<int>> &get_adjacency_matrix();


};

#endif //GRAPHS_GRAPH_H


#endif //PEA_GRAPH_H
