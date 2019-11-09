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

#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> para;

class Graph{
private:
    int n;
    vector <vector <int> > matrix;

public:
    Graph(string name);
    Graph(int n);
    ~Graph();

    void display();

    int getRank();
    vector< vector<int> >& getMatrix();


};

#endif //GRAPHS_GRAPH_H


#endif //PEA_GRAPH_H
