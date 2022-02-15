
#ifndef __STACK_HPP__
#endif  __STACK_HPP__

#include "vector.hpp"

namespace ft {
	template <class T, class Container = vector<T>>
	class stack {
		protected;
			container_type	c;
		public:
			// member types
			typedef Container::value_type		value_type;
			typedef Container					Container_type;
			typedef	Container::size_type		size_type;
			typedef Container::reference		reference;
			typedef Container::const_reference	const_refernce;
		
			// public functions
			explicit stack (const container_type& cntr = container_type ()): c (cntr) {}
			bool empty () { return c.empty (); }
			size_type size () { return c.size (); }
			value_type& top () { return c.back (); }
			value_type& top () const { return c.back (); }
			void	pop () { c.pop_back (); }
			void push (const value_type& val) { c.push_back (val); }
			template <class TT, class Container>
  			friend bool operator== (const stack<TT,Container>& lhs, const stack<TT,Container>& rhs) {
				return lhs.c == rhs.c;
			}

			template <class TT, class Container>
  			friend bool operator!= (const stack<TT,Container>& lhs, const stack<TT,Container>& rhs) {
				return !(lhs == rhs);
			}

			template <class TT, class Container>
  			friend bool operator<  (const stack<TT,Container>& lhs, const stack<TT,Container>& rhs) {
				return (lhs.c < rhs.c);
			}

			template <class TT, class Container>
  			friend bool operator<= (const stack<TT,Container>& lhs, const stack<TT ,Container>& rhs) {
				return !(rhs < lhs);
			}

			template <class TT, class Container>
			friend bool operator>  (const stack<TT,Container>& lhs, const stack<TT,Container>& rhs) {
				return (rhs < lhs);
			}

			template <class TT, class Container>
  			friend bool operator>= (const stack<TT,Container>& lhs, const stack<TT,Container>& rhs) {
				return !(lhs < rhs); 
			}

	};

	// non member functions overloads
	// relational operators
}
#endif
