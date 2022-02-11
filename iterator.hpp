
#include <iterator>


namespace ft {
    
	template <typename T>
	struct iterator_traits {
		typedef typename T::difference_type		difference_type;
		typedef typename T::value_type			value_type;
		typedef typename T::pointer				pointer;
		typedef typename T::reference			reference;
		typedef typename T::iterator_category	iterator_category;
	};

	template <typename T>
	struct iterator_traits <T *> {
		typedef		ptrdiff_t							difference_type;
		typedef		T									value_type;
		typedef 	T*									pointer;
		typedef 	T&									reference;
		typedef		std::random_access_iterator_tag		iterator_categoty;
	};

	template <typename T>
	struct iterator_traits <const T*> {
		typedef		ptrdiff_t							difference_type;
		typedef		T									value_type;
		typedef 	const T*							pointer;
		typedef 	const T&							reference;
		typedef		std::random_access_iterator_tag		iterator_categoty;
	};

	template <typename T>
	class Iterator: public std::iterator <std::random_access_iterator_tag, T> {
		public:
            typedef typename std::iterator<std::random_access_iterator_tag, T>::pointer pointer;
			typedef typename std::iterator<std::random_access_iterator_tag, T>::reference reference;
			typedef typename std::iterator<std::random_access_iterator_tag, T>::difference_type difference_type;
			typedef typename std::iterator<std::random_access_iterator_tag, T>::value_type value_type;
			typedef typename std::iterator<std::random_access_iterator_tag, T>::iterator_category iterator_category;
        public:

			Iterator (): _ptr (nullptr) {}
			explicit Iterator (pointer  ptr): _ptr (ptr) {}
			explicit Iterator (const Iterator& iter): _ptr (iter._ptr) {}
			Iterator & operator = (const Iterator &iter) { _ptr = iter._ptr; return *this; }

			operator Iterator<const T> () const { return Iterator<const T> (*this) ; };

			bool operator == (const Iterator& iter) const { return _ptr == iter._ptr; }
			bool operator != (const Iterator& iter) const { return _ptr != iter._ptr; }


			Iterator& operator++ () { ++_ptr; return *this; }
			Iterator operator++ (int) { Iterator tmp (*this); ++_ptr; return tmp; }

			Iterator& operator-- () { --_ptr; return *this; }
			Iterator operator-- (int) { Iterator tmp (*this); --_ptr; return tmp; }

			difference_type operator - (const Iterator& iter) const { return _ptr - iter._ptr; }
			Iterator operator - (difference_type n) const {return Iterator (_ptr - n); }
			Iterator operator + (difference_type n) const { return Iterator (_ptr + n); }
			
			friend Iterator operator + (difference_type n, const Iterator& iter) { return Iterator (iter._ptr + n); }

			Iterator& operator += (difference_type n) { _ptr += n; return *this; }
			Iterator& operator -= (difference_type n) { _ptr -= n; return *this; }

			
			bool operator < (const Iterator& iter) { return _ptr < iter._ptr; }
			bool operator > (const Iterator& iter) { return _ptr > iter._ptr; }
			bool operator <= (const Iterator& iter) { return _ptr <= iter._ptr; }
			bool operator >= (const Iterator& iter) { return _ptr>= iter._ptr; }
			
			
			reference operator * () const { return *_ptr; }
			pointer operator->() const { return _ptr; }
			reference operator [] (size_t n) const { return _ptr[n]; }

		private:
			pointer _ptr;
	};

    template <typename iterator>
    class rev_iterator {
        public:
            typedef iterator    iterator_type;
            typedef typename iterator_traits<iterator>::iterator_category   iterator_category;
            typedef typename iterator_traits<iterator>::value_type			value_type;
			typedef typename iterator_traits<iterator>::difference_type		difference_type;
			typedef typename iterator_traits<iterator>::pointer				pointer;
			typedef typename iterator_traits<iterator>::reference			reference;
		public:
			rev_iterator (): _iter () {};
			explicit rev_iterator (iterator_type it): _iter (it) {}
			template <class iter>
				rev_iterator (const rev_iterator<iter>& rev_it): _iter (rev_it.base ()) {}
			
			iterator_type base () const { return _iter; }
			reference operator *() const {
				iterator it (_iter);
				return (*--it);
			}
			pointer operator->() const { return &(operator *()); };
			reference operator [] (difference_type n) const { return *(*this + n); }

			rev_iterator operator+ (difference_type n) const { return rev_iterator (_iter - n); }
			rev_iterator& operator++ () {
				--_iter;
				return (*this);
			}
			rev_iterator operator++ (int) {
				iterator_type it (_iter);
				--_iter;
				return rev_iterator (it);
			}
			rev_iterator& operator += (difference_type n) {
				_iter -= n;
				return (*this);
			}

			rev_iterator operator- (difference_type n) const {
				return reverse_iterator (_iter + n);
			}
			rev_iterator& operator-- () {
				++_iter;
				return (*this);
			}
			rev_iterator operator-- (int) {
				iterator_type it (_iter);
				++_iter;
				return reverse_iterator (it);
			}
			rev_iterator& operator -= (difference_type n) {
				_iter += n;
				return *this;
			};

		
		private:
			iterator_type _iter;
	};
}

