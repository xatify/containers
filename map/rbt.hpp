#ifndef __RBT_HPP__
#define	__RBT_HPP__

#include <memory>

template <typename Key, typename T>
struct node {

	typedef std::pair<Key, T> 	value_type;
	enum { RED, BLACK }			Color;
	
	Color						color;
	std::pair<Key, T>			value;
	node						*left, *right, *parent;

	node (): c(BLACK), left (0x0), right (0x0), parent (0x0) {};
	node (const value_type& v): value (v), left (0x0), right (0x0), parent (0x0) {};
	Key& key () { return value.first; };
};

template <typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<node<Key, T> > >
class RBT {


	typedef Alloc							allocator_type;
	typedef node<T, Key>					node_type;
	typedef typename node_type::value_type	value_type;
	typedef typename value_type::BLACK		BLACK;
	typedef typename value_type::BLACK		RED;
	typedef node_type *						pointer;
	typedef Key								key_type;
	typedef	Compare							key_compare;
	
	public:
		RBT (const key_compare& comp_ = key_compare () , const allocator_type& alloc_ = allocator_type ()): comp (comp_), alloc (alloc_), ROOT (0x0) {
			NIL = alloc.allocate (1);
			alloc.construct (NIL, node_type ());
			ROOT = NIL;
		}
		~RBT () {
			// destroy and deallocate the NIL
			alloc.destroy (NIL); alloc.deallocate (NIL, 1);
			//
		}

		// returns the node with the key k if found otherwise returns NIL
		pointer		search (pointer x, key_type& k) {
			while (x != NIL && k != x->key ())
				x = (comp (k, x->key ())) ? x->left: x->right;
			return x;
		}

		// returns the deepest left node in sub-binay tree rooted at x
		pointer		extrem_left (pointer x) {
			while (x->left != NIL)
				x = x->left;
			return x;
		}

		// returns the deepest right node in sub-binay tree rooted at x
		pointer		extrem_right (pointer x) {
			while (x->right != NIL)
				x = x->right;
			return x;
		}

		// returns the successor of a node assuming x != nil 
		pointer		successor (pointer	x) {
			if (x->right != NIL)
				return extrem_left (x->right);
			pointer y = x->parent;
			while (y != NIL && x == y->right) {
				x = y;
				y = y->parent;
			}
			return y;
		}

		// returns the predecessor of a node assuming x != nil
		pointer		predecessor (pointer x) {
			if (x->left != NIL)
				return extrem_right (x->left);
			pointer y = x->parent;
			while (y != NIL && x == y->left) {
				x = y;
				y = y->parent;
			}
			return y;
		}

		// left rotate operation 
		// assuming that x.right != nil and the root's parent is nil

		void left_rotate (pointer x) {
			pointer y = x->right;
			x->right = y.left;
			if (y->left != NIL)
				y->left->parent = x;
			y->parent = x->parent;
			if (x->parent == NIL)
				ROOT = y;
			else if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
			y->left = x;
			x->parent = y;
		}

		// right rotate operation
		// assuming that y.left != nil and the root's parent is nil
		void right_rotate (pointer y) {
			pointer x = y->left;
			y->left = x->right;
			if (x->right != NIL)
				x->right->parent = y;
			x->parent = y->parent;
			if (y->parent == NIL)
				ROOT = x;
			else if (y == y->parent->left)
				y->parent->left = x;
			else
				y->parent->right = x;
			x->right = y;
			y->parent = y;
		}

		// construct a node z from the pair v
		// we insert the node z in the tree as an oridnary binary tree
		// then we color z red
		// to guarantee that the red-black properties are preserved
		// we call the Insert_fixup function on z to recolor nodes and 
		// perform rotations

		void	insert (const value_type& v) {
			pointer z = alloc.allocate (1);
			alloc.construct (z, node (v));
			
			pointer y = NIL;
			pointer x = ROOT;
			while (x != NIL) {
				y = x;
				x = (comp (z->key (), x->key ()) ? x->left: x->right;
			}
			z->parent = y;
			if (y == NIL)
				ROOT = z;
			else if (comp (z->key (), y->key ()))
				y->left = z;
			else
				y->right = z;
			
			z->left = NIL;
			z->right = NIL;
			z->color = RED;
			this->fixup (z);
		}

		void	fixup (pointer z) {
			while (z->parent->c == RED) {
				if (z->parent == z->parent->parent->left) {
					pointer y = z->parent->parent->right;		// y the uncle of z
					if (y->c == RED) {
						z->parent->c = BLACK;
						y->color = BLACK;
						z->parent->parent->c = RED;
						z = z->parent->parent;
					}
					else if (z)
				}
			}
		}

		

 	private:
		Compare			comp;		// compare function object for the keys
		allocator_type	alloc;		// allocator
		pointer			ROOT;		// the root of the tree
		pointer			NIL;		// one non internal node

};
#endif
