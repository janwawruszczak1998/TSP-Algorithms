//
// Created by Jan on 2019-10-07.
//
#include "ExactAlgorithms.hpp"

#include <algorithm>
#include <iostream>
#include <random>
#include <set>


//Dynamik
//
//

void DP(std::unique_ptr<Graph> &g) {

    //preprocessing
    int n = g->get_graph_order();
    int **dp;
    dp = new int *[1 << n];
    for (int i = 0; i < (1 << n); ++i) dp[i] = new int[n];

    for (int i = 0; i < (1 << n); ++i) {
        for (int j = 0; j < n; ++j) {
            dp[i][j] = 1 << 30;    //wypełnienie tablicy tras nieskończonościami
        }
    }


    for (int i = 0; i < n; ++i) {
        dp[(1 << i) |
           1][i] = g->get_adjacency_matrix()[0][i]; //na początku koszt jednokrawędziowej ścieżki Hamiltona z 0 do I to po prostu koszt krawędzi
    }

    //dynamic programming
    for (int bitMask = 0; bitMask < (1 << n); ++bitMask) {                        //dla każdej maski bitowej liczymy trasy
        for (int v = 0; v < n; ++v) {                                             //rozważamy trasy zakończone w wierzchołku V
            if (!(bitMask & (1 << v))) {
                continue;                                                          //jeżeli w ścieżce nie ma w ogóle wierzchołja V, to pomijamy
            }
            for (int j = 0; j < n; ++j) {                                         //patrzymy, do którego wierzchołka J możemy dojść z V
                if (!(bitMask & (1 << j))) {                                     //jeśli wierzchołka J nie ma w trasie
                    dp[bitMask | (1 << j)][j]
                        = std::min(dp[bitMask][v] + g->get_adjacency_matrix()[v][j], dp[bitMask |(1 << j)][j]);    //jeżeli koszt dojścia do V i przejścia do J jest mniejszy od aktualnie najlepszego osiągnięcia J, to zaktualizuj
                }
            }
        }
    }

    //ustalenie resultu
    int result = 1e9;
    for (int v = 0; v < n; ++v) {
        int act = dp[(1 << n) - 1][v] +
                  g->get_adjacency_matrix()[v][0]; //koszt "powrotu" z wierzchołka v do wierzchołka 0
        if (result > act) result = act;
    }

    //zwolnienie pamięci
    for (int i = 0; i < (1 << n); ++i) {
        delete[] dp[i];
    }
    delete[] dp;

    std::cout << "Najtańszy cykl Hamiltona wyznaczona DP: " << result << "\n";
}


void BF(std::unique_ptr<Graph> &g) {
    std::list<int> l{};
    int *best = new int{1 << 30};
    BF_search(g, best, l);
    std::cout << "Najtańszy cykl Hamiltona wyznaczona BF: " << *best << "\n";
}

void BF_search(std::unique_ptr<Graph> &g, int *best, std::list<int> &vertices) {

    if (vertices.size() == g->get_graph_order()) {
        int result = calculate_objective(vertices, g);
        if (*best > result) *best = result;
        return;
    }

    for(int i = 0; i < g->get_graph_order(); ++i){
        bool exist = false;
        for(auto element : vertices){
            if(element == i){
                exist = true;
                break;
            }
        }
        if(!exist){
            vertices.push_back(i);
            BF_search(g, best, vertices);
            vertices.pop_back();
        }
    }

}

//algorytm B&B
//
//preprocessing podziału i ograniczeń
void BB(std::unique_ptr<Graph> &g) {
    std::list<int> l;
    vector<int> v(g->get_graph_order(), 0); //lista na sciezki Hamiltona w grafie
    std::pair<std::list<int>, int> vertices = make_pair(l, 0);  //vector na zachlanna sciezke

    v[0] = 1;
    int greedy_pathVal = greedy_path(g, 0, v, 0); //obliczenie zachlannej sciezki

    //ustalenie najlepszej losowej sciezki
    int random_pathVal = 1e9;
    for (int i = 0; i < g->get_graph_order() * g->get_graph_order(); ++i) {
        random_pathVal = std::min(random_pathVal, random_path(g->get_graph_order(), g));
    }

    int *minimalEdges = get_minimal_edges(g); //tablica najtanszych krawedzi z kazdego wierzcholka
    int lowerBound = calculate_lower_bound(minimalEdges, g->get_graph_order());   //obliczenie dolnego ograniczenia

    //wybranie gornego ograniczenia z najlepszej sciezki losowej i zachlannej
    int upperBound = random_pathVal < greedy_pathVal ? random_pathVal : greedy_pathVal;

    int result = BB_search(g, &lowerBound, &upperBound, vertices, minimalEdges);
    std::cout << "Najtańszy cykl Hamiltona wyznaczona B&B: " << result << "\n";
}


int BB_search(std::unique_ptr<Graph> &g, int *lowerBound, int *upperBound, std::pair<std::list<int>, int> &vertices,
              int *minimalEdges) {

    //nie rozwazamy sciezek gorszych, niz oszacowanie gorne cyklu
    if (vertices.second > *upperBound) return 0;

    if (vertices.first.size() == g->get_graph_order()) {
        int result = calculate_objective(vertices.first, g);
        if (*upperBound > result) *upperBound = result;
        return 0;
    }

    for (int i = 0; i < g->get_graph_order(); ++i) {
        bool exist = false;
        for (int j : vertices.first) {
            if (j == i) {
                exist = true;
                break;
            }
        }
        if (!exist) {
            vertices.second += g->get_adjacency_matrix()[vertices.first.back()][i];
            *lowerBound = *lowerBound - minimalEdges[i] + g->get_adjacency_matrix()[vertices.first.back()][i];
            vertices.first.push_back(i);
            if ((*upperBound >= *lowerBound) && (vertices.second <=
                                                 *upperBound))//jezeli przejście do wierzchołka nie przekresla szansy na otrzymanie lepszego cyklu Hamiltona
            {
                BB_search(g, lowerBound, upperBound, vertices, minimalEdges);
            }
            vertices.first.pop_back();
            *lowerBound = *lowerBound + minimalEdges[i] - g->get_adjacency_matrix()[vertices.first.back()][i];
            vertices.second -= g->get_adjacency_matrix()[vertices.first.back()][i];
        } else continue;
    }
    return *upperBound;
}

template<typename Container>
int calculate_objective(Container permutation, std::unique_ptr<Graph> &g) {
    int result = 0, start = permutation.front(), act{}, next{};
    while( !permutation.empty() ){
        permutation.pop_front();
        next = permutation.front();
        result += g->get_adjacency_matrix()[act][next];
        act = next;
    }
    result += g->get_adjacency_matrix()[act][start];
    return result;

}


int calculate_lower_bound(const int *minimalEdges, int size) {
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += minimalEdges[i];
    }

    return sum;
}

int *get_minimal_edges(std::unique_ptr<Graph> &g) {
    int n = g->get_graph_order();
    int *table = new int[n];
    for (int i = 0; i < n; ++i) {
        int min = 1e9;
        for (int j = 0; j < n; ++j) {
            if (i != j && min > g->get_adjacency_matrix()[i][j]) min = g->get_adjacency_matrix()[i][j];
        }
        table[i] = min;
    }
    return table;
}

int random_path(int n, std::unique_ptr<Graph> &g) {

    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::list<int> randomPerm;
    std::vector<int> v(n);
    std::shuffle(v.begin(), v.end(), generator);     //przetasowanie na vectorze
    for (int i = 0; i < n; ++i) {
        randomPerm.push_back(v[i]);
    }
    return calculate_objective(randomPerm, g);

}

int greedy_path(std::unique_ptr<Graph> &g, int row, vector<int> &visited, int result) {
    if (row == g->get_graph_order() - 1) {
        result += g->get_adjacency_matrix()[row][0];
        return result;
    }
    int minValInRow = 1e9;
    int nearestNeighbour = 0;
    for (int i = 0; i < g->get_graph_order(); ++i) {
        if (visited[i]) continue;
        if (minValInRow > g->get_adjacency_matrix()[row][i] && i != row) {
            minValInRow = g->get_adjacency_matrix()[row][i];
            nearestNeighbour = i;
            visited[i] = 1;
        }
    }
    result += g->get_adjacency_matrix()[row][nearestNeighbour];

    return greedy_path(g, ++row, visited, result);

}