#ifndef MULTIPLECONTAINER_H
#define MULTIPLECONTAINER_H

#include <vector>

#include "Core/Console/Console.h"

/*
 * This need to be specialized
 * Did not expect to use this directly... 
 * Do nothing and explicitly show an error.
*/

#define MULTIPLE_CONTAINER_TEMPLATE(containerType) \
public: \
	template<typename T> \
	typename std::containerType<T>::iterator begin (); \
	template<typename T> \
	typename std::containerType<T>::iterator end (); \
	template<typename T> \
	typename std::containerType<T>::const_iterator begin () const; \
	template<typename T> \
	typename std::containerType<T>::const_iterator end () const;

/*
 *
*/

#define MULTIPLE_CONTAINER_SPECIALIZATION(containerType, type, class, container) \
template<> \
inline std::containerType<type>::iterator class::begin<type> () \
{ \
	return container.begin (); \
} \
template<> \
inline std::containerType<type>::iterator class::end<type> () \
{ \
	return container.end (); \
} \
template<> \
inline std::containerType<type>::const_iterator class::begin<type> () const \
{ \
	return container.begin (); \
} \
template<> \
inline std::containerType<type>::const_iterator class::end<type> () const \
{ \
	return container.end (); \
}

/*
 * Range based loop macro for iterating over multiple container controller
 * by specifying type of the container.
 *
 * Restrictions: every two containers need to have different types
*/

// TODO: Fix scope

#define for_each_type(type, name, container) \
	for (type name = *(container).begin<type> (); name != nullptr; name = nullptr) \
		for (auto name##_it = (container).begin<type> (); \
			name##_it != (container).end<type> () && (name = *name##_it); \
			name##_it ++)

#endif