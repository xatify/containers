
#include <memory>
#include "iterator.hpp"
#include <algorithm>

namespace ft {
	template <typename T, typename Alloc = std::allocator <T> >
	class vector {
		public:
			typedef std::allocator<T>              			allocator_type;
			typedef size_t                          		size_type;
			typedef T                               		value_type;
			typedef Iterator<T>                     		iterator;
			typedef Iterator<const T>               		const_iterator;
			typedef T&					            		reference;
			typedef const T&			            		const_reference;
			typedef rev_iterator<iterator>      		    reverse_iterator;
			typedef rev_iterator<const_iterator>		    const_reverse_iterator;

			explicit vector (const allocator_type& alloc_ = allocator_type ()): alloc (alloc_) {
				elem = space = last = 0x0;
			}

			explicit vector (size_type n, const value_type& value = value_type (), const allocator_type& alloc_ = allocator_type ()): alloc (alloc_) {
				elem = alloc.allocate (n);
				space = last = elem + n;
				for (T* p = elem; p != last; ++p)
					alloc.construct (p, value);
			}

			template <typename InputIterator>
				vector(InputIterator first, InputIterator last, const allocator_type& alloc_ = allocator_type ()): alloc (alloc_) {
					size_type n = 0;
					for (InputIterator f = first; f != last; ++f)
						n++;
					elem = alloc.allocate (n);
					space = last = elem + n;
					for (size_type n = 0; first != last; ++first, ++n) {
						alloc.construct (elem + n, *first);
					}
				}
			
			vector (const vector& x): alloc (x.alloc) {
				if (!x.capacity ())
					elem = space = last = 0x0;
				else {
					elem = alloc.allocate (x.capacity());
					space = elem + x.size();
					last = elem + x.capacity ();
					size_type i = 0;
					iterator it = x.begin ();
					while (it != it.end ())
						alloc.construct (elem + i++, *it++);
				}
			}
			
			~vector () {
				clear ();
				alloc.deallocate (elem, last - elem);
			};

			vector& operator = (const vector& x) {
				clear ();
				alloc.deallocate (elem, last - elem);
				elem = space = elem = 0x0;
				if (x.capacity ()) {
					elem = alloc.allocate (x.capacity());
					size_type n = 0;
					for (iterator iter = x.begin (); iter != x.end (); ++iter)
						alloc.construct (elem + n++, *iter);
					space = elem + x.size ();
					last = elem + x.capacity ();
				}
				return (*this);
 			}

			// iterators
			iterator begin () { return iterator (elem) ; };
			const_iterator begin () const { return const_iterator (elem); };

			iterator end () { return iterator (space); };
			const_iterator end () const { return const_iterator (space); }

			reverse_iterator rbegin () { return space; };
			const_reverse_iterator rbegin () const { return space; };

			reverse_iterator rend () { return elem - 1; };
			const_reverse_iterator rend () const { return elem - 1; };

			// capacity
			size_type size () const { return (space - elem); };

			size_type max_size () const { return alloc.max_size (); }

			bool empty () const { return (elem == space); };
			
			size_type capacity () const { return (last - elem); };

			void resize (size_type n, value_type val = value_type ()) {
				size_type size = space - elem;
				if (n > size) {
					while (size++ < n)
						push_back (val);
				}
				else {
					space = elem + n;
					while (n++ < size)
						~(*(elem + n));
				}
			}

			void reserve (size_type n) {
				if (n > max_size())
					throw std::length_error ("length_error");
				else if (n > capacity ()) {
					T* tmp = alloc.allocate (n);
					if (elem == nullptr) {
						elem = space = tmp;
						last = elem + n;
					}
					else {
						size_type sz = size ();
						for (size_type i = 0; i < sz; ++i) {
							alloc.construct (tmp + i, *(elem + i));
							~(*(elem + i));
						}
						alloc.deallocate (elem, capacity ());
						elem = tmp;
						space = elem + sz;
						last = elem + n;
					}
				}
			};

			// element access
			reference front () { return *elem; };
			const_reference front () const { return *elem; };

			reference back () { return *space; };
			const_reference back () const { return *space; };

			reference operator [] (size_type n) { return elem[n]; };
			const_reference operator [] (size_type n) const { return elem[n]; };

			reference at (size_type n) { 
				if (n >= (space - elem)) throw std::out_of_range ("out of range");
				return (elem[n]);
			}

			const_reference at (size_type n) const {
				if (n >= (space - elem)) throw std::out_of_range ("out of range");
				return (elem[n]);
			};

			// modifiers
			template <class InputIterator>
				void assign (InputIterator first, InputIterator last) {
					clear ();
					while (first != last) {
						push_back (*first);
						++first;
					}
				}
			
			void assign (size_type n, const value_type& val) {
				clear ();
				while (n--)
					push_back (val);
			}

			iterator insert (iterator position, const value_type& val) {
				size_type diff = position - begin ();
				push_back (val);
				position = begin () + diff;
                iterator it = end ();
				for (; it != position; --it) {
					*(it) = *(it - 1);
				}
				*it = val;
				return it;
			}
			
			void insert (iterator position, size_type n, const value_type& val) {
				size_type diff = position - begin ();
				if (n + size () > capacity ())
					reserve (n + size ());
				position = begin () + diff;
				for (iterator it = end () - 1; it >= position; --it) {
					alloc.construct (&(*it) + n, *it);
				}
				space += n;
				while (n--)
					*position++ = val;
			}

			template <class InputIterator>
				void insert (iterator position, InputIterator first, InputIterator last) {
					InputIterator tmp = first;
					size_type sz = 0;
					while (first++ != last)
						++sz;
					first = tmp;
					size_type pos = position - begin ();
					if (sz + size () > capacity ())
						reserve (sz + size ());
					position = begin () + pos;
					for (iterator it = end () - 1; it >= position; --it) {
						alloc.construct (&(*it) + sz, *it);
					}
					while (sz--)
						*position++ = *first++;
				}
			
			iterator erase (iterator position) {
				~(*position);
				for (iterator it = position; it != end () - 1; ++it)
					*it = *(it + 1);
				~(*(end () - 1));
				space -= 1;
				return position;
			}
			
			iterator erase (iterator first, iterator lst) {
				iterator tmp (first);
				size_type i = 0;
				while (first != lst) {
					~(*first++);
					++i;
				}
				while (lst != end())
					*tmp++ = *lst++;
				while (tmp != end ())
					~(*tmp++);
				space -= i;
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
					for (size_type i = 0; i < space - elem; ++i) {
						alloc.construct (tmp + i, *(elem + i));
					    alloc.destroy(elem + i);
					}
					space = (space - elem) + tmp;
					alloc.deallocate (elem, 1);
					elem = tmp;
					last = elem + c;
				}
				alloc.construct (space++, val);
			}
			
			void pop_back () {
				~(*(--space));
			}

			void clear () {
				for (T* f = elem; f != space; ++f)
					alloc.destroy (f);
				space = elem;
			}

			void swap (vector &v) {
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

}