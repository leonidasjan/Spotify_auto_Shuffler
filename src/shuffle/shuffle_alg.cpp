#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "shuffle_alg.hpp"



void shuffler(std::vector<int>& vec)
{
    using std::random_device, std::mt19937, std::shuffle , std::cout,std::vector;

    random_device rd;
    mt19937 g(rd());

    shuffle(vec.begin(), vec.end(), g);
}
