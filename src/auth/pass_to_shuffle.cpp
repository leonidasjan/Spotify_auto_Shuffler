#include<vector>
#include "get_vector.hpp"
#include "shuffle_alg.hpp"
//Gets shuffled vector
std::vector<int> get_vector(){
    std::vector<int> vec = {1,2,3};
    shuffler(vec);
    return vec;
}