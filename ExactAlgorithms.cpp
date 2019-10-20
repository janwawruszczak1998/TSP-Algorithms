//
// Created by Jan on 2019-10-07.
//
#include "ExactAlgorithms.h"


void DP(Graph *g){

    //preprocessing
    int n = g->getRank();
    int ** dp;
    dp = new int* [1 << n];
    for(int i = 0; i < (1 << n); ++i) dp[i] = new int [n];

    for(int i = 0; i < (1 << n); ++i){
        for(int j = 0; j < n; ++j){
            dp[i][j] = 1 << 30;	//wypełnienie tablicy tras nieskończonościami
        }
    }

    for(int i = 0; i < n; ++i) {
        dp[(1 << i) | 1][i] = g->getMatrix()[0][i]; //na początku koszt jednokrawędziowej ścieżki Hamiltona z 0 do I to po prostu koszt krawędzi
    }

    //dynamic programming
    for(int bitMask = 0; bitMask < (1 << n); ++bitMask){	                    //dla każdej maski bitowej liczymy trasy
        for(int v = 0; v < n; ++v){                                             //rozważamy trasy zakończone w wierzchołku V
            if( !(bitMask & (1 << v) ) ) continue;                              //jeżeli w ścieżce nie ma w ogóle wierzchołja V, to pomijamy
            for(int j = 0; j < n; ++j) {                                        //patrzymy, z którego wierzchołka J najlepiej dojść do wierzchołka V
                if (!(bitMask & (1 << j))) {   //warunki: mamy w trasie wierzchołek J i dojśćie do J i przejście do V jest tańsze niż aktualne dojście do V
                    dp[bitMask | (1 << j)][j] = min(dp[bitMask][v]+g->getMatrix()[v][j], dp[bitMask|(1<<j)][j]);    //jeżeli koszt dojścia do J i przejścia do V jest mniejszy niż aktualnie najlepszego osiągnięcia V jest mniejszy, to zaktualizuj
                }
            }
        }
    }

    //ustalenie resultu
    int result = 1e9;
    for(int v = 0; v < n; ++v){
        int act = dp[(1 << n) - 1][v] + g->getMatrix()[v][0]; //koszt "powrotu" z wierzchołka v do wierzchołka 0
        if(result > act) result = act;
    }
    std::cout << "Najtańszy cykl Hamiltona wyznaczona DP: "<< result << "\n";
    return;

}

void BF(Graph *g){
    std::list<int> l; l.push_back(0);
    int max_val = (1 << 30);
    int *best = &max_val;
    int resoult = treeSearch(g, best, l);
    std::cout << "Najtańszy cykl Hamiltona wyznaczona BF: " << resoult << "\n";
    return;
}


void BB(Graph *g){
    std::list<int> l;
    int max_val = (1 << 30);
    int *best = &max_val;
    int resoult = treeSearch(g, best, l);
    std::cout << "Najtańszy cykl Hamiltona wyznaczona BF: " << resoult << "\n";
    return;
}


int treeSearch(Graph *g, int *best, std::list<int> vertices){

    if(vertices.size() == g->getRank()){
        int result = calculateObjective(vertices, g);
        if(*best > result) *best = result;
        return 0;
    }

    for(int i = 0; i < g->getRank(); ++i){
        bool exist = false;
        for(int j : vertices){
            if(j == i){
                exist = true;
                break;
            }
        }
        if(!exist){
            vertices.push_back(i);
            treeSearch(g, best, vertices);
            vertices.pop_back();
        }
        else continue;
    }
    return *best;
}

int calculateObjective(std::list<int> permutation, Graph *g){

    int result = 0; int start = permutation.front(); int act = permutation.front(); int next;
    while( !permutation.empty() ){
        permutation.pop_front();
        next = permutation.front();
        result += g->getMatrix()[act][next];
        act = next;

    }
    result += g->getMatrix()[act][start];
    return result;

}