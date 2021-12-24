
#include <memory>

namespace ft {
    template <typename T, typename Alloc = std::allocator <T> >
    class vector {
		public:
            typedef std::allocator<T>   allocator_type;
            typedef size_t              size_type;
            typedef T                   value_type;
            typedef Iterator<T>         iterator;
            typedef Iterator<const T>   const_iterator;
			typedef T&					reference;
			typedef const T&			const_reference;

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
				if (x.empty ())
					elem = space = last = nullptr;
			}
			
			~vector () {
					for (T* p = elem; p != last; ++p)
						~(*p);
					alloc.deallocate (elem, last - elem);
			};

			vector& operator = (const vector& x) {

			};


			// iterators
			iterator begin ();
			const_iterator begin () const;

			iterator end ();
			const_iterator end () const;

			reverse_iterator rbegin ();
			const_reverse_iterator rbegin () const;

			reverse_iterator rend ();
			const_reverse_iterator rend () const;

			// capacity
			size_type size () const;
			size_type max_size () const;

			bool empty () const;

			void resize (size_type n, value_type val = value_type ());

			size_type capacity () const;

			void reserve (size_type n);

			// element access
			reference front ();
			const_reference front () const;

			reference back ();
			const_reference back () const;

			reference operator [] (size_type n);
			const_reference operator [] (size_type n) const;

			reference at (size_type n);
			const_reference at (size_type n);

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
			allocator_type get_allocator () const;
		
		private:
			T*				elem;
			T*				space;
			T* 				last;
			allocator_type	alloc;
    };


}