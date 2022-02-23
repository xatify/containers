#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include <memory>
#include "iterator.hpp"
#include "../reverse_iterator/reverse_iterator.hpp"
#include "../utility/type_traits.hpp"
#include "../utility/lexicographical.hpp"
#include <iostream>

namespace ft {
	template <typename T, typename Alloc = std::allocator <T> >
	class Vector {
		public:
			typedef Alloc             						allocator_type;
			typedef typename allocator_type::size_type		size_type;
			typedef T                               		value_type;
			typedef Iterator<T>                     		iterator;
			typedef Iterator<const T>               		const_iterator;
			typedef T&					            		reference;
			typedef const T&			            		const_reference;
			typedef reverse_iterator<iterator>      		reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

			explicit Vector (const allocator_type& alloc_ = allocator_type ()): alloc (alloc_) {
				elem = space = last = 0x0;
			}

			explicit Vector (size_type n, const value_type& value = value_type (), 
                            const allocator_type& alloc_ = allocator_type ()): alloc (alloc_) {
				
				elem = alloc.allocate (n);
				space = last = elem + n;
				for (T* p = elem; p != last; ++p)
					alloc.construct (p, value);
			}

			template <typename InputIterator>
				Vector(InputIterator frst, typename enable_if <check<typename std::iterator_traits<InputIterator>::iterator_category>::val, InputIterator>::type lst,
                        const allocator_type& alloc_ = allocator_type ()): alloc (alloc_) {
					elem = space = last = 0x0;
					while (frst != lst)
						push_back (*frst++);
				}
			
			Vector (const Vector& x): alloc (x.alloc) {
                elem = alloc.allocate (x.capacity());
                space = elem + x.size();
                last = elem + x.capacity ();
                size_type i = 0;
                const_iterator it = x.begin ();
                while (it != x.end ())
                    alloc.construct (elem + i++, *it++);
			}
			
			~Vector () {
				clear ();
				alloc.deallocate (elem, last - elem);
			};

			Vector& operator = (const Vector& x) {
                if (&x != this) {
                    clear ();
                    alloc.deallocate (elem, last - elem);
                    elem = space = elem = 0x0;
                    if (x.capacity ()) {
                        elem = alloc.allocate (x.capacity());
                        size_type n = 0;
                        for (const_iterator iter = x.begin (); iter != x.end (); ++iter)
                            alloc.construct (elem + n++, *iter);
                        space = elem + x.size ();
                        last = elem + x.capacity ();
                    }
                }
				return (*this);
 			}

			// iterators
			iterator begin () { return iterator (elem) ; };
			const_iterator begin () const { return const_iterator (elem); };

			iterator end () { return iterator (space); };
			const_iterator end () const { return const_iterator (space); }

			reverse_iterator rbegin () { return reverse_iterator (space) ; };
			const_reverse_iterator rbegin () const { return const_reverse_iterator (space); };

			reverse_iterator rend () { return reverse_iterator (elem); };
			const_reverse_iterator rend () const { return const_reverse_iterator (elem); };

			// capacity
			size_type size () const { return (space - elem); };

			size_type max_size () const { return alloc.max_size (); }

			bool empty () const { return (elem == space); };
			
			size_type capacity () const { return (last - elem); };

			void resize (size_type n, value_type val = value_type ()) {
				size_type sz = size ();
				if (n > sz) {
					while (sz++ < n)
						push_back (val);
				}
				else {
					space = elem + n;
					while (n < sz)
					    alloc.destroy (elem + n++);
				}
			}

			void reserve (size_type n) {
				if (n > max_size())
					throw std::length_error ("length_error");
				else if (n > capacity ()) {
					T* tmp = alloc.allocate (n);
					if (elem == 0x0) {
						elem = space = tmp;
						last = elem + n;
					}
					else {
						size_type sz = size ();
						for (size_type i = 0; i < sz; ++i) {
							alloc.construct (tmp + i, *(elem + i));
							alloc.destroy (elem + i);
						}
						alloc.deallocate (elem, capacity ());
						elem = tmp;
						space = elem + sz;
						last = elem + n;
					}
				}
			}

			// element access
			reference front () { return *elem; };
			const_reference front () const { return *elem; };

			reference back () { return *(space - 1); };
			const_reference back () const { return *(space - 1); };

			reference operator [] (size_type n) { return elem[n]; };
			const_reference operator [] (size_type n) const { return elem[n]; };

			reference at (size_type n) { 
				if (n >= size ()) throw std::out_of_range ("vector");
				return (elem[n]);
			}

			const_reference at (size_type n) const {
				if (n >= size ()) throw std::out_of_range ("vector");
				return (elem[n]);
			};

			// modifiers
			template <class InputIterator>
				void assign (InputIterator first, typename  enable_if<check<typename std::iterator_traits<InputIterator>::iterator_category>::val, InputIterator>::type last) {
					clear ();
					while (first != last) push_back (*first++);
				}
			
			void assign (size_type n, const value_type& val) {
				clear ();
				while (n--) push_back (val);
			}


			iterator insert (iterator position, const value_type& val) {
				size_type diff = position - begin ();
				push_back (val);
				position = begin () + diff;
                iterator it = end () - 1;
				for (; it != position; --it)
					*(it) = *(it - 1);
				*it = val;
				return it;
			}
			
			void insert (iterator position, size_type n, const value_type& val) {
				
				if (size () + n > capacity ()) {
					Vector tmp (begin (), position);
					while (n--)
						tmp.push_back (val);
					while (position != end ())
						tmp.push_back (*position++);
					this->swap (tmp);
				}
				else {
					Vector tmp (position, end ());
					iterator it = tmp.begin ();
					while (position != end () && n) {
						*position++ = val;
						n--;
					}
					if (position == end ()) {
						while (n--)
							alloc.construct (space++, val);
					}
					else if (!n) {
						while (position != end ())
							*position++ = *it++;
					}
					while (it != tmp.end ())
						push_back (*it++);
				}
			}

			template <class InputIterator>
				void insert (iterator position, typename enable_if<check<typename std::iterator_traits<InputIterator>::iterator_category>::val, InputIterator>::type first, InputIterator last) {
					
					Vector range_ (first, last);
					size_type n = range_.size ();

					if (size () + n > capacity ()) {
						Vector new_vec (begin (), position);
						for (iterator it = range_.begin (); it != range_.end (); ++it)
							new_vec.push_back (*it);
						while (position != end ())
							new_vec.push_back (*position++);
						this->swap (new_vec);
					}
					else {
						Vector tmp (position, end ());
						iterator it = tmp.begin ();
						iterator range_it = range_.begin ();
						while (position != end () && range_it != range_.end())
							*position++ = *range_it++;
						if (position == end ()) {
							while (range_it != range_.end())
								alloc.construct (space++, *range_it++);
						}
						else if (range_it == range_.end()) {
							while (position != end ())
								*position++ = *range_it++;
						}
						while (it != tmp.end ())
							push_back (*it++);
					}	
				}
			
			iterator erase (iterator position) {
				alloc.destroy(&(*position));
				for (iterator it = position; it != end () - 1; ++it)
					*it = *(it + 1);
				alloc.destroy(space - 1);
				space -= 1;
				return position;
			}
			
			iterator erase (iterator first, iterator lst) {
				iterator tmp(first);
				iterator t (first);
				size_type s = lst - first;
				while (first != lst)
					alloc.destroy(&(*first++));
				while (lst != end())
					*tmp++ = *lst++;
				while (tmp != end ())
					alloc.destroy (&(*tmp++));
				space -= s;
				return t;
			}

			void push_back (const value_type& val) {
				size_type c = capacity ();
				if (c == 0) {
					elem = alloc.allocate (1);
					space = elem;
					last = elem + 1;
				}
				else if (space == last) {
					c <<= 1;
					T *tmp = alloc.allocate (c);
					for (size_type i = 0; i < static_cast <size_type> (space - elem); ++i) {
						alloc.construct (tmp + i, *(elem + i));
					    alloc.destroy(elem + i);
					}
					alloc.deallocate (elem, space - elem);
					space = (space - elem) + tmp;
					elem = tmp;
					last = elem + c;
				}
				alloc.construct (space++, val);
			}
			
			void pop_back () { alloc.destroy(&(*(--space))); }

			void clear () {
				for (T* f = elem; f != space; ++f)
					alloc.destroy (f);
				space = elem;
			}

			void swap (Vector &v) {
				std::swap (alloc, v.alloc);
				std::swap (elem, v.elem);
				std::swap (space, v.space);
				std::swap (last, v.last);
			}

			// observers
			allocator_type get_allocator () const { return alloc; };
		
		private:
			T*				elem;
			T*				space;
			T* 				last;
			allocator_type	alloc;
	};

	// non_members overloads

	// swap
	template <class T, class Alloc>
  	void swap (Vector<T,Alloc>& x, Vector<T,Alloc>& y) { x.swap (y); }
	
	// relational operators
	template <class T, class Alloc>
  	bool operator== (const Vector<T,Alloc>& lhs, const Vector<T,Alloc>& rhs) {
		if (lhs.size () != rhs.size ()) return false;
		return ft::equal (lhs.begin(), lhs.end(), rhs.begin());
	};

	template <class T, class Alloc>
  	bool operator != (const Vector<T,Alloc>& lhs, const Vector<T,Alloc>& rhs) {
		return (!(lhs == rhs));
	}

	template <class T, class Alloc>
	bool operator <  (const Vector<T,Alloc>& lhs, const Vector<T,Alloc>& rhs) {
		return ft::lexicographical_compare (lhs.begin (), lhs.end(), rhs.begin (), rhs.end ());
	}

	template <class T, class Alloc>
  	bool operator <= (const Vector<T,Alloc>& lhs, const Vector<T,Alloc>& rhs) {
		return !(rhs < lhs);
	}

	template <class T, class Alloc>
  	bool operator >  (const Vector<T,Alloc>& lhs, const Vector<T,Alloc>& rhs) {
		return (rhs < lhs);
	}
	
	template <class T, class Alloc>
	bool operator >= (const Vector<T,Alloc>& lhs, const Vector<T,Alloc>& rhs) {
		return !(lhs < rhs);
	}
}

#endif