
#include <memory>
#include "iterator.hpp"

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
            typedef reverse_iterator<iterator>      		reverse_iterator;
            typedef reverse_iterator<const_iterator			const_reverse_iterator;

			explicit vector (const allocator_type& alloc_ = allocator_type (): alloc (alloc) {
				elem = space = last = nullptr;
			}

			explicit vector (size_type n, const value_type& value = value_type (), const allocator_type& alloc_ = allocator_type ())): alloc (alloc_) {
				elem = alloc.allocate (n);
				space = last = elem + n;
				for (T* p = elem; p != last; ++p)
					alloc.construct (p, value);
			}

			template <typename InputIterator>
				vector(InputIterator first, InputIterator last, const allocator_type& alloc_ = allocator_type ()): alloc (alloc_) {
					size_type n = 0;
					for (InputIterator f = first; f != last; ++f)
						n++
					elem = alloc.allocate (n);
					space = last = elem + n;
					for (size_type n = 0; first != last; ++first, ++n) {
						alloc.construct (elem + n, *first);
					}
				}
			
			vector (const vector& x): alloc (x.alloc) {
				if (!x.capacity ())
					elem = space = last = nullptr;
				else {
					elem = alloc.allocate (x.capacity());
					space = elem + x.size();
					last = elem + x.capacity ();
					for (x.iter it = x.begin (); it != it.end (); ++it) {
						alloc.construct (elem + (it - x.begin ()), *it);
					}
				}
			}
			
			~vector () {
					for (T* p = elem; p != space; ++p)
						~(*p);
					alloc.deallocate (elem, last - elem);
			};

			vector& operator = (const vector& x) {
				for (iterator it = being (); it != last (); ++iter)
					~(*iter);
				alloc.deallocate (elem, last - elem);
				elem = space = elem = nullptr;
				if (x.capacity ()) {
					elem = alloc.allocate (x.capacity());
					size_type n = 0;
					for (iterator iter = x.being (); iter != x.end (); ++iter) {
						alloc.construct (elem + n, *iter);
						++n;
					}
					iterator rhs = x.begin ();
					n = 0;
					while (rhs != x.end ())
						alloc.construct (elem + n++, *rhs++);
					space = elem + n;
					last = elem + x.capacity ();
				}
				return (*this);
 			};


			// iterators
			iterator begin () { return elem; };
			const_iterator begin () const { return elem; };

			iterator end () { return space; };
			const_iterator end () const { return space; }

			reverse_iterator rbegin () { return space; };
			const_reverse_iterator rbegin () const { return space; };

			reverse_iterator rend () { return elem - 1; };
			const_reverse_iterator rend () const { return elem - 1; };

			// capacity
			size_type size () const { return (space - elem); };

			size_type max_size () const { return alloc.max_size (); }

			bool empty () const { return (elem == space) };
			
			size_type capacity () const { return (last - elem); };

			void resize (size_type n, value_type val = value_type ()) {
				
			}

			void reserve (size_type n);

			// element access
			reference front () { return *elem; };
			const_reference front () const { return *elem; };

			reference back () { return *space; };
			const_reference back () const { return *space; };

			reference operator [] (size_type n) { return elem[i]; };
			const_reference operator [] (size_type n) const { return elem[i]; };

			reference at (size_type n) { 
				if (n >= (space - elem)) throw std::out_of_range ();
				return (elem[i]);
			}

			const_reference at (size_type n) {
				if (n >= (space - elem)) throw std::out_of_range ();
				return (elem[i]);
			};

			// modifiers
			template <class InputIterator>
				void assign (InputIterator first, InputIterator last);
			
			void assign (size_type n, const value_type& val);

			iterator insert (iterator position, const value_type& val);
			
			void insert (iterator position, size_type n, const value_type& val);

			template <class InputIterator>
				void insert (iterator position, InputIterator first, InputIterator last);
			
			iterator erase (iterator position);
			
			iterator erase (iterator first, iterator last);

			void push_back (const value_type& val);
			
			void pop_back ();

			void clear ();

			void swap (vector &v);

			// observers
			allocator_type get_allocator () const { return alloc; };
		
		private:
			T*				elem;
			T*				space;
			T* 				last;
			allocator_type	alloc;
	};

}