#ifndef __ITERATOR_MAP_HPP__
#define __ITERATOR_MAP_HPP__

#include <iterator>

template <	typename RBT>

class rbt_iterator: public std::iterator <std::bidirectional_iterator_tag, typename RBT::value_type> {
	public:
		typedef typename RBT::value_type		value_type;
		typedef typename std::iterator<std::bidirectional_iterator_tag, value_type>::pointer			pointer;
		typedef typename std::iterator<std::bidirectional_iterator_tag, value_type>::reference			reference;
		typedef typename std::iterator<std::bidirectional_iterator_tag, value_type>::difference_type	difference_type;
		typedef typename std::iterator<std::bidirectional_iterator_tag, value_type>::iterator_category	iterator_category;

	public:
		rbt_iterator ();
		rbt_iterator (const rbt_iterator& x);
		rbt_iterator& operator (const rbt_iterator& x);
		
		bool operator == (const rbt_iterator)
};

#endif