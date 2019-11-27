//
// Created by Jan on 2019-11-25.
//

#include "LocalSearchAlgorithms.h"
#include "ExactAlgorithms.h"


void SA(Graph *g){

    std::vector<int> solution;
    int result = random_path(solution, g);   //pierwsze losowe rozwiazanie
    auto permutation(solution);
    auto next_step(permutation);
    int iteration = 0;
    double temperature = 1e9;

    for(int num_of_try = 0; num_of_try < 100; ++num_of_try) {
        cout << num_of_try << "%\n";
        while(temperature > 0.1){

            iteration++;
            int rep = g->get_rank()*g->get_rank();
            next_step = permutation;

            while(rep-->0){

                int pos1 = rand() % g->get_rank();
                int pos2 = rand() % g->get_rank();

                std::swap(next_step[pos1], next_step[pos2]);
                int step_val = calculate_objective(next_step, g);
                int difference = step_val - result;


                if(step_val < result){
                    result = step_val;
                    solution = next_step;
                }
                if(difference < 0 || (difference >0 &&  get_probability(difference, temperature) > ( (double) rand() / (RAND_MAX)) + 1) ){
                    permutation = next_step;
                }


            }
            temperature = cooling(temperature, iteration);
        }


        random_path(permutation, g);
        next_step = permutation;
        iteration = 0;
        temperature = 1e9;
    }

    std::cout << "Najlepszy wynik uzyskany SA: " << result << "\n";
}

void TS(Graph *g){

    //preprocessing
    std::vector<int> permutation, solution;
    int result = random_path(permutation, g);
    std::vector<std::vector<int> > tabu_list;
    tabu_list.resize(g->get_rank());
    for(std::vector<vector<int> >::iterator it = tabu_list.begin(); it != tabu_list.end(); ++it){
        it->resize(g->get_rank(), 0);
    }
    std::vector<int> next_step;


    //algorytm przeszukiwania z zakazami
    for(int num_of_try = 0; num_of_try < 200; ++num_of_try) {
        cout << num_of_try/200 << "%\n";
        for (int step = 0; step < 15*g->get_rank(); ++step) {
            int next_step_val = std::numeric_limits<int>::max();

            int f_tabu = 0, s_tabu = 0;
            for (int i = 0; i < g->get_rank(); ++i) {
                for (int j = i + 1; j < g->get_rank(); ++j) {
                    swap(permutation[i], permutation[j]);
                    int curr_val = calculate_objective(permutation, g);
                    if(curr_val < result){
                        result = curr_val;
                        solution = permutation;
                    }
                    if (curr_val < next_step_val) {
                        if (tabu_list[j][i] <= step) {
                            s_tabu = i;
                            f_tabu = j;
                            next_step = permutation;
                            next_step_val = curr_val;
                        }
                    }
                    swap(permutation[i], permutation[j]);
                }
            }
            permutation = next_step;
            tabu_list[f_tabu][s_tabu] += g->get_rank();
        }

        //generate new  solution;
        permutation.clear();
        random_path(permutation, g);
        for(std::vector<vector<int> >::iterator it = tabu_list.begin(); it != tabu_list.end(); ++it){
            it->clear();
        }
        tabu_list.clear();
        tabu_list.resize(g->get_rank());
        for(std::vector<vector<int> >::iterator it = tabu_list.begin(); it != tabu_list.end(); ++it){
            it->resize(g->get_rank(), 0);
        }
    }

    std::cout << "Najlepszy rezultat ustaliony TS: " << result << "\n";
}


double cooling(double temperature, int t){
    return (temperature *= 0.85);
}

double get_probability(int difference,double temperature) //Funkcja określa jak słabe jest sugerowane rozwiązanie
{
    return exp(-1*difference/temperature);
}

int random_path(std::vector<int>& v, Graph* g){

    v.reserve(g->get_rank());
    for(int i = 0; i < g->get_rank(); ++i){
        v.push_back(i);
    }

    std::random_shuffle(v.begin(), v.end());
    return calculate_objective(v, g);

}


int calculate_objective(std::vector<int>& permutation, Graph *g){
    int result = 0;
    for(std::vector<int>::iterator it = permutation.begin(); it + 1 != permutation.end(); ++it) {
        result += g->get_matrix()[*it][*(it + 1)];
    }
    result += g->get_matrix()[*(permutation.end() - 1)][*permutation.begin()];
    return result;

}