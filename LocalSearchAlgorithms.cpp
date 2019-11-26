//
// Created by Jan on 2019-11-25.
//

#include "LocalSearchAlgorithms.h"


void SA(Graph *g){

    std::vector<int> solution;
    int result = randomPath(solution, g);   //pierwsze losowe rozwiazanie
    auto permutation(solution);
    auto next_step(permutation);

    double temperature = 1e30;
    std::cout << "Radnom path val: " << result << "\n";

    int iteration = 0;
    while(temperature > 0.0000001){

        iteration++;
        int rep = g->getRank()*g->getRank();
        next_step = permutation;

        while(rep-->0){

            int pos1 = rand() % g->getRank();
            int pos2 = rand() % g->getRank();

            std::swap(next_step[pos1], next_step[pos2]);
            int step_val = calculateObjective(next_step, g);

            int difference = step_val - result;

            if(difference < 0 || (difference >0 &&  getProbability(difference, temperature) > ( (double) rand() / (RAND_MAX)) + 1) ){
                permutation = next_step;
            }
            if(difference < 0){
                result = step_val;
                solution = next_step;
            }

        }
        temperature = cooling(temperature, iteration);
    }
    std::cout << "Liczba iteracji: " << iteration << "\n";
    std::cout << "Najlepszy wynik uzyskany SA: " << result << "\n";
}

void TS(Graph *g){
    std::cout << "TS";
}


double cooling(double temperature, int t){
    return (temperature *= 0.98);
}

double getProbability(int difference,double temperature) //Funkcja określa jak słabe jest sugerowane rozwiązanie
{
    return exp(-1*difference/temperature);
}

int randomPath(std::vector<int>& v, Graph* g){

    v.reserve(g->getRank());
    for(int i = 0; i < g->getRank(); ++i){
        v.push_back(i);
    }

    std::random_shuffle(v.begin(), v.end());
    return calculateObjective(v, g);

}

int greedyyPath(Graph* g, int row, vector<int> &visited, int result){
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

    return greedyyPath(g, ++row, visited, result);

}

int calculateObjective(std::vector<int>& permutation, Graph *g){
    int result = 0;
    for(std::vector<int>::iterator it = permutation.begin(); it + 1 != permutation.end(); ++it) {
        result += g->getMatrix()[*it][*(it + 1)];
    }
    result += g->getMatrix()[*(permutation.end() - 1)][*permutation.begin()];
    return result;

}