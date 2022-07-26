#pragma once

#include <vector>
#include <algorithm>

namespace UtilSH
{
	/*
	* Remove element from vector
	* This messes up the order of elements
	* This is faster than normal erase (doesn't need iter)
	*/ 
	template<typename T>
	inline void remove_element( std::vector<T>& vec, size_t index )
	{
		// Swap element to be removed with element at back, and pop back
		std::swap( vec[index], vec.back() );
		vec.pop_back();
	}

	/*
	* uses remove_if to remove elements matching predicate over entire container
	*/
	template<class Container, class Pred>
	void remove_erase_if( Container& container, Pred pred )
	{
		// this destroys all elements matching the predicate
		// and fills the spaces with elements from the end
		const auto new_end = std::remove_if( container.begin(), container.end(), pred );
		// erase garbage husk element at end
		container.erase( new_end, container.end() );
	}

	/*
	* First, Try to find element by binary search
	* If no elements, return end
	* Access is a functor used to access the search keys in the elements
	*/
	template<class Iter, typename T, typename Access>
	auto binary_find( Iter begin, Iter end, const T& val,
		Access acc = []( const Iter::value_type& obj )->const Iter::value_type&
		{
			return obj;
		} )
	{
		// Finds the lower bound ( log(last - first) + 1 comparison )
		const auto i = std::lower_bound( begin, end, val,
			[acc]( const Iter::value_type& lhs, const T& rhs )
			{
				return acc( lhs ) < rhs;
			}
		);

		/*
		* if i is end, no element
		* else i >= val
		* if !(i > val) than it must equal
		*/
		if ( i != end && !(val < acc( *i ) ))
		{
			return i;
		}
		else
		{
			return end;
		}
	}
}
