
#include <memory>
#include "iterator.hpp"

namespace ft {
	template <typename T, typename Alloc = std::allocator <T> >
		class vector {
            typedef std::allocator<T>   allocator_type;
            typedef size_t              size_type;
            typedef T                   value_type;
            typedef iterator<T>         iterator;
            typedef iterator<const T>   const_iterator;

            explicit vector (const allocator_type& alloc = allocator_type ());
            explicit vector (size_type n, const value_type& = value_type (), const allocator_type& alloc = allocator_type ()));
            template <typename InputIterator>
                vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type ());
            
            vector (const vector& x);

            ~vector ();

            vector& operator = (const vector& x);

            iterator begin ();

		};
}


// template <class Category, class T, class Distance = ptrdiff_t,
//           class Pointer = T*, class Reference = T&>
//   struct iterator {
//     typedef T         value_type;
//     typedef Distance  difference_type;
//     typedef Pointer   pointer;
//     typedef Reference reference;
//     typedef Category  iterator_category;
//   };