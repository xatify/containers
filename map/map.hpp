#ifndef __MAP_HPP__
#define	__MAP_HPP__

#include <functional>
#include <memory>
#include <utility>
#include "../reverse_iterator/reverse_iterator.hpp"
#include "map_iterator.hpp"
#include "rbt.hpp"

namespace ft {

	template <
			typename Key,
			typename T,
			typename Compare = std::less<Key>,
			typename Alloc = std::allocator<std::pair<const Key, T> >
			>
	class map {

		class value_compare;
		
		public:
			typedef	Key															key_type;
			typedef T															mapped_type;
			typedef std::pair<const key_type, mapped_type>						value_type;
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
			explicit map (const key_compare& comp_ = key_compare (), 
						const allocator_type& alloc_ = allocator_type ()): comp (comp_), alloc (alloc_) {}

			template <typename InputIterator>
				map (InputIterator first, InputIterator last,
					const key_compare& comp_ = key_compare (),
					const allocator_type& alloc_ = allocator_type ()): comp (comp_), alloc (alloc_) {
					while (first != last) {
						rbt.insert (*first);
						++first;
					}
				}
			
			map (const map& x): comp (x.comp), alloc (x.alloc), rbt (x.rbt) {}

			~map () {};

			map& operator= (const map& x) {
				if (this != &x) { rbt = x.rbt; }
				return *this;
			}

			// iterators
			iterator begin () { return iterator (&rbt, rbt.extrem_left (rbt.root ())); }
			
			const_iterator begin () const { return const_iterator (begin ()); }
			
			iterator end () { return iterator (&rbt, rbt.nil ()); }

			const_iterator end () const { return const_iterator (&rbt, rbt.nil ()); }
			
			reverse_iterator rbegin () { return reverse_iterator (end ()); };
			
			const_reverse_iterator rbegin () const { return reverse_iterator (end()); }
			
			reverse_iterator rend () { return reverse_iterator (begin ()); }
			
			const_reverse_iterator rend () const { return reverse_iterator (begin ()); }


			// capacity
			bool empty () { return rbt.empty (); }
			
			size_type size () const { return rbt.size (rbt.root ()); }
			
			size_type max_size () const { return alloc.max_size (); }

			// Element access
			mapped_type& operator[] (const key_type& k) {
				return (*((this->insert(std::make_pair(k,mapped_type()))).first)).second;
			}

			// modifiers
			std::pair<iterator, bool> insert (const value_type& val) {
				std::pair<node_ptr, bool> p_ = rbt.insert (val);

				return (std::make_pair(iterator (&rbt, p_.first), p_.second));
			}

			iterator insert (iterator position, const value_type& val) {
				return iterator (&rbt, (rbt.insert (val)).first);
			}  

			template <typename InputIterator>
			void insert (InputIterator first, InputIterator last) {
				while (first != last)
					insert (*first++);
			}
			
			void erase (iterator position) {
				rbt.remove ((*position).first);
			}


			size_type erase (const key_type& k) {
				rbt.remove (k);
			}
			
			void erase (iterator first, iterator last) {
				rbt.remove (first, last);
			}
			
			void swap (map& x) {
				std::swap (alloc, x.alloc);
				std::swap (comp, x.comp);
				std::swap (rbt, x.rbt);
			}

			void clear () { rbt.clear (rbt.root()); }


			// Oberservers
			key_compare 	key_comp () const { return comp; };
			value_compare	value_comp () const { return value_compare (); };

			//Operations
			iterator find (const key_type& x) {
				return iterator (&rbt, rbt.find (x));
			}

			const_iterator find (const key_type& k) const {
				return const_iterator (&rbt, rbt.find (k));
			}
			size_type count (const key_type& k) const {
				return (rbt.search (k) != rbt.nil ());
			}

			iterator lower_bound (const key_type& k) {
				return iterator (&rbt, rbt.lower_bound (k));
			}

			const_iterator lower_bound (const key_type& k) const {
				return const_iterator (&rbt, rbt.lower_bound (k));
			}
			
			iterator upper_bound (const key_type& k) {
				return iterator (&rbt, rbt.upper_bound (k));
			}

			const_iterator upper_bound (const key_type& k) const {
				return const_iterator (&rbt, rbt.upper_bound (k));
			}
			
			std::pair<const_iterator, const_iterator> equal_range (const key_type& k) const {
				return std::make_pair (lower_bound (k), upper_bound (k));
			}
			
			std::pair <iterator, iterator> equal_range (const key_type& k) {
				return std::make_pair (lower_bound (k), upper_bound (k));
			}

			allocator_type get_allocator () const { return alloc; };

		private:
			key_compare			comp;
			allocator_type		alloc;
			RBT_				rbt;

	};

	// Relational operations
	template <class Key, class T, class Compare, class Alloc>
  	bool operator== ( const map<Key ,T ,Compare ,Alloc>& lhs, const map<Key, T, Compare,Alloc>& rhs);

	template <class Key, class T, class Compare, class Alloc>
	bool operator!= ( const map<Key, T, Compare, Alloc>& lhs, const map< Key, T, Compare, Alloc>& rhs );

	template <class Key, class T, class Compare, class Alloc>
  	bool operator<  ( const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs );

	template <class Key, class T, class Compare, class Alloc>
	bool operator<= ( const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs );

	template <class Key, class T, class Compare, class Alloc>
  	bool operator>  ( const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs );

	template <class Key, class T, class Compare, class Alloc>
  	bool operator>= ( const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs );

	// Swap function template
	template <class Key, class T, class Compare, class Alloc>
  	void swap (map<Key ,T ,Compare ,Alloc>& x, map<Key, T, Compare, Alloc>& y);

	// function object template inside map 
	// uses the internal comparison object to generate the appropriate comparison
	// functional class
	template <typename Key, typename T, typename Compare, typename Alloc>
	class map< Key, T, Compare, Alloc>::value_compare {
		friend class map;
		
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