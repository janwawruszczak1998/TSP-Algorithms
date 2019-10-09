//
// Created by Jan on 2019-10-07.
//
#include "ExactAlgorithms.h"

int treeSearch(Graph *pGraph, int *pInt, list<int> list);

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
        if(bitMask % 10000 == 0) printf("%d\n", bitMask);
        for(int v = 0; v < n; ++v){                                             //rozważamy trasy zakończone w wierzchołku V
            if( !(bitMask & (1 << v) ) ) continue;                              //jeżeli w ścieżce nie ma w ogóle wierzchołja V, to pomijamy
            for(int j = 0; j < n; ++j) {                                        //patrzymy, z którego wierzchołka J najlepiej dojść do wierzchołka V
                if (!(bitMask & (1 << j)) && ((dp[bitMask | (1 << j)][j]) > dp[bitMask][v] + g->getMatrix()[v][j])) {   //warunki: mamy w trasie wierzchołek J i dojśćie do J i przejście do V jest tańsze niż aktualne dojście do V
                    dp[bitMask | (1 << j)][j] = (dp[bitMask][v] + g->getMatrix()[v][j]); //jeżeli koszt dojścia do J i przejścia do V jest mniejszy niż aktualnie najlepszego osiągnięcia V jest mniejszy, to zaktualizuj
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
    std::cout << "Najtańsza ścieżka Hamiltona wyznaczona DP: "<< result << "\n";
    return;

}

void BF(Graph *g){
    std::list<int> l; l.push_back(0);
    int max_val = (1 << 30);
    int *best = &max_val;
    int resoult = treeSearch(g, best, l);
    std::cout << "Najtańsza ścieżka Hamiltona wyznaczona BF: " << resoult << "\n";
    return;
}


void BB(Graph *g){
    std::cout << "BB";
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
    //std::cout << "rozmiar: " << vertices.size() << "\n";
    if(permutation.size() == g->getRank()){
        int result = 0; int start = 0;
        while( !permutation.empty() ){
            result += g->getMatrix()[start][permutation.front()];
            start = permutation.front();
            permutation.pop_front();
        }
        result += g->getMatrix()[start][0];
        //std::cout << "rozwiazanie: " << result << "\n";
        return result;
    }
}