#ifndef __RBT_HPP__
#define	__RBT_HPP__

#include <memory>
#include <iostream>

template <	typename Key,
			typename T
			>
struct node {
	typedef std::pair<Key, T> 	value_type;
	enum Color { RED, BLACK }	color;
	
	value_type					value;
	node						*left,
								*right,
								*parent;

	node (): color (BLACK), left (0x0), right (0x0), parent (0x0) {};
	node (const value_type& v): value (v), left (0x0), right (0x0), parent (0x0) {};
	Key& key () { return value.first; };
};

template <	typename Key,
			typename T,
			typename Compare = std::less<Key>
		>
class RBT {

	//****************************************************************************
	// a RED-BLACK tree is a binary search tree with one extra bit of storage per
	// node: it's color (either red or black). by constraining the node colors
	// an any simple path from the root to the leaf, RBT ensures that no such 
	// path is more than twice as long as any other, so that the tree is
	// approximately balanced.
	// the RBT properties:
	// 1: Every node is either red or black
	// 2: The Root is black
	// 3: Every leaf (NIL) is black
	// 4: if a node is red, then both it's children are black
	// 5: for each node, all simple paths from the node to descendant leaves
	//	the same number of black nodes (black hight)
	//*****************************************************************************

	public:
		typedef std::allocator<node<Key, T> >	allocator_type;
		typedef node<Key, T>					node_type;
		typedef typename node_type::Color		Color;
		typedef typename node_type::value_type	value_type;
		typedef node_type *						pointer;
		typedef Key								key_type;
		typedef	Compare							key_compare;
		
	
	public:

		RBT (const key_compare& comp_ = key_compare ()): comp (comp_), alloc (allocator_type ()) {
			NIL = alloc.allocate (1);
			alloc.construct (NIL, node_type ());
			ROOT = NIL;
		}

		RBT& operator = (const RBT& x) {
			if (this != &x) {
				comp = x.comp;
				alloc = x.alloc;
				this->clear (ROOT);
				if (x.ROOT == x.NIL)
					ROOT = NIL;
				else {
					ROOT = x.clone (x.ROOT, NIL);
					ROOT->parent = NIL;
				}
			}
			return (*this);
		}

		~RBT () {
			clear (ROOT);
			alloc.destroy (NIL);
			alloc.deallocate (NIL, 1);
		}

		// clone a new tree from the subtree rooted at x
		// and use new_nil as the new sentinel in case
		// we have different trees 
		pointer clone (pointer x, pointer new_nil) {
			if (x == NIL)
				return new_nil;
			pointer clone = alloc.allocate (1);
			alloc.construct (clone, *x);
			clone->left = clone (x->left, new_nil);
			clone->left->parent = clone;
			clone->right = clone (x->right, new_nil);
			clone->right->parent = clone;
			return x;
		}

		// search the tree for a node with key k
		bool	search (key_type k) {
			pointer x = ROOT;
			while (x != NIL && k != x->key ())
				x = (comp (k, x->key ())) ? x->left: x->right;
			return  (x != NIL);
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
	
		pointer		successor (pointer	x) {					// returns the successor of a node assuming x != nil
			if (x->right != NIL)
				return extrem_left (x->right);
			pointer y = x->parent;
			while (y != NIL && x == y->right) {
				x = y;
				y = y->parent;
			}
			return y;
		}

		pointer		predecessor (pointer x) {					// returns the predecessor of a node assuming x != nil
			if (x->left != NIL)
				return extrem_right (x->left);
			pointer y = x->parent;
			while (y != NIL && x == y->left) {
				x = y;
				y = y->parent;
			}
			return y;
		}

		
		void left_rotate (pointer x) {							// left rotate operation 			
			pointer y = x->right;								// assuming that x.right != nil and the root's parent is nil
			x->right = y->left;									// and the root's parent is nil
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

		void right_rotate (pointer y) {							// right rotate operation
			pointer x = y->left;								// assuming that y.left != nil and the root's parent is nil
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
			alloc.construct (z, node_type (v));
			
			pointer y = NIL;
			pointer x = ROOT;
			while (x != NIL) {
				y = x;
				x = (comp (z->key (), x->key ())) ? x->left: x->right;
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
			z->color = node_type::RED;
			this->fixup (z);
		}

		// to fix up the RBT properties that are violated after inserting z
		// with color RED
		// the only violations that might happen are
		// property 2: the root is black (if z is root)
		// property 4: a red node cann't have red children (if z->parent is red)
		// 
		
		void	fixup (pointer z) {
			while (z->parent->color == node_type::RED) {
				if (z->parent == z->parent->parent->left) {
					pointer y = z->parent->parent->right;					// y the uncle of z
					if (y->color == node_type::RED) {						// case 1: the color of y is red
						z->parent->color = node_type::BLACK;				// we color z.parent and z.uncle in black
						y->color = node_type::BLACK;						// and z.p.p in red, this might violate property 4 between 
						z->parent->parent->color = node_type::RED;			// z.p.p and z.p.p.p so we set z = z.p.p 
						z = z->parent->parent;								// to restore it in the next iteration (we go up 2 levels)
					}
					else { 
						if (z == z->parent->right) {						// case 2: y is black and z is the right child
							z = z->parent;									// we set z = z.p
							left_rotate (z);								// left rotate z to get case 3
						}					
						z->parent->color = node_type::BLACK;				// case 3: y is black and z is the left child
						z->parent->parent->color = node_type::RED;			// then right rotate right z.p.p
						right_rotate (z->parent->parent);					// now the property 4 is maintained 
					}
				}
				else {
					pointer y = z->parent->parent->left;					// same as the above, except 
					if (y->color == node_type::RED) {						// the uncle y is in the left
						z->parent->color = node_type::BLACK;
						y->color = node_type::BLACK;
						z->parent->parent->color = node_type::RED;
						z = z->parent->parent;
					}
					else {
						if (z == z->parent->left) {
							z = z->parent;
							right_rotate (z);
						}
						z->parent->color = node_type::BLACK;
						y->parent->parent->color = node_type::RED;
						left_rotate (z->parent->parent);
					}
				}
			}
			ROOT->color = node_type::BLACK;										// always fix the second property 
		}

		
		// replaces subtree rooted at node u with the 
		// subtree rooted at node v 
		void	transplant (pointer u, pointer v) {
			if (u->parent == NIL)
				ROOT = v;
			else if (u == u->parent->left)
				u->parent->left = v;
			else
				u->parent->right = v;
			v->parent = u->parent;
		}

		// delete a node z from the RBT
		void	remove (pointer z) {
			pointer y = z;
			Color y_original_color = y->color;
			pointer x;
			if (z->left == NIL) {
				x = z->right;
				this->transplant (z, z->right);
			}
			else if (z->right == NIL) {
				x = z->left;
				this->transplant (z, z->left);
			}
			else {
				y = this->extrem_left (z->right);
				y_original_color = y->color;
				x = y->right;
				if (y->parent == z)
					x->parent = y;
				else {
					this->transplant (y, y->right);
					y->right = z->right;
					y->right->parent = y;
				}
				this->transplant (z, y);
				y->left = z->left;
				y->left->parent = y;
				y->color = z->color;
			}
			if (y_original_color == Color::BLCK)
				this->remove_fixeup (x);
			alloc.destroy (z);
			alloc.deallocate (z, 1);
		}

		void	remove_fixup (pointer x) {
			while (x != NIL && x->color == node_type::BLACK) {
				if (x == x->parent->left) {
					pointer w = x->parent->right;
					if (w->color == node_type::RED) {
						w->color = node_type::BLACK;
						x->parent->color = node_type::RED;
						this->left_rotate (x->parent);
						w = x->parent->right;
					}
					if (w->left->color == node_type::BLACK && w->right->color == node_type::BLACK) {
						w->color = node_type::RED;
						x = x->parent;
					}
					else { 
						if (w->right->color == node_type::BLACK) {
							w->left->color = node_type::BLACK;
							w->color = node_type::RED;
							this->right_rotate (w);
							w = x->parent->right;
						}
						w->color = x->parent->color;
						x->parent->color = node_type::BLACK;
						w->right->color = node_type::BLACK;
						this->left_rotate (x->parent);
						x = ROOT;
					}
				}
				else {
					pointer w = x->parent->left;
					if (w->color == node_type::RED) {
						w->color = node_type::BLACK;
						x->parent->color = node_type::RED;
						this->right_rotate (x->parent);
						w = x->parent->left;
					}
					if (w->right->color == node_type::BLACK && w->left->color == node_type::BLACK) {
						w->color = node_type::RED;
						x = x->parent;
					}
					else {
						if (w->left->color == node_type::BLACK) {
							w->right->color = node_type::BLACK;
							w->color = node_type::RED;
							this->left_rotate (w);
							w = x->parent->parent;
						}
						w->color = x->parent->color;
						x->parent->color = node_type::BLACK;
						this->right_rotate (x->parent);
						x = ROOT;
					}
				}
			}
			x->color = node_type::BLACK;
		}

		// capacity functions
		bool empty () { return ROOT == NIL; }

		// number of nodes in the tree
		size_t size () const {
			return size (ROOT);
		}


		// count recursively the number of nodes in the subtree
		// rooted at x
		size_t size (pointer x) const {
			if (x == NIL) return 0;
			return (size (x->right) + size (x->left));
		}
 

		void clear () { clear (ROOT); }									//delete all the elements
	
		void clear (pointer x) {										// delete the subtree rooted at x  except nil								
			if (x != NIL) {
				clear (x->left);
				clear (x->right);
				alloc.destroy (x);
				alloc.deallocate (x, 1);
			}
		}

 	private:
		Compare			comp;		// compare function object for the keys
		allocator_type	alloc;		// allocator
		pointer			ROOT;		// the root of the tree
		pointer			NIL;		// one non internal node

};
#endif
