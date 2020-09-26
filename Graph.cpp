//
// Created by Jan on 2019-10-07.
//

#include <fstream>
#include <iostream>

#include "Graph.hpp"

Graph::Graph(std::string &&name) {

    std::ifstream in(name);

    if (!in) {
        std::cout << "Niepoprawna nazwa pliku!\n";
        return;
    }

    in >> name;
    in >> order;

    int cost{};
    adjacency_matrix.resize(order, std::vector<int>(order));

    for (int i = 0; i < order; ++i) {
        for (int j = 0; j < order; ++j) {
            in >> cost;
            if (i == j) adjacency_matrix[i][j] = 0;
            else adjacency_matrix[i][j] = cost;
        }
    }
    display();

}

Graph::Graph(const int order_)
        : order(order_) {

    int cost;
    adjacency_matrix.resize(order, std::vector<int>(order));

    for (int i = 0; i < order; ++i) {
        for (int j = 0; j < order; ++j) {
            std::cin >> cost;
            if (cost == -1) { std::cout << "-"; }
            else adjacency_matrix[i][j] = cost;
        }
    }
}


void Graph::display() {
    std::cout << "Order: " << order << "\n";
    for (int i = 0; i < order; ++i) {
        for (int j = 0; j < order; ++j)
            std::cout << get_adjacency_matrix()[i][j] << " ";
        std::cout << std::endl;
    }
    return;
}

int Graph::get_graph_order() {
    return order;
}


std::vector<std::vector<int>> &Graph::get_adjacency_matrix() {
    return adjacency_matrix;
}
