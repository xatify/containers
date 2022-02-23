
#ifndef __VECTOR_ITERATOR_HPP__
#define __VECTOR_ITERATOR_HPP__

#include <iterator>

namespace ft {
    
	template <typename T>
	class Iterator: public std::iterator <std::random_access_iterator_tag, T> {
		public:
            typedef typename std::iterator<std::random_access_iterator_tag, T>::pointer pointer;
			typedef typename std::iterator<std::random_access_iterator_tag, T>::reference reference;
			typedef typename std::iterator<std::random_access_iterator_tag, T>::difference_type difference_type;
			typedef typename std::iterator<std::random_access_iterator_tag, T>::value_type value_type;
			typedef typename std::iterator<std::random_access_iterator_tag, T>::iterator_category iterator_category;
        public:

			Iterator (): _ptr (0x0) {}
			Iterator (pointer  ptr): _ptr (ptr) {}
			Iterator (const Iterator& iter): _ptr (iter._ptr) {}
			Iterator & operator = (const Iterator &iter) { _ptr = iter._ptr; return *this; }

			operator Iterator<const T> () const { return _ptr; };

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
			reference operator [] (difference_type n) const { return _ptr[n]; }

		private:
			pointer _ptr;
	};

	
}

#endif

