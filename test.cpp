#include "vector.hpp"
#include <iostream>

int main () {
    ft::vector<int> v;

    v.push_back (10);
    v.push_back (20);
    v.push_back (30);

    for (ft::vector<int>::iterator it = v.begin (); it != v.end (); it++)
        std::cout << *it;
}