//
// Created by Jan on 2019-11-25.
//

#include "LocalSearchAlgorithms.hpp"
#include "ExactAlgorithms.hpp"
#include <unistd.h>
#include "Randoms.hpp"

extern re::Randoms randoms;

unsigned random_path(std::vector<unsigned> &v, std::unique_ptr <Graph> &g) {

    v.clear();
    v.reserve(g->get_graph_order());
    for (unsigned i = 0; i < g->get_graph_order(); ++i) {
        v.push_back(i);
    }

    std::random_shuffle(v.begin(), v.end());
    return calculate_objective(v, g);

}


void SA(std::unique_ptr <Graph> &g) {

    std::vector<unsigned> solution;
    unsigned result = random_path(solution, g);   //pierwsze losowe rozwiazanie
    auto permutation(solution);
    auto next_step(permutation);
    unsigned step_val{};

    unsigned stagnation = 0; //wskaznik stagnacji

    unsigned iteration = 0;  //liczba iteracji w ramach jednego chlodzenia i wyzazania
    double temperature = 1e9;   //temperatura poczatkowa

    for (unsigned num_of_try = 0; num_of_try < 100; ++num_of_try) {
        //cout << num_of_try << "%\n";
        while (temperature > 0.1) {

            iteration++;
            unsigned rep = 3 * g->get_graph_order();  //liczba obrotow (sprawdzen sasiadow w pokoleniu) = 3n
            next_step = permutation;
            step_val = calculate_objective(next_step, g);   //wartosc kroku

            while (rep-- > 0) {

                unsigned f_pos = 0;  //wyznaczenie pierwszego miejsca swapa
                unsigned s_pos = 0;  //wyznaczenie drugiego miejsca swapa
                do {
                    f_pos = randoms.random_in_range(0, g->get_graph_order() - 1);
                    s_pos = randoms.random_in_range(0, g->get_graph_order() - 1);
                } while (f_pos == s_pos);
                if (f_pos > s_pos) std::swap(f_pos, s_pos);

                unsigned curr_val = step_val;

                //wyznaczenie nastepnikow i poprzednikow miejsc swapa
                unsigned pre_f_pos = f_pos - 1;
                unsigned next_f_pos = f_pos + 1;
                if (f_pos == 0) pre_f_pos = g->get_graph_order() - 1;

                unsigned pre_s_pos = s_pos - 1;
                unsigned next_s_pos = s_pos + 1;
                if (s_pos == g->get_graph_order() - 1) next_s_pos = 0;

                //sprawdzenie wartosci kroku
                if (s_pos - f_pos == 1 || s_pos == g->get_graph_order() - 1 || f_pos == 0) { //krancowe przypadki
                    std::swap(next_step[f_pos], next_step[s_pos]);
                    step_val = calculate_objective(next_step, g);
                } else {   //ogolny przypadek
                    step_val = step_val
                               - g->get_adjacency_matrix()[next_step[pre_f_pos]][next_step[f_pos]]
                               - g->get_adjacency_matrix()[next_step[f_pos]][next_step[next_f_pos]]
                               - g->get_adjacency_matrix()[next_step[pre_s_pos]][next_step[s_pos]]
                               - g->get_adjacency_matrix()[next_step[s_pos]][next_step[next_s_pos]]
                               + g->get_adjacency_matrix()[next_step[pre_f_pos]][next_step[s_pos]]
                               + g->get_adjacency_matrix()[next_step[s_pos]][next_step[next_f_pos]]
                               + g->get_adjacency_matrix()[next_step[pre_s_pos]][next_step[f_pos]]
                               + g->get_adjacency_matrix()[next_step[f_pos]][next_step[next_s_pos]];
                    std::swap(next_step[f_pos], next_step[s_pos]);
                }

                unsigned difference = step_val - curr_val; //roznica rozwiazan najlepszego i aktualnego
                if (step_val < result) {  //jesli poprawilismy, wyzeruj wskaznik stagnacji i aktualizuj rezultat
                    stagnation = 0;
                    result = step_val;
                    solution = next_step;
                } else {   //jesli nie poprawilismy, to zwieksz wskaznik stagnacji
                    stagnation++;
                    if (stagnation < g->get_graph_order()) {
                        temperature = (double) (1e9) / 2;   //jesli stagnacja jest duza, to zwieksz temperature

                        unsigned f_inv = 0;  //wyznaczenie pierwszego miejsca inverta
                        unsigned s_inv = 0;  //wyznaczenie drugiego miejsca inverta
                        do {
                            f_inv = randoms.random_in_range(0, g->get_graph_order() - 1);
                            s_inv = randoms.random_in_range(0, g->get_graph_order() - 1);
                        } while (f_inv == s_inv);

                        if (f_inv > s_inv) std::swap(f_inv, s_inv);
                        for (unsigned i = f_inv; i < (f_pos + s_inv) / 2 - 1; ++i) {   //przetasuj permutacje
                            std::swap(next_step[i], next_step[s_pos - i + f_inv]);
                            permutation = next_step;
                            break;
                        }
                    }
                }

                double acc = get_probability(difference, temperature);
                auto prob = static_cast<double>(randoms.random_in_range(0, 1e9)) / static_cast<double>((1e9));

                //decyzja o przyjeciu/odrzuceniu rozwiazania
                if (difference < 0 || (difference > 0 && acc > prob)) {
                    permutation = next_step;
                    break;
                } else {
                    std::swap(next_step[f_pos], next_step[s_pos]);
                }


            }

            temperature = cooling(temperature);  //akutalizacja temperatury
            if (iteration > g->get_graph_order() * g->get_graph_order())
                break; //jesli zbyt dlugo siedzimy w jednym miejscu

        }

        //losowanie nowego punktu startu i reset zmiennych
        random_path(permutation, g);
        next_step = permutation;
        iteration = 0;
        stagnation = 0;
        temperature = 1e9;
    }

    std::cout << "Najlepszy wynik uzyskany SA: " << result << "\n";
}

void TS(std::unique_ptr <Graph> &g) {

    //preprocessing
    std::vector<unsigned> permutation, solution;
    unsigned result = random_path(permutation, g);
    unsigned stagnation = 0;
    unsigned curr_val = calculate_objective(permutation, g);
    std::vector <std::vector<unsigned>> tabu_list;
    tabu_list.resize(g->get_graph_order(), std::vector<unsigned>(g->get_graph_order(), 0));
    std::vector<unsigned> next_step;


    //algorytm przeszukiwania z zakazami
    for (unsigned num_of_try = 0; num_of_try < 100; ++num_of_try) { // 100 losowych instancji
        //std::cout << num_of_try << "%\n";
        for (unsigned step = 0; step < 15 * g->get_graph_order(); ++step) {   // liczba "ruchów" w calym pokoleniu
            unsigned next_step_val = std::numeric_limits<unsigned>::max();
            unsigned f_tabu = 0, s_tabu = 0;
            //przeglad ruchow w pozycji
            for (unsigned f_pos = 0; f_pos < g->get_graph_order(); ++f_pos) {

                for (unsigned s_pos = f_pos + 1; s_pos < g->get_graph_order(); ++s_pos) {

                    unsigned curr_val_tmp = curr_val;

                    if (s_pos - f_pos == 1 || s_pos == g->get_graph_order() - 1 ||
                        f_pos == 0) { //krancowe przypadki ruchow - aktualizacja O(n)
                        std::swap(permutation[f_pos], permutation[s_pos]);
                        curr_val = calculate_objective(permutation, g);
                    } else {   //ogolny przypadek - aktualizacja w O(1)
                        curr_val = curr_val
                                   - g->get_adjacency_matrix()[permutation[f_pos - 1]][permutation[f_pos]]
                                   - g->get_adjacency_matrix()[permutation[f_pos]][permutation[f_pos + 1]]
                                   - g->get_adjacency_matrix()[permutation[s_pos - 1]][permutation[s_pos]]
                                   - g->get_adjacency_matrix()[permutation[s_pos]][permutation[s_pos + 1]]
                                   + g->get_adjacency_matrix()[permutation[f_pos - 1]][permutation[s_pos]]
                                   + g->get_adjacency_matrix()[permutation[s_pos]][permutation[f_pos + 1]]
                                   + g->get_adjacency_matrix()[permutation[s_pos - 1]][permutation[f_pos]]
                                   + g->get_adjacency_matrix()[permutation[f_pos]][permutation[s_pos + 1]];
                        std::swap(permutation[f_pos], permutation[s_pos]);
                    }   // zamortyzowany czas O(1)

                    if (curr_val < result) {      //jesli rozwiazanie lepsze niz globalne najlepsze
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
                        stagnation = 0;
                    } else {   //jesli nie poprawilismy, to zwieksz wskaznik stagnacji
                        stagnation++;
                        if (stagnation < g->get_graph_order() * sqrt(g->get_graph_order())) {
                            unsigned f_inv = 0;  //wyznaczenie pierwszego miejsca inverta
                            unsigned s_inv = 0;  //wyznaczenie drugiego miejsca inverta
                            do {
                                f_inv = randoms.random_in_range(0, g->get_graph_order() - 1);
                                s_inv = randoms.random_in_range(0, g->get_graph_order() - 1);
                            } while (f_inv == s_inv);
                            if (f_inv > s_inv) std::swap(f_inv, s_inv);
                            //       for(unsigned i = f_inv; i < (f_pos + s_inv) / 2; ++i){   //przetasuj permutacje
                            //           std::swap(permutation[i], permutation[s_pos - i + f_inv]  );
                            //       }
                        }
                    }

                    std::swap(permutation[f_pos],
                              permutation[s_pos]);  //powrot do wczesniejszej pozycji i szukanie lepszych ruchow
                    curr_val = curr_val_tmp;
                }
            }
            permutation = next_step;    //przypisanie najlepszego znalezionego ruchu
            tabu_list[f_tabu][s_tabu] += g->get_graph_order(); //wrzucenie na liste tabu
        }

        //nowa wylosowane podejscie
        permutation.clear();
        random_path(permutation, g);
        for (auto tabu_vec : tabu_list) {
            tabu_vec.clear();
        }
        tabu_list.clear();
        tabu_list.resize(g->get_graph_order());
        for (auto tabu_vec : tabu_list) {
            tabu_vec.resize(g->get_graph_order(), 0);
        }
    }

    std::cout << "Najlepszy rezultat ustaliony TS: " << result << "\n";
}


double cooling(double temperature) {
    return (temperature *= 0.95);
}

double get_probability(unsigned difference, double temperature) //Funkcja określa jak słabe jest sugerowane rozwiązanie
{
    return exp(-1 * difference / temperature);
}

unsigned calculate_objective(std::vector<unsigned> &permutation, std::unique_ptr <Graph> &g) {
    unsigned result = 0;

    for (auto it = permutation.cbegin(); it + 1 != permutation.cend(); ++it) {
        result += g->get_adjacency_matrix()[*it][*(it + 1)];

    }

    result += g->get_adjacency_matrix()[*(permutation.end() - 1)][*(permutation.begin())];
    return result;

}

void partial_invert(vector<unsigned> &permutation, unsigned start, unsigned finish) {
    for (unsigned i = start; i == start + (start - finish) / 2; ++i) {
        std::swap(permutation[i], permutation[finish - i - start]);
    }
}
