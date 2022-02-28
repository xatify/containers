#ifndef __MAP_HPP__
#define	__MAP_HPP__

#include <functional>
#include <memory>
#include <utility>
#include "../reverse_iterator/reverse_iterator.hpp"
#include "map_iterator.hpp"
#include "rbt.hpp"
#include "../utility/lexicographical.hpp"
#include "../pair/pair.hpp"
#include <algorithm>

namespace ft {

	template <
			typename Key,
			typename T,
			typename Compare = std::less<Key>,
			typename Alloc = std::allocator<ft::pair<const Key, T> >
			>
	class Map {

		class value_compare;
		
		public:
			typedef	Key															key_type;
			typedef T															mapped_type;
			typedef ft::pair<const key_type, mapped_type>						value_type;
			typedef Compare														key_compare;
		
		// the type of pointer to the node of the rbt 
		private:
			typedef RBT<value_type, key_compare>								RBT_;
			typedef RBT<const value_type, key_compare>							Const_RBT_;
			typedef typename RBT_::pointer										node_ptr;
		
		public :
			typedef Alloc														allocator_type;
			typedef	typename allocator_type::reference							reference;
			typedef typename allocator_type::const_reference					const_reference;
			typedef typename allocator_type::pointer							pointer;
			typedef typename allocator_type::const_pointer						const_pointer;
			typedef	rbt_iterator<RBT_>											iterator;
			typedef	rbt_iterator<Const_RBT_>									const_iterator;
			typedef reverse_iterator<iterator>									reverse_iterator;
			typedef	ft::reverse_iterator<const_iterator>						const_reverse_iterator;
			typedef typename std::iterator_traits<iterator>::difference_type	difference_type;
			typedef size_t														size_type;
		
		
		public :

			// constructors
			explicit Map (const key_compare& comp_ = key_compare (), 
						const allocator_type& alloc_ = allocator_type ()): comp (comp_), alloc (alloc_) {}

			template <typename InputIterator>
				Map (InputIterator first, InputIterator last,
					const key_compare& comp_ = key_compare (),
					const allocator_type& alloc_ = allocator_type ()): comp (comp_), alloc (alloc_) {
					while (first != last) {
						rbt.insert (*first);
						++first;
					}
				}
			
			Map (const Map& x): comp (x.comp), alloc (x.alloc), rbt (x.rbt) {}

			~Map () {};

			Map& operator= (const Map& x) {
				if (this != &x) { rbt = x.rbt; }
				return *this;
			}

			// iterators
			iterator begin () { return iterator (&rbt, rbt.extrem_left (rbt.root ())); }
			
			const_iterator begin () const { return iterator (&rbt, rbt.extrem_left (rbt.root ())); }
			
			iterator end () { return iterator (&rbt, rbt.nil ()); }

			const_iterator end () const { return iterator (&rbt, rbt.nil ()); }
			
			reverse_iterator rbegin () { return reverse_iterator (end ()); };
			
			const_reverse_iterator rbegin () const { return reverse_iterator (end()); }
			
			reverse_iterator rend () { return reverse_iterator (begin ()); }
			
			const_reverse_iterator rend () const { return reverse_iterator (begin ()); }


			// capacity
			bool empty () { return rbt.empty (); }
			
			size_type size () const { return rbt.size (); }
			
			size_type max_size () const { return alloc.max_size (); }

			// Element access
			mapped_type& operator[] (const key_type& k) {
				return (*((this->insert(ft::make_pair(k,mapped_type()))).first)).second;
			}

			// modifiers
			ft::pair<iterator, bool> insert (const value_type& val) {
				std::pair<node_ptr, bool> p_ = rbt.insert (val);

				return (ft::make_pair(iterator (&rbt, p_.first), p_.second));
			}

			iterator insert (iterator position, const value_type& val) {
				(void)position;
				return iterator (&rbt, (rbt.insert (val)).first);
			}  

			template <typename InputIterator>
			void insert (InputIterator first, InputIterator last) {
				while (first != last) {
					insert (*first);
					++first;
				}
			}
			
			void erase (iterator position) {
				rbt.remove ((*position).first);
			}


			size_type erase (const key_type& k) {
				return rbt.remove (k);
			}
			
			void erase (iterator first, iterator last) {
				rbt.remove (first, last);
			}
			
			void swap (Map& x) {
				std::swap (alloc, x.alloc);
				std::swap (comp, x.comp);
				rbt.swap (x.rbt);
			}

			void clear () { rbt.clear (rbt.root()); }


			// Oberservers
			key_compare 	key_comp () const { return comp; };
			value_compare	value_comp () const { return value_compare (comp); };

			//Operations
			iterator find (const key_type& x) {
				return iterator (&rbt, rbt.search (x));
			}

			const_iterator find (const key_type& k) const {
				return iterator (&rbt, rbt.search (k));
			}
			size_type count (const key_type& k) const {
				return (rbt.search (k) != rbt.nil ());
			}

			iterator lower_bound (const key_type& k) {
				return iterator (&rbt, rbt.lower_bound (k));
			}

			const_iterator lower_bound (const key_type& k) const {
				return iterator (&rbt, rbt.lower_bound (k));
			}
			
			iterator upper_bound (const key_type& k) {
				return iterator (&rbt, rbt.upper_bound (k));
			}

			const_iterator upper_bound (const key_type& k) const {
				return iterator (&rbt, rbt.upper_bound (k));
			}
			
			ft::pair<const_iterator, const_iterator> equal_range (const key_type& k) const {
				return ft::make_pair (lower_bound (k), upper_bound (k));
			}
			
			ft::pair <iterator, iterator> equal_range (const key_type& k) {
				return ft::make_pair (lower_bound (k), upper_bound (k));
			}

			allocator_type get_allocator () const { return alloc; };

		private:
			key_compare			comp;
			allocator_type		alloc;
			RBT_				rbt;

	};

	// Relational operations
	template <class Key, class T, class Compare, class Alloc>
  	bool operator== ( const Map<Key ,T ,Compare ,Alloc>& lhs, const Map<Key, T, Compare,Alloc>& rhs) {
		if (lhs.size () == rhs.size ()) {
			return ft::equal (lhs.begin (), lhs.end (), rhs.begin ());
		}
		return false;
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator!= ( const Map<Key, T, Compare, Alloc>& lhs, const Map< Key, T, Compare, Alloc>& rhs ) {
		return !(lhs == rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
  	bool operator <  ( const Map<Key, T, Compare, Alloc>& lhs, const Map<Key, T, Compare, Alloc>& rhs ) {
		return std::lexicographical_compare (lhs.begin (), lhs.end (), rhs.begin (), rhs.end ());
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator<= ( const Map<Key, T, Compare, Alloc>& lhs, const Map<Key, T, Compare, Alloc>& rhs ) {
		return !(rhs < lhs);
	}

	template <class Key, class T, class Compare, class Alloc>
  	bool operator>  ( const Map<Key, T, Compare, Alloc>& lhs, const Map<Key, T, Compare, Alloc>& rhs ) {
		return 	rhs < lhs;  
	}

	template <class Key, class T, class Compare, class Alloc>
  	bool operator>= ( const Map<Key, T, Compare, Alloc>& lhs, const Map<Key, T, Compare, Alloc>& rhs ) {
		return !(lhs < rhs); 
	}

	// Swap function template
	template <class Key, class T, class Compare, class Alloc>
  	void swap (Map<Key ,T ,Compare ,Alloc>& x, Map<Key, T, Compare, Alloc>& y) {
		x.swap (y);
	}

	// function object template inside map 
	// uses the internal comparison object to generate the appropriate comparison
	// functional class
	template <typename Key, typename T, typename Compare, typename Alloc>
	class Map< Key, T, Compare, Alloc>::value_compare: public std::binary_function<typename Map::value_type, typename Map::value_type, bool> {
		friend class Map;
		
		protected:
			Compare comp;
			value_compare (Compare c): comp (c) {}
		public:
			typedef bool			result_type;
			typedef	value_type		first_argument_type;
			typedef value_type		second_argument_type;
			bool operator () (const value_type& x, const value_type& y) {
				return comp (x.first, y.first);
			}
	};
}

#endif