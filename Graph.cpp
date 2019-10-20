//
// Created by Jan on 2019-10-07.
//


#include "Graph.h"

Graph::Graph(string name){

    ifstream in(name);

    if (!in) {
        cout << "Niepoprawna nazwa pliku!\n";
        return;
    }

    in >> name;
    in >> n;

    int  cost;
    matrix.resize(n);
    for(int i = 0; i < n; ++i){
        matrix[i].resize(n);
    }
    for(int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j){
            in >> cost;
            if(i == j) matrix[i][j] = 0;
            matrix[i][j] = cost;
        }
    }


}
Graph::Graph(int n){

    this->n = n;
    int  cost;
    matrix.resize(n);
    for(int i = 0; i < n; ++i){
        matrix[i].resize(n);
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

void Graph::display(){
    for(int i = 0; i < getRank(); ++i){
        for(int j = 0; j <getRank(); ++j)
            cout << getMatrix()[i][j] << " ";
        cout << "\n";
    }
    return;
}

int Graph::getRank() {
    return n;
}


vector< vector<int> > Graph::getMatrix(){
    return matrix;
}
