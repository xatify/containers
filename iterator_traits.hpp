
#ifndef __ITERATOR_TRAITS_HPP__
#define __ITERATOR_TRAITS_HPP__

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
}

#endif