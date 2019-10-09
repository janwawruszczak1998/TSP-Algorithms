#include <bits/stdc++.h>
#include "Graph.h"
#include "ExactAlgorithms.h"
#pragma GCC optimize ("O3")

void menu(){
    cout << "Aby przetestować dane wprowadź:\n";
    cout << "1, aby uruchomić BF\n";
    cout << "2, aby uruchomić DP\n";
    cout << "3, aby uruchomić B&B\n";
    cout << "0, aby zakończyć\n";
}

int main(){
    char action;
    string name;
    int n;
    Graph* g;
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    do{
        menu();
        cin >> action;
        switch(action){
            case '1':
                cout << "Wprowadź nazwe, rozmiar i macierz grafu\n";
                cin >> name >> n;
                g = new Graph(n);
                start_time = std::chrono::high_resolution_clock::now();
                BF(g);
                break;
            case '2':
                cout << "Wprowadź nazwe, rozmiar i macierz grafu\n";
                cin >> name >> n;
                g = new Graph(n);
                start_time = std::chrono::high_resolution_clock::now();
                DP(g);
                break;
            case '3':
                cout << "Wprowadź nazwe, rozmiar i macierz grafu\n";
                cin >> name >> n;
                g = new Graph(n);
                start_time = std::chrono::high_resolution_clock::now();
                BB(g);
                break;
        }


        auto end_time = std::chrono::high_resolution_clock::now();
        g->Graph::~Graph();
        std::cout << "Czas: " << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << std::endl;

    }while(action != '0');


    return 0;
}