#ifndef __MAP_HPP__
#define	__MAP_HPP__

#include <functional>
#include <memory>
#include <utility>
#include "rbt.hpp"

namespace ft {

	template <typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<std::pair<const Key, T> > >
	class map {

		class value_compare;

		public :
			typedef	Key															key_type;
			typedef T															mapped_type;
			typedef std::pair<const key_type, mapped_type>						value_type;
			typedef Compare														key_compare;
			typedef Alloc														allocator_type;
			typedef	typename allocator_type::reference							reference;
			typedef typename allocator_type::const_reference					const_reference;
			typedef typename allocator_type::pointer							pointer;
			typedef typename allocator_type::const_pointer						const_pointer;
			//typedef															iterator
			//typedef															const_iterator
			//typedef															reverse_iterator
			//typedef															const_reverse_iterator
			typedef typename std::iterator_traits<iterator>::difference_type	difference_type;
			typedef size_t														size_type;
		
		public :

			// constructors
			explicit map (const key_compare& comp_ = key_compare (), 
						const allocator_type& alloc_ = allocator_type ()): comp (comp_), alloc (alloc_), rbt () {}

			template <typename InputIterator>
				map (InputIterator first,
					InputIterator last,
					const key_compare& comp = key_compare (),
					const allocator_type& alloc = allocator_type ()));
			
			map (const map& x) {
				comp = x.comp;
				alloc = x.alloc;
				rbt = x.rbt;
			}

			~map () {};

			map& operator= (const map& x) {
				if (this != &x) {
					comp = x.comp;
					alloc = x.alloc;
					rbt = x.rbt;
				}
				return *this;
			}

			// iterators
			//iterator begin ();
			//const_iterator begin ();
			//iterator end ();
			//const_iterator end () const;
			//reverse_iterator rbegin ();
			//const_reverse_iterator rbegin () const;
			//reverse_iterator rend ();
			//const_reverse_iterator rend () const;


			// capacity
			bool empty () { return rbt.empty (); }
			size_type size () const { return rbt.size (rbt.root ()); }
			size_type max_size () const { return alloc.max_size (); }

			// Element access
			mapped_type& operator[] (const key_type& k);

			// modifiers
			//std::pair<iterator, bool> insert (const value_type& val);
			//iterator insert (iterator position, const value_type& val);
			//template <typename InputIterator>
			//	void insert (InputIterator first, InputIterator last);
			
			//void erase (iterator position);
			size_type erase (const key_type& k);
			//void erase (iterator first, iterator last);
			
			void swap (map& x) {
				std::swap (alloc, x.alloc);
				std::swap (comp, x.comp);
				std::swap (rbt, x.rbt)
			}

			void clear () { rbt.clear (rbt.root()); }


			// Oberservers
			key_compare key_comp () const { return comp; };
			value_compare  value_comp () const { return value_compare (); };

			// Operations
			//iterator find (const key_type& x);
			//const_iterator find (const ke_type& k) const;
			size_type count (const key_type& k) const;
			//iterator lower_bound (const key_type& k);
			//const_iterator lower_bound (const key_type& k) const;
			//iterator upper_bound (const key_type& k);
			//const_iterator upper_bound (const key_type& k) const;
			//std::pair<const iterator, const_iterator> equal_range (const key_type& k) const;
			//pair <iterator, iterator> equal_range (const key_type& k);

			allocator_type get_allocator () const { return alloc; };

		private:
			key_compare			comp;
			allocator_type		alloc;
			RBT <key_type, mapped_type, comp> rbt;

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
			value_compare (Compare c): comp () {}
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