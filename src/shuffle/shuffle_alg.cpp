#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "shuffle_alg.hpp"



void shuffler(std::vector<int>& vec)
{
    using std::vector;

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(vec.begin(),vec.end(), g);
}
