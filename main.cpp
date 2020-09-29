#include <bits/stdc++.h>
#include "Graph.hpp"
#include "ExactAlgorithms.hpp"
#include "LocalSearchAlgorithms.hpp"
#include "ACO.hpp"
#include "Randoms.hpp"

#pragma GCC optimize ("O3")

re::Randoms randoms{time(0)};


void menu() {
    std::cout << "Aby przetestować dane wprowadź:\n";
    std::cout << "1, aby uruchomić BF\n";
    std::cout << "2, aby uruchomić DP\n";
    std::cout << "3, aby uruchomić B&B\n";
    std::cout << "4, aby uruchomić SA\n";
    std::cout << "5, aby uruchomić TS\n";
    std::cout << "6, aby uruchomić ACO\n";
    std::cout << "u, aby wczytać dane wprowadzane ręcznie\n";
    std::cout << "a, aby wczytać dane wprowadzane z pliku\n";
    std::cout << "0, aby zakończyć\n";
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
                    std::cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '2':
                if (g) {
                    start_time = std::chrono::high_resolution_clock::now();
                    DP(g);
                } else {
                    std::cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '3':
                if (g) {
                    start_time = std::chrono::high_resolution_clock::now();
                    BB(g);
                } else {
                    std::cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '4':
                if (g) {
                    start_time = std::chrono::high_resolution_clock::now();
                    SA(g);
                } else {
                    std::cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '5':
                if (g) {
                    start_time = std::chrono::high_resolution_clock::now();
                    TS(g);
                } else {
                    std::cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case '6':
                if (g) {
                    start_time = std::chrono::high_resolution_clock::now();
                    ACO(g);
                } else {
                    std::cout << "Wczytaj pierwej graf!\n";
                    break;
                }
                break;
            case 'u':
                std::cout << "Wprowadź rozmiar i macierz grafu\n";
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
                std::cout << "Wprowadź nazwę pliku\n";
                std::cin >> name;
                g = std::make_unique<Graph>(Graph(std::move(std::string("Graphs/") + name)));
                break;
            default:
                if (g) {
                    g.reset(nullptr);
                }
                return 0;
        }


        auto end_time = std::chrono::high_resolution_clock::now();

        if (action != '0')
            std::cout << "Czas: " << (std::chrono::duration_cast<std::chrono::milliseconds>(
                    end_time - start_time).count()) / 1000 << "s" << std::endl;

    } while (action != '0');


    return 0;
}