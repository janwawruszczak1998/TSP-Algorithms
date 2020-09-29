//
// Created by jan on 9/29/20.
//

#ifndef PEA_RANDOMS_HPP
#define PEA_RANDOMS_HPP


#include <cstdlib>
#include <cmath>
#include <limits>
#include <random>

namespace re {
    class Randoms {

    private:
        long xpto;
        std::random_device rand_device;
    public:

        explicit Randoms(long);

        // Returns a random Gaussian number.
        double Normal(double, double);

        // Returns a uniform random number between 0 and 1.
        double uniform();

        int random_in_range(int, int);


        float ran1(long *);

        double gaussdev(long *);

    };
} // namespace re;

#endif //TSP_SOLVER_RANDOMS_HPP
