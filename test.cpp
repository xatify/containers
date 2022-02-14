#include "vector.hpp"
#include <vector>
#include <iostream>



template <typename C>
void print (C    v) {
    for (typename C::iterator it = v.begin (); it != v.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl; 
};
struct TYPE{};

template <typename C>
void test (C    v) {
    std::cout << "push_back :\n";
    for (int i = 0; i < 20; i++)
        v.push_back (i);
    print (v);
    std::cout << v.capacity () << "  " << v.size () << std::endl;
    std::cout << v.max_size () << std::endl;
    std::cout << std::boolalpha << v.empty ()  << std::endl;

    std::cout << "resize :\n";
    v.resize (100, 10);
    print (v);
    std::cout << std::endl;

    std::cout << "reserve :\n";
    v.reserve (200);
    print (v);
    std::cout  << v.capacity () << std::endl;

    std::cout << "operator [10]: " << v[10] << std::endl;
    std::cout << "operator at(10): " << v.at(10) << std::endl;
    std::cout << "operator at(1000): ";
    try {
        v.at(1000) = 0;
    }
    catch (std::exception &e) {
        std::cout << e.what ();
    }
    std::cout << std::endl;

    std::cout << "assign : \n";
	int array[] = {1, 2, 5,  8, 7, 87};
	v.assign (array, array + 3);
	std::vector<int> v2 (10, 1);
    v.assign (v2.begin (), v2.end());
	print (v);
	v.assign (10, 2);
    print (v);


	std::cout << "insert: \n";
	typename C::iterator it = v.begin () + 1;
	v.insert(it, 2, 100);
	print (v);

	v.insert(it, 100);
	print (v);

	it = v.begin () + 10;
	v.insert(it, array, array + 5);
	print (v);

	std::cout << "erase: \n";
	v.erase (v.begin () + 1);
	print (v);

	v.erase (v.begin (), v.begin () + 10);
	print (v);

	std::cout << "push_back: \n";
	v.push_back (-1);
	print (v);

	std::cout << "pop_back: \n";
	v.pop_back ();
	print (v);

}

int main () {
    
    {
        std::vector<int> v;
        test (v);
    }

    std::cout << "***************************************\n";
    {
		ft::vector<int> v;
        test (v);
    }


}

