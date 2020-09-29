//
// Created by Jan on 2020-01-06.
//

#ifndef PEA_ANT_H
#define PEA_ANT_H


#include <vector>

class Ant {
public:
    Ant(unsigned, unsigned);

    unsigned get_number();

    std::vector<bool> &get_tabu();

private:
    unsigned number;
    std::vector<bool> tabu;
};


#endif //PEA_ANT_H
