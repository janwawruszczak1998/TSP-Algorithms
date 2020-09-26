//
// Created by Jan on 2020-01-05.
//
#include <bits/stdc++.h>
#include "Ant.hpp"
#include "ACO.hpp"
#include "Randoms.cpp"

Randoms randoms{time(0)};

double Phi(int city_i, int city_j, Ant *ant, std::unique_ptr<Graph> &g, std::vector<std::vector<double> > &pheromones) {
    double A = 1.1, B = 4.5;

    auto eta_ij = static_cast<double>(pow(1.0 / g->get_adjacency_matrix()[city_i][city_j], B));
    auto tau_ij = static_cast<double>(pow(pheromones[city_i][city_j], A));
    auto sum = 0.0;

    //obliczenie sumy dla pozostalych miast
    for (int i = 0; i < g->get_graph_order(); ++i) {
        if (i == city_i) continue;   //nie rozwazamy przejscia do samego siebie
        if (!ant->get_tabu()[i]) {
            auto eta = (double) pow((double) (1.0 / g->get_adjacency_matrix()[city_i][i]), B);
            auto tau = (double) pow(pheromones[city_i][i], A);
            sum += eta * tau;
        }
    }

    return (eta_ij * tau_ij) / sum;
}

int route_value(std::vector<int> &permutation, std::unique_ptr<Graph> &g) {
    int result = 0;

    for (auto it = permutation.cbegin(); it + 1 != permutation.cend(); ++it) {
        result += g->get_adjacency_matrix()[*it][*(it + 1)];

    }

    result += g->get_adjacency_matrix()[*(permutation.end() - 1)][*(permutation.begin())];
    return result;

}

void update_pheromones(std::vector<std::vector<double> > &pheromones, std::vector<std::vector<int> > &routes,
                       std::unique_ptr<Graph> &g) {
    double q = g->get_graph_order(); //parametr feromonowy - reguluje ile jest kladzione feromonu
    double ro = 0.5; //ro - parametr aktualizowanego feromonu < 1
    for (int i = 0; i < routes.size(); ++i) {
        int route_i = route_value(routes[i], g); //obliczenie wartosci i-tej trasy
        for (int j = 0; j < routes.size() - 1; ++j) {
            int city_i = routes[i][j];
            int city_j = routes[i][j + 1];
            pheromones[city_i][city_j] = (1.0 - ro) * pheromones[city_i][city_j] + q / (double) route_i;
            pheromones[city_j][city_i] = (1.0 - ro) * pheromones[city_j][city_i] + q / (double) route_i;
        }
    }
}

int getNextCity(std::vector<double> &probability) {

    double xi = randoms.uniforme();
    int i = 0;
    double sum = probability[i];
    while (sum < xi) {
        i++;
        sum += probability[i];
    }

    return i;
}


void route(Ant *ant, std::vector<std::vector<int> > &routes, std::unique_ptr<Graph> &g,
           std::vector<std::vector<double> > &pheromones) {

    std::vector<double> probability; //vector prawodpodobienstw przejsc do miast

    routes[ant->get_number()][0] = ant->get_number();   //miasto poczatkowe mrowki to jej numer
    ant->get_tabu()[ant->get_number()] = true;
    //ustalenie kolejnych n-1 miast trasy
    for (int i = 0; i < g->get_graph_order() - 1; ++i) {
        int city_i = routes[ant->get_number()][i];  //miasto i-te
        probability.clear();
        probability.resize(g->get_graph_order(), 0.0);

        //wylicz prawdopodobienswo przejscia do miasta j-tego
        for (int city_j = 0; city_j < g->get_graph_order(); ++city_j) {
            if (city_i == city_j) continue;  //polaczenie ze samym soba
            if (!ant->get_tabu()[city_j]) {   //jesli miasto jeszcze nie odwiedzone
                probability[city_j] = Phi(city_i, city_j, ant, g, pheromones);
            }
        }


        //ustalenie kolejnego miasta
        routes[ant->get_number()][i + 1] = getNextCity(probability);
        ant->get_tabu()[routes[ant->get_number()][i + 1]] = true;
    }
}

void ACO(std::unique_ptr<Graph> &g) {

    int best = INT_MAX;
    int iterations = 100;
    int numOfAnts = g->get_graph_order();

    //trasy mrowek
    std::vector<std::vector<int> > routes(numOfAnts);
    for (int i = 0; i < g->get_graph_order(); ++i) {
        routes[i].resize(g->get_graph_order());
    }
    //feromony na krawedziach
    std::vector<std::vector<double> > pheromones(g->get_graph_order());
    for (int i = 0; i < g->get_graph_order(); ++i) {
        pheromones[i].resize(g->get_graph_order());
        for (int j = 0; j < g->get_graph_order(); ++j) {
            pheromones[i][j] =
                    randoms.uniforme() * g->get_graph_order() /
                    g->get_adjacency_matrix()[0][1];  //losowa ilość feromonu na krawędziach
        }
    }


    //wielokrotne puszczenie mrowek
    for (int i = 0; i < iterations; ++i) {
        std::cout << 100 * i / iterations << "%\n";
        //czyszczenie starych tras
        for (int ant = 0; ant < numOfAnts; ++ant) {
            for (auto &route : routes[ant]) {
                route = -1;
            }
            //stworzenie mrowki i puszczenie jej w trase
            Ant *next_ant = new Ant(ant, g->get_graph_order());
            route(next_ant, routes, g, pheromones);

        }

        //akutalizacja feromonow
        update_pheromones(pheromones, routes, g);

        //wyczyszczenie tras
        for (int a = 0; a < g->get_graph_order(); ++a) {
            if (route_value(routes[a], g) < best) {
                best = route_value(routes[a], g);
                std::cout << "Poprawiono na " << best << "\n";
            }
            for (int b = 0; b < g->get_graph_order(); ++b)
                routes[a][b] = -1;
        }

    }


    std::cout << "Najlepsze co znalazly mrowki to: " << best << "\n";
}
