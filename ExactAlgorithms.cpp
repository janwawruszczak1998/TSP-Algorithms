//
// Created by Jan on 2019-10-07.
//
#include "ExactAlgorithms.h"
//Dynamik
//
//

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
            for(int j = 0; j < n; ++j){                                         //patrzymy, do którego wierzchołka J możemy dojść z V
                if (!(bitMask & (1 << j))){                                     //jeśli wierzchołka J nie ma w trasie
                    dp[bitMask | (1 << j)][j] = std::min(dp[bitMask][v]+g->getMatrix()[v][j], dp[bitMask|(1 << j)][j]);    //jeżeli koszt dojścia do V i przejścia do J jest mniejszy od aktualnie najlepszego osiągnięcia J, to zaktualizuj
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

    //zwolnienie pamięci
    for(int i = 0; i < (1 << n); ++i){
        delete[] dp[i];
    }
    delete[] dp;

    std::cout << "Najtańszy cykl Hamiltona wyznaczona DP: "<< result << "\n";
    return;

}



void BF(Graph *g){
    std::list<int> l; l.push_back(0);
    int max_val = (1 << 30);
    int *best = &max_val;
    int result = BFSearch(g, best, l);
    std::cout << "Najtańszy cykl Hamiltona wyznaczona BF: " << result << "\n";
    return;
}

int BFSearch(Graph *g, int *best, std::list<int> &vertices){

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
            BFSearch(g, best, vertices);
            vertices.pop_back();
        }
        else continue;
    }
    return *best;
}
//algorytm B&B
//
//preprocessing podziału i ograniczeń
void BB(Graph *g){
    std::list<int> l; l.push_back(0); vector<int> v; v.resize(g->getRank(), 0); //lista na sciezki Hamiltona w grafie
    std::pair<std::list<int>, int> vertices = make_pair(l, 0);  //vector na zachlanna sciezke

    v[0] = 1;
    int greedyPathVal = greedyPath(g, 0, v, 0); //obliczenie zachlannej sciezki

    //ustalenie najlepszej losowej sciezki
    int randomPathVal = 1e9;
    for(int i = 0; i < g->getRank()*g->getRank(); ++i){
        randomPathVal = std::min(randomPathVal, randomPath(g->getRank(), g) );
    }

    int* minimalEdges = getMinimalEdges(g); //tablica najtanszych krawedzi z kazdego wierzcholka
    int lb = calculateLowerBound(minimalEdges, g->getRank());   //obliczenie dolnego ograniczenia
    int* lowerBound = &lb;

    //wybranie gornego ograniczenia z najlepszej sciezki losowej i zachlannej
    int *upperBound = &(randomPathVal < greedyPathVal ? randomPathVal : greedyPathVal);

    int result = BBSearch(g, lowerBound, upperBound, vertices, minimalEdges);
    std::cout << "Najtańszy cykl Hamiltona wyznaczona B&B: " << result << "\n";
    return;
}


int BBSearch(Graph *g, int* lowerBound, int* upperBound, std::pair<std::list<int>, int> &vertices, int* minimalEdges){

    //nie rozwazamy sciezek gorszych, niz oszacowanie gorne cyklu
    if(vertices.second > *upperBound) return 0;

    if(vertices.first.size() == g->getRank()){
        int result = calculateObjective(vertices.first, g);
        if(*upperBound > result)  *upperBound = result;
        return 0;
    }

    for(int i = 0; i < g->getRank(); ++i){
        bool exist = false;
        for(int j : vertices.first){
            if(j == i){
                exist = true;
                break;
            }
        }
        if(!exist){
            vertices.second += g->getMatrix()[vertices.first.back()][i];
            *lowerBound = *lowerBound - minimalEdges[i] + g->getMatrix()[vertices.first.back()][i];
            vertices.first.push_back(i);
            if( (*upperBound >= *lowerBound) && (vertices.second <= *upperBound))//jezeli przejście do wierzchołka nie przekresla szansy na otrzymanie lepszego cyklu Hamiltona
            {
                BBSearch(g, lowerBound, upperBound, vertices, minimalEdges);
            }
            vertices.first.pop_back();
            *lowerBound = *lowerBound + minimalEdges[i] - g->getMatrix()[vertices.first.back()][i];
            vertices.second -= g->getMatrix()[vertices.first.back()][i];
        }
        else continue;
    }
    return *upperBound;
}

int calculateObjective(std::list<int>& permutation, Graph *g){

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


int calculateLowerBound(int* minimalEdges, int size){
    int sum = 0;
    for(int i = 0; i < size; ++i){
        sum += minimalEdges[i];
    }

    return sum;
}

int* getMinimalEdges(Graph* g){
    int n = g->getRank();
    int* table = new int[n];
    for(int i = 0; i < n; ++i){
        int min = 1e9;
        for(int j = 0; j < n; ++j){
            if(i != j && min > g->getMatrix()[i][j]) min = g->getMatrix()[i][j];
        }
        table[i] = min;
    }
    return table;
}

int randomPath(int n, Graph* g){

    std::list<int> randomPerm;
    std::vector<int> v;
    for(int i = 0; i < n; ++i) v.push_back(i);
    std::random_shuffle(v.begin(), v.end());     //przetasowanie na vectorze
    for(int i = 0; i < n; ++i) randomPerm.push_back( v[i] );
    return calculateObjective(randomPerm, g);

}

int greedyPath(Graph* g, int row, vector<int> &visited, int result){
    if(row == g->getRank() - 1 ){
        result += g->getMatrix()[row][0];
        return result;
    }
    int minValInRow = 1e9; int nearestNeighbour = 0;
    for(int i = 0; i < g->getRank(); ++i){
        if(visited[i]) continue;
        if(minValInRow > g->getMatrix()[row][i] && i != row){
            minValInRow = g->getMatrix()[row][i];
            nearestNeighbour = i;
            visited[i] = 1;
        }
    }
    result += g->getMatrix()[row][nearestNeighbour];

    return greedyPath(g, ++row, visited, result);

}