//
// Created by Jan on 2019-10-07.
//


#include "Graph.h"

Graph::Graph(int n){

    this->n = n;
    int  cost;
    matrix.resize(n + 1);
    for(int i = 0; i < n + 1; ++i){
        matrix[i].resize(n + 1);
    }
    for(int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j){
            std::cin >> cost;
            if(i == j) matrix[i][j] = 0;
            matrix[i][j] = cost;
        }
    }


}
Graph::~Graph(){

    for(int i = 0; i < matrix.size(); ++i){
        matrix[i].clear();
    }
    matrix.clear();
}

int Graph::getRank() {
    return n;
}


vector< vector<int> > Graph::getMatrix(){
    return matrix;
}
