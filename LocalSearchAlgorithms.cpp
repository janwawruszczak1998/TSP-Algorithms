//
// Created by Jan on 2019-11-25.
//

#include "LocalSearchAlgorithms.h"
#include "ExactAlgorithms.h"
#include <unistd.h>


void SA(Graph *g){

    std::vector<int> solution;
    int result = random_path(solution, g);   //pierwsze losowe rozwiazanie
    auto permutation(solution);
    auto next_step(permutation);
    int step_val = calculate_objective(next_step, g);

    int stagnation = 0; //wskaznik stagnacji

    int iteration = 0;  //liczba iteracji w ramach jednego chlodzenia i wyzazania
    double temperature = 1e9;   //temperatura poczatkowa

    for(int num_of_try = 0; num_of_try < 100; ++num_of_try) {
        cout << num_of_try << "%\n";
        while(temperature > 0.1){

            iteration++;
            int rep = 3*g->get_rank();  //liczba obrotow (sprawdzen sasiadow w pokoleniu) = 3n
            next_step = permutation;
            step_val = calculate_objective(next_step, g);   //wartosc kroku

            while(rep-->0){
                int f_pos = 0;  //wyznaczenie pierwszego miejsca swapa
                int s_pos = 0;  //wyznaczenie drugiego miejsca swapa
                do{
                    f_pos= rand() % g->get_rank();
                    s_pos = rand() % g->get_rank();
                }while(f_pos == s_pos);
                if(f_pos > s_pos) std::swap(f_pos, s_pos);

                //wyznaczenie nastepnikow i poprzednikow miejsc swapa
                int pre_f_pos = f_pos - 1;
                int next_f_pos = f_pos + 1;
                if(f_pos == 0) pre_f_pos = g->get_rank() - 1;

                int pre_s_pos = s_pos - 1;
                int next_s_pos = s_pos + 1;
                if(s_pos == g->get_rank() - 1) next_s_pos = 0;


                //sprawdzenie wartosci kroku
                if(s_pos - f_pos == 1 || s_pos == g->get_rank() - 1 || f_pos == 0){ //krancowe przypadki
                    std::swap(next_step[f_pos], next_step[s_pos]);
                    step_val = calculate_objective(next_step, g);
                }
                else{   //ogolny przypadek
                    step_val = step_val
                               - g->get_matrix()[next_step[pre_f_pos]][next_step[f_pos]]
                               - g->get_matrix()[next_step[f_pos]][next_step[next_f_pos]]
                               - g->get_matrix()[next_step[pre_s_pos]][next_step[s_pos]]
                               - g->get_matrix()[next_step[s_pos]][next_step[next_s_pos]]
                               + g->get_matrix()[next_step[pre_f_pos]][next_step[s_pos]]
                               + g->get_matrix()[next_step[s_pos]][next_step[next_f_pos]]
                               + g->get_matrix()[next_step[pre_s_pos]][next_step[f_pos]]
                               + g->get_matrix()[next_step[f_pos]][next_step[next_s_pos]];
                    std::swap(next_step[f_pos], next_step[s_pos]);
                }
                //if(step_val != calculate_objective(next_step, g) ) cout << "!";

                int difference = step_val - result; //roznica rozwiazan najlepszego i aktualnego

                if(step_val < result){  //jesli poprawilismy, wyzeruj wskaznik stagnacji i aktualizuj rezultat
                    stagnation = 0;
                    result = step_val;
                    solution = next_step;
                }
                else{   //jesli nie poprawilismy, to zwieksz wskaznik stagnacji
                    stagnation++;
                    if(stagnation < g->get_rank()) temperature = (double)(1e9)/2;   //jesli stagnacja jest duza, to zwieksz temperature
                    //moze zrobic duzego inverta? jesli sama temperatura nie wystarcza?

                }

                //decyzja o przyjeciu/odrzuceniu rozwiazania
                if(difference < 0 || (difference > 0 &&  get_probability(difference, temperature) > ( (double) rand() / (RAND_MAX)) + 1) ){
                    permutation = next_step;
                    break;
                }
                else{
                    std::swap(next_step[f_pos], next_step[s_pos]);
                }

            }
            temperature = cooling(temperature, iteration);  //akutalizacja temperatury
            if(iteration > g->get_rank()*g->get_rank()) break; //jesli zbyt dlugo siedzimy w jednym miejscu
        }

        //losowanie nowego punktu startu i reset zmiennych
        random_path(permutation, g);
        next_step = permutation;
        step_val = calculate_objective(permutation, g);
        iteration = 0;
        stagnation = 0;
        temperature = 1e9;
    }

    std::cout << "Najlepszy wynik uzyskany SA: " << result << "\n";
}

void TS(Graph *g){

    //preprocessing
    std::vector<int> permutation, solution;
    int result = random_path(permutation, g);
    int curr_val = calculate_objective(permutation, g);
    std::vector<std::vector<int> > tabu_list;
    tabu_list.resize(g->get_rank());
    for(std::vector<vector<int> >::iterator it = tabu_list.begin(); it != tabu_list.end(); ++it){
        it->resize(g->get_rank(), 0);
    }
    std::vector<int> next_step;


        //algorytm przeszukiwania z zakazami
    for(int num_of_try = 0; num_of_try < 100; ++num_of_try) { // 200 losowych instancji
        std::cout << num_of_try << "%\n";
        for (int step = 0; step < 15*g->get_rank(); ++step) {   // liczba "ruchów" w calym pokoleniu
            int next_step_val = std::numeric_limits<int>::max();

            int f_tabu = 0, s_tabu = 0;
            //przeglad ruchow w pozycji
            for (int f_pos = 0; f_pos < g->get_rank(); ++f_pos) {
                for (int s_pos = f_pos + 1; s_pos < g->get_rank(); ++s_pos) {

                    int curr_val_tmp = curr_val;

                    if(s_pos - f_pos == 1 || s_pos == g->get_rank() - 1 || f_pos == 0){ //krancowe przypadki ruchow - aktualizacja O(n)
                        std::swap(permutation[f_pos], permutation[s_pos]);
                        curr_val = calculate_objective(permutation, g);
                    }
                    else{   //ogolny przypadek - aktualizacja w O(1)
                        curr_val = curr_val
                                   - g->get_matrix()[permutation[f_pos - 1]][permutation[f_pos]]
                                   - g->get_matrix()[permutation[f_pos]][permutation[f_pos + 1]]
                                   - g->get_matrix()[permutation[s_pos - 1]][permutation[s_pos]]
                                   - g->get_matrix()[permutation[s_pos]][permutation[s_pos + 1]]
                                   + g->get_matrix()[permutation[f_pos - 1]][permutation[s_pos]]
                                   + g->get_matrix()[permutation[s_pos]][permutation[f_pos + 1]]
                                   + g->get_matrix()[permutation[s_pos - 1]][permutation[f_pos]]
                                   + g->get_matrix()[permutation[f_pos]][permutation[s_pos + 1]];
                        std::swap(permutation[f_pos], permutation[s_pos]);
                    }   // zamortyzowany czas O(1)

                    if(curr_val < result){      //jesli rozwiazanie lepsze niz globalne najlepsze
                        result = curr_val;      //to przypisz jako najlepsze globalne
                        solution = permutation;
                    }

                    if (curr_val < next_step_val) {              //jesli rozwiazanie lepsze niz lokalne (w ramach ruchu)
                        if (tabu_list[s_pos][f_pos] <= step) {  //jesli ruch jest dostepny to aktualizacja
                            s_tabu = f_pos;
                            f_tabu = s_pos;
                            next_step = permutation;
                            next_step_val = curr_val;
                        }
                    }

                    std::swap(permutation[f_pos], permutation[s_pos]);  //powrot do wczesniejszej pozycji i szukanie lepszych ruchow
                    curr_val = curr_val_tmp;
                }
            }
            permutation = next_step;    //przypisanie najlepszego znalezionego ruchu
            tabu_list[f_tabu][s_tabu] += g->get_rank(); //wrzucenie na liste tabu
        }

        //nowa wylosowane podejscie
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
    return (temperature *= 0.95);
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

    result += g->get_matrix()[*(permutation.end() - 1)][*(permutation.begin())];
    return result;

}