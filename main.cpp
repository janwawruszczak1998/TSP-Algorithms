#include <bits/stdc++.h>
#include "Graph.h"
#include "ExactAlgorithms.h"
#include "LocalSearchAlgorithms.h"
#include "ACO.h"
#pragma GCC optimize ("O3")

void menu(){
    cout << "Aby przetestować dane wprowadź:\n";
    cout << "1, aby uruchomić BF\n";
    cout << "2, aby uruchomić DP\n";
    cout << "3, aby uruchomić B&B\n";
    cout << "4, aby uruchomić SA\n";
    cout << "5, aby uruchomić TS\n";
    cout << "6, aby uruchomić ACO\n";
    cout << "u, aby wczytać dane wprowadzane ręcznie\n";
    cout << "a, aby wczytać dane wprowadzane z pliku\n";
    cout << "0, aby zakończyć\n";
}

int main(){
    srand( time(NULL) );
    char action;
    string name;
    int n;
    Graph* g = nullptr;
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    do{
        menu();
        cin >> action;
        switch(action){
            case '1':
                if(g != nullptr) {
                    start_time = std::chrono::high_resolution_clock::now();
                    BF(g);
                }
                else{
                    cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '2':
                if(g != nullptr) {
                    start_time = std::chrono::high_resolution_clock::now();
                    DP(g);
                }
                else{
                    cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '3':
                if(g != nullptr) {
                    start_time = std::chrono::high_resolution_clock::now();
                    BB(g);
                }
                else{
                    cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '4':
                if(g != nullptr) {
                    start_time = std::chrono::high_resolution_clock::now();
                    SA(g);
                }
                else{
                    cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '5':
                if(g != nullptr) {
                    start_time = std::chrono::high_resolution_clock::now();
                    TS(g);
                }
                else{
                    cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '6':
                if(g != nullptr) {
                    start_time = std::chrono::high_resolution_clock::now();
                    ACO(g);
                }
                else{
                    cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case 'u':
                cout << "Wprowadź rozmiar i macierz grafu\n";
                cin >> n;
                g = new Graph(n);
                break;
            case 'a':
                cout << "Wprowadź nazwę pliku\n";
                string name;
                cin >> name;
                g = new Graph(name);
                //g->display();
                break;
        }


        auto end_time = std::chrono::high_resolution_clock::now();
        //g->Graph::~Graph();
        if(action !='0') std::cout << "Czas: " << (std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()) / 100 << std::endl;

    }while(action != '0');


    return 0;
}