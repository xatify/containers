#ifndef __ITERATOR_RBT_HPP__
#define __ITERATOR_RBT_HPP__

#include <iterator>

template <typename RBT>

class rbt_iterator: public std::iterator <std::bidirectional_iterator_tag, typename RBT::value_type> {
	
	typedef	typename RBT::pointer		pointer;
	public:
		
		typedef typename RBT::value_type																value_type;
		typedef typename std::iterator<std::bidirectional_iterator_tag, value_type>::pointer			pointer;
		typedef typename std::iterator<std::bidirectional_iterator_tag, value_type>::reference			reference;
		typedef typename std::iterator<std::bidirectional_iterator_tag, value_type>::difference_type	difference_type;
		typedef typename std::iterator<std::bidirectional_iterator_tag, value_type>::iterator_category	iterator_category;

	public:
		rbt_iterator (RBT* tree_ = 0x0, pointer node_ = 0x0): tree (tree_), node (node_) {};

		rbt_iterator (const rbt_iterator& x): tree (x.tree), node (x.node) {}
		rbt_iterator& operator = (const rbt_iterator& x) { 
			tree = x.tree;
			node = x.node;
			return *this;
		}
		
		bool operator == (const rbt_iterator& iter) const { return node == x.node; }
		bool operator != (const rbt_iterator& iter) const { return node != x.node; }

		reference	operator * () const { return node->value; }
		pointer		operator-> () const { return &(node->value); };

		rbt_iterator& operator ++ () {
			node = tree->successor (node);
			return (*this);
		}

		rbt_iterator& operator ++ (int) {
			rbt_iterator tmp (tree, node);
			node = tree->successor (node);
			return 
		}

		rbt_iterator& operator -- () {
			node = tree->predecessor (node);
			return (*this);
		}
		rbt_iterator& operator -- (int) {
			rbt_iterator = tree->predecessor (node);
			node = tree->predecessor (node);
			return (*this);
		}

	private:
		RBT							*tree;
		pointer						*node;
};

#endif