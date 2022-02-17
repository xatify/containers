
#ifndef __REVERSE_ITERATOR_HPP__
#define __REVERSE_ITERATOR_HPP__

namespace ft {
	
	template <typename  Iterator>
	class reverse_iterator {
		public:
			typedef 	Iterator		iterator_type;
			typedef typename iterator_traits<Iterator>::iterator_category   iterator_category;
            typedef typename iterator_traits<Iterator>::value_type			value_type;
			typedef typename iterator_traits<Iterator>::difference_type		difference_type;
			typedef typename iterator_traits<Iterator>::pointer				pointer;
			typedef typename iterator_traits<Iterator>::reference			reference;
		public:
			reverse_iterator (): iter () {};

			explicit reverse_iterator (iterator_type it): iter (it.base ()) {}

			template <class Iter>
				reverse_iterator (const reverse_iterator<Iter>& rev): iter (rev.base ()) {}
			
			iterator_type base () const { return iter; }				// copy of the base iterator

			reference operator * () const {
				iterator_type it (iter);
				return (*(--it));
			}

			pointer operator-> () const { return &(operator*()); }

			reference operator [] (difference_type n) const { return *(base ()[-n - 1]); } 

			reverse_iterator operator+ (difference_type n) const { return reverse_iterator (iter - n); }
			
			reverse_iterator& operator ++ () {
				--iter;
				return (*this);
			}
			reverse_iterator operator ++ (int) {
				reverse_iterator it (iter);
				++(*this);
				return 	it;
			}

			reverse_iterator& operator += (difference_type n) {
				iter -= n;
				return (*this);
			}

			reverse_iterator operator- (difference_type n) const { return reverse_iterator (iter + n); }
			
			reverse_iterator& operator -- () {
				++iter;
				return (*this);
			}
			reverse_iterator operator -- (int) {
				reverse_iterator it (iter);
				--(*this);
				return 	it;
			}

			reverse_iterator& operator -= (difference_type n) {
				iter += n;
				return (*this);
			}

		private:
			iterator_type	iter;
	};

	template <class Iterator>
        bool operator== (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) { return lhs.base () == rhs.base (); };

    template <class Iterator>
        bool operator!= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) { return lhs.base () != rhs.base (); };
	
    template <class Iterator>
        bool operator<  (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) { return lhs.base () > rhs.base (); };

    template <class Iterator>
        bool operator<= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) { return lhs.base () >= rhs.base (); };

    template <class Iterator>
        bool operator>  (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) { return lhs.base () < rhs.base (); };

    template <class Iterator>
        bool operator>= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) { return lhs.base () <= rhs.base (); }

	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type operator - (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return (lhs.base () - rhs.base ());
	}

	template <class Iterator>
	reverse_iterator<Iterator> operator + (typename reverse_iterator<Iterator>::difference_type n,
											const reverse_iterator<Iterator>& rev_it) {
		return (rev_it + n);
	}
}
#endif