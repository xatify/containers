#ifndef __RBT_HPP__
#define	__RBT_HPP__

#include "map_iterator.hpp"
#include <memory>

namespace ft {
	
	template <typename Pair>
	struct node {
		
		typedef Pair 				value_type;
		value_type					value;
		enum Color { RED, BLACK }	color;
		node						*left, *right, *parent;
		
		node (const value_type& v = value_type ()): value (v), color (BLACK) {};
		typename Pair::first_type & key () { return value.first; };
	};

	template < typename Pair, 
			typename Compare = std::less<typename Pair::first_type>
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
			typedef node<Pair>						node_type;
			typedef std::allocator<node_type>		allocator_type;
			typedef typename node_type::value_type	value_type;
			typedef node_type *						pointer;
			typedef typename Pair::first_type		key_type;
			typedef	Compare							key_compare;
			typedef	rbt_iterator<RBT>				iterator;
			
		public:

			RBT (const key_compare& comp_ = key_compare ()): comp (comp_), alloc (allocator_type ()) {
				NIL = alloc.allocate (1);
				alloc.construct (NIL, node_type ());
				ROOT = NIL;
			}

			RBT (const RBT& x): comp (x.comp), alloc (x.alloc) {
				NIL = alloc.allocate (1);
				alloc.construct (NIL, node_type ());
				ROOT = x.clone (x.ROOT, NIL, this->alloc);
				ROOT->parent = NIL;
			}

			RBT& operator = (const RBT& x) {
				if (this != &x) {
					comp = x.comp;
					alloc = x.alloc;
					this->clear (ROOT);					// delete all internal nodes
					ROOT = x.clone (x.ROOT, NIL, this->alloc);
					ROOT->parent = NIL;
				}
				return (*this);
			}

			~RBT () {
				clear (ROOT);
				alloc.destroy (NIL);
				alloc.deallocate (NIL, 1);
			}

			// search the tree for a node with key k
			pointer		search (key_type& k) const {
				pointer x = ROOT;
				while (x != NIL) {
					if (comp (k, x->key ())) 						// k < x.key
						x = x->left;				
					else if (comp (x->key (), k))					// x.key < k
						x = x->right;
					else											// !(k < x.key) && !(x.key < k) <=> k == x.key
						break;
				}
				return  (x);
			}

			// Returns a pointer  to the first element in 
			// the tree whose key is not considered to go before k 
			// (i.e., either it is equivalent or goes after).
			pointer lower_bound (key_type& k) const {
				pointer x = ROOT;

				pointer y = NIL;
				while (x != NIL) {
					if (comp (k, x->key ())) {
						y = x;
						x = x->left;
					}
					else if (comp (x->key (), k))
						x = x->right;
					else
						return x;
				}
				return y;
			}

			// Returns a pointer to the first element 
			// in the tree whose key is considered to go after k.
			pointer upper_bound (key_type& k) const {
				pointer lb = lower_bound (k);

				if (!comp (lb->key (), k) && !comp (k, lb->key ()))
					return successor (lb);
				return lb;
			}

			// returns the deepest left node in sub-binay tree rooted at x
			pointer		extrem_left (pointer x) const {
				if (x == NIL) return x;
				while (x->left != NIL)
					x = x->left;
				return x;
			}

			// returns the deepest right node in sub-binay tree rooted at x
			pointer		extrem_right (pointer x) const {
				if (x == NIL) return x;
				while (x->right != NIL)
					x = x->right;
				return x;
			}
		
			pointer		successor (pointer	x) const {					// returns the successor of a node assuming x != nil
				if (x == NIL) return x;
				if (x->right != NIL)
					return extrem_left (x->right);
				pointer y = x->parent;
				while (y != NIL && x == y->right) {
					x = y;
					y = y->parent;
				}
				return y;
			}

			pointer		predecessor (pointer x) const {					// returns the predecessor of a node assuming x != nil
				if (x == NIL) return x;
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
				else if (y == y->parent->right)
					y->parent->right = x;
				else
					y->parent->left = x;
				x->right = y;
				y->parent = x;
			}

			// construct a node z from the pair v
			// we insert the node z in the tree as an oridnary binary tree
			// then we color z red
			// to guarantee that the red-black properties are preserved
			// we call the Insert_fixup function on z to recolor nodes and 
			// perform rotations

			std::pair <pointer, bool> insert (const value_type& v) {
				
				pointer z = alloc.allocate (1);
				alloc.construct (z, node_type (v) );
				
				pointer x = ROOT;
				pointer y = NIL;
				while (x != NIL) {
					y = x;
					if (comp (z->key (), x->key ()))
						x = x->left;
					else if (comp (x->key (), z->key ()))
						x = x->right;
					else {
						alloc.destroy (z);
						alloc.deallocate (z, 1);
						return std::make_pair (x, false);
					}
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
				return (std::make_pair (z, true));
			}

			// to fix up the RBT properties that are violated after inserting z
			// with color RED
			// the only violations that might happen are
			// property 2: the root is black (if z is root)
			// property 4: a red node cann't have red children (if z->parent is red)
			
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
							z->parent->parent->color = node_type::RED;
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

			// delete a an element using the key if it exists
			void	remove (key_type& k) {
				pointer z = search (k);

				if (z != NIL)
					remove (z);
			}
			
			// remove a range of node between iterator first last
			void remove (iterator first, iterator last) {
				pointer first_node	= first.get_node ();
				pointer last_node	= last.get_node ();


				while (first_node != last_node) {
					pointer next_node = successor (first_node);
					remove (first_node);

					first_node = next_node;
				}
			}	
			// delete a node z from the RBT
			void	remove (pointer z) {
				pointer y = z;
				typename node_type::Color y_original_color = y->color;
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
				if (y_original_color == node_type::BLACK)
					this->remove_fixup (x);
				alloc.destroy (z);
				alloc.deallocate (z, 1);
			}
			

			// fix up the invariants of the rbt
			// after removing a node
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
			bool empty () const { return ROOT == NIL; }

			// count recursively the number of nodes 
			//in the subtree rooted at x
			size_t size (pointer x) const {
				if (x == NIL) return 0;
				return (size (x->right) + size (x->left) + 1);
			}
	
			pointer		root () const { return ROOT; }						// return the root of the rbt
			pointer		nil () const { return NIL; }						// return the nil node
		

			void clear (pointer x) {										// delete the subtree rooted at x  except nil
				if (x != NIL) {
					clear (x->left);
					clear (x->right);
					alloc.destroy (x);
					alloc.deallocate (x, 1);
				}
				if (x == ROOT)
					ROOT = NIL;
			}
		private :
			// clone a new tree from the subtree rooted at x
			// and use new_nil as the new sentinel in case
			// we have different trees
			pointer clone (pointer x, pointer new_NIL, allocator_type& alloc_) const {
				if (x == NIL)
					return new_NIL;
				pointer clone_ = alloc_.allocate (1);
				alloc_.construct (clone_, *x);
				clone_->left = clone (x->left, new_NIL, alloc_);
				if (clone_->left != new_NIL)
					clone_->left->parent = clone_;
				clone_->right = clone (x->right, new_NIL, alloc_);
				if (clone_->right != new_NIL)
					clone_->right->parent = clone_;
				return clone_;
			}

		private:
			Compare			comp;		// compare function object for the keys
			allocator_type	alloc;		// allocator
			pointer			ROOT;		// the root of the tree
			pointer			NIL;		// one non internal node
	};
}
#endif
