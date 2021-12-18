
namespace ft {
	template <typename T>
	class iterator_traits {
		typedef typename T::difference_type		difference_type;
		typedef typename T::value_type			value_type;
		typedef typename T::pointer				pointer;
		typedef typename T::reference			reference;
		typedef typename T::iterator_category	iterator_category;
	};

	template <typename T>
	class iterator_traits <T *> {
		typedef		ptrdiff_t							difference_type;
		typedef		T									value_type;
		typedef 	T*									pointer;
		typedef 	T&									reference;
		typedef		std::random_access_iterator_tag		iterator_categoty;
	};

	template <typename T>
	class iterator_traits <const T*> {
		typedef		ptrdiff_t							difference_type;
		typedef		T									value_type;
		typedef 	const T*							pointer;
		typedef 	const T&							reference;
		typedef		std::random_access_iterator_tag		iterator_categoty;
	};

	template <class Categorie, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct iterator {
		typedef		T									value_type;
		typedef		Distance							difference_type;
		typedef		Pointer								pointer;
		typedef		Reference							reference;
		typedef		Categorie							iterator_category;
	};

	template <typename T>
	class iterator: public iterator <std::random_access_iterator_tag, T> {
		public:
			iterator (): _ptr (nullptr) {}
			iterator (pointer ptr): _ptr (ptr) {}
			iterator (const iterator& iter): _ptr (iter._ptr) {}
			iterator & operator = (const iterator &iter) { _ptr = iter._ptr; return *this; }

			bool operator == (const iterator& iter) const { return _ptr == iter._ptr; }
			bool operator != (const iterator& iter) const { return _ptr != iter._ptr; }


			iterator& operator++ () { ++_ptr; return *this; }
			iterator operator++ (int) { iterator tmp (*this); ++_ptr; return tmp; }

			iterator& operator-- () { --_ptr; return *this; }
			iterator operator-- (int) { iterator tmp (*this); --_ptr; return tmp; }

			difference_type operator - (const iterator& iter) const { return _ptr - iter._ptr; }
			iterator operator - (difference_type n) const {return iterator (_ptr - n); }
			iterator operator + (difference_type n) const { return iterator (_ptr + n); }
			
			friend iterator operator + (difference_type n, const iterator& iter) const { return iterator (_ptr + n); }

			iterator& operator += (difference_type n) { _ptr += n; return *this; }
			iterator& operator -= (difference_type n) { _ptr -= n; return *this; }

			
			bool operator < (const iterator& iter) { return _ptr < iter._ptr; }
			bool operator > (const iterator& iter) { return _ptr > iter._ptr; }
			bool operator <= (const iterator& iter) { return _ptr <= iter._ptr; }
			bool operator >= (const iterator& iter) { return _ptr>= iter._ptr; }
			
			
			reference operator * () const { return *_ptr; }
			pointer operator->() const { return _ptr; }
			reference operator [] (size_t n) { return _ptr[n]; }

		private:
			pointer _ptr;
	};
}

