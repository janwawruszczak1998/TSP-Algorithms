//
// Created by Jan on 2020-01-05.
//
#include <bits/stdc++.h>
#include "Ant.h"
#include "ACO.h"
#include "Randoms.cpp"

Randoms* randoms = new Randoms(time(NULL));

double Phi(int city_i, int city_j, Ant* ant, Graph* g, std::vector< std::vector<double> >& pheromones){
    double A = 1.1, B = 4.5;

    double eta_ij = (double) pow (1.0 / g->get_matrix()[city_i][city_j], B);
    double tau_ij = (double) pow (pheromones[city_i][city_j],   A);
    double sum = 0.0;

    //obliczenie sumy dla pozostalych miast
    for (int i = 0; i < g->get_rank(); ++i) {
        if(i == city_i) continue;   //nie rozwazamy przejscia do samego siebie
        if(ant->get_tabu()[i] == false){
            double eta = (double) pow ( (double)(1.0 / g->get_matrix()[city_i][i]), B);
            double tau = (double) pow (pheromones[city_i][i], A);
            sum += eta * tau;
        }
    }

    return (eta_ij * tau_ij) / sum;
}

int route_value(std::vector<int>& permutation, Graph *g){
    int result = 0;

    for(std::vector<int>::iterator it = permutation.begin(); it + 1 != permutation.end(); ++it) {
        result += g->get_matrix()[*it][*(it + 1)];

    }

    result += g->get_matrix()[*(permutation.end() - 1)][*(permutation.begin())];
    return result;

}

void updatePheromones(std::vector< std::vector<double> >& pheromones, std::vector< std::vector<int> >& routes, Graph* g){
    double q = g->get_rank(); //parametr feromonowy - reguluje ile jest kladzione feromonu
    double ro = 0.5; //ro - parametr aktualizowanego feromonu < 1
    for(int i = 0; i < routes.size(); ++i){
        int route_i = route_value(routes[i], g); //obliczenie wartosci i-tej trasy
        for(int j = 0; j < routes.size() - 1; ++j){
            int city_i = routes[i][j];
            int city_j = routes[i][j+1];
            pheromones[city_i][city_j] = (1.0 - ro) * pheromones[city_i][city_j] + q / (double)route_i;
            pheromones[city_j][city_i] = (1.0 - ro) * pheromones[city_j][city_i] + q / (double)route_i;
        }
    }
}

int getNextCity(std::vector<double>& probability){

    double xi = randoms -> Uniforme();
    int i = 0;
    double sum = probability[i];
    while (sum < xi) {
        i++;
        sum += probability[i];
    }

    return i;
}


void route(Ant* ant, std::vector< std::vector<int> >& routes, Graph* g, std::vector< std::vector<double> >& pheromones ){

    std::vector<double> probability; //vector prawodpodobienstw przejsc do miast

    routes[ant->get_number()][0] = ant->get_number();   //miasto poczatkowe mrowki to jej numer
    ant->get_tabu()[ant->get_number()] = true;
    //ustalenie kolejnych n-1 miast trasy
    for(int i = 0; i < g->get_rank() - 1; ++i){
        int city_i = routes[ant->get_number()][i];  //miasto i-te
        probability.clear();
        probability.resize(g->get_rank(), 0.0);

        //wylicz prawdopodobienswo przejscia do miasta j-tego
        for(int city_j = 0; city_j < g->get_rank(); ++city_j){
            if(city_i == city_j) continue;  //polaczenie ze samym soba
            if(ant->get_tabu()[city_j] == false){   //jesli miasto jeszcze nie odwiedzone
                probability[city_j] = Phi(city_i, city_j, ant, g, pheromones);
            }
        }


        //ustalenie kolejnego miasta
        routes[ant->get_number()][i + 1] = getNextCity(probability);
        ant->get_tabu()[routes[ant->get_number()][i + 1]] = true;
    }
}

void ACO(Graph * g) {

    int best = INT_MAX;
    int iterations = 100;
    int numOfAnts = g->get_rank();

    //trasy mrowek
    std::vector< std::vector<int> > routes(numOfAnts);
    for(int i = 0; i < g->get_rank(); ++i){
        routes[i].resize(g->get_rank());
    }
    //feromony na krawedziach
    std::vector< std::vector<double> > pheromones(g->get_rank());
    for(int i = 0; i < g->get_rank(); ++i){
        pheromones[i].resize(g->get_rank());
        for(int j = 0; j < g->get_rank(); ++j){
            pheromones[i][j] = randoms -> Uniforme() * g->get_rank() / g->get_matrix()[0][1];  //losowa ilość feromonu na krawędziach
        }
    }


    //wielokrotne puszczenie mrowek
    for(int i = 0; i < iterations; ++i){
        std::cout << 100 * i / iterations << "%\n";
        //puszczenie mrowek
        for(int ant = 0; ant < numOfAnts; ++ant){
            //std::cout << "Poszla mrowka nr " << ant << "\n";
            //wyczyszczenie starej trasy mrowki numer
            for(std::vector<int>::iterator it = routes[ant].begin(); it != routes[ant].end(); ++it){
                *it = -1;
            }
            //stworzenie mrowki i puszczenie jej w trase
            Ant* next_ant = new Ant(ant, g->get_rank());
            route(next_ant, routes, g, pheromones);

        }

        //akutalizacja feromonow
        updatePheromones(pheromones, routes, g);

        //wyczyszczenie tras
        for(int i = 0; i < g->get_rank(); ++i) {
            if (route_value(routes[i], g) < best) {
                best = route_value(routes[i], g);
                std::cout << "Poprawiono na " << best << "\n";
            }
            for (int j = 0; j < g->get_rank(); ++j)
                routes[i][j] = -1;
        }

    }


    std::cout << "Najlepsze co znalazly mrowki to: " << best << "\n";
    return;
}
