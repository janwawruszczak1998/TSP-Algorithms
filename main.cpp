#include <bits/stdc++.h>
#include "Graph.hpp"
#include "ExactAlgorithms.hpp"
#include "LocalSearchAlgorithms.hpp"
#include "ACO.hpp"

#pragma GCC optimize ("O3")

void menu() {
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

int main() {
    srand(time(NULL));
    char action{};
    string name{};
    int n{};
    std::unique_ptr<Graph> g = nullptr;
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

    do {
        menu();
        std::cin >> action;
        switch (action) {
            case '1':
                if (g) {
                    start_time = std::chrono::high_resolution_clock::now();
                    BF(g);
                } else {
                    cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '2':
                if (g) {
                    start_time = std::chrono::high_resolution_clock::now();
                    DP(g);
                } else {
                    cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '3':
                if (g) {
                    start_time = std::chrono::high_resolution_clock::now();
                    BB(g);
                } else {
                    cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '4':
                if (g) {
                    start_time = std::chrono::high_resolution_clock::now();
                    SA(g);
                } else {
                    cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '5':
                if (g) {
                    start_time = std::chrono::high_resolution_clock::now();
                    TS(g);
                } else {
                    cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '6':
                if (g) {
                    start_time = std::chrono::high_resolution_clock::now();
                    ACO(g);
                } else {
                    cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case 'u':
                cout << "Wprowadź rozmiar i macierz grafu\n";
                std::cin >> n;
                if (g) {
                    g.reset(nullptr);
                }
                g = std::make_unique<Graph>(Graph(n));
                break;
            case 'a':
                if (g) {
                    g.reset(nullptr);
                }
                cout << "Wprowadź nazwę pliku\n";
                std::cin >> name;
                g = std::make_unique<Graph>(Graph(std::move(std::string("Graphs/") + name)));
                break;
            default:
                return 0;
                break;
        }


        auto end_time = std::chrono::high_resolution_clock::now();

        if (action != '0')
            std::cout << "Czas: " << (std::chrono::duration_cast<std::chrono::milliseconds>(
                    end_time - start_time).count()) / 1000 << "s" << std::endl;

    } while (action != '0');


    return 0;
}