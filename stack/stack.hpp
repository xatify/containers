
#ifndef		__STACK_HPP__
#define 	__STACK_HPP__

#include "../vector/vector.hpp"

namespace ft {
	template <class T, class Container = ft::Vector<T> >
	class Stack {
		public:
			// member types
			typedef typename Container::value_type		value_type;
			typedef Container							container_type;
			typedef	typename Container::size_type		size_type;
		
			// public functions
			explicit Stack (const container_type& cntr = container_type ()): c (cntr) {}
			bool empty () { return c.empty (); }
			size_type size () { return c.size (); }
			value_type& top () { return c.back (); }
			const value_type& top () const { return c.back (); }
			void	pop () { c.pop_back (); }
			void push (const value_type& val) { c.push_back (val); }

			template <class TT, class Cntr>
  			friend bool operator < (const Stack<TT, Cntr>& lhs, const Stack<TT, Cntr>& rhs) {
				return lhs.c  < rhs.c;
			}
		
		protected:
			container_type	c;

	};

	// non member functions overloads
	// relational operators
	template <class TT, class Cntr>
	bool operator== (const Stack<TT, Cntr>& lhs, const Stack<TT,Cntr>& rhs) {
		return !(lhs < rhs || rhs < lhs);
	}

	template <class TT, class Cntr>
	bool operator != (const Stack <TT, Cntr>& lhs, const Stack<TT, Cntr>& rhs) {
		return !(lhs == rhs);
	}

	template <class TT, class Cntr>
	bool operator<= (const Stack<TT, Cntr>& lhs, const Stack<TT , Cntr>& rhs) {
		return !(rhs < lhs);
	}

	template <class TT, class Cntr>
	bool operator>  (const Stack<TT, Cntr>& lhs, const Stack<TT, Cntr>& rhs) {
		return (rhs < lhs);
	}

	template <class TT, class Cntr>
	bool operator>= (const Stack<TT, Cntr>& lhs, const Stack<TT, Cntr>& rhs) {
		return !(lhs < rhs); 
	}
	
}

#endif
