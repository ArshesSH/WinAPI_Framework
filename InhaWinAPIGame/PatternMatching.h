#pragma once

#include <unordered_map>
#include <functional>
#include <typeindex>
#include <type_traits>
#include "Collider.h"

// Create TypePair
using TypePair = std::pair<const std::type_index, const std::type_index>;

// Create Hash for map
namespace std
{
	template<>
	struct hash<TypePair>
	{
		size_t operator()( const TypePair& tp ) const
		{
			const auto hash0 = tp.first.hash_code();
			return hash0 ^ (tp.second.hash_code() + 0x9e3779b9 + (hash0 << 6) + (hash0 >> 2));
		}
	};
}

template <class C>
class PatternMatching
{
public:
	// Get TypeTrait T and U, and Functor for F, Create Both case of T,U and U,T 
	template<typename T, typename U, typename F>
	void Case( F f )
	{
		handlers[{typeid(T), typeid(U)}] = f;
		handlers[{typeid(U), typeid(T)}] = std::bind(
			f, std::placeholders::_2, std::placeholders::_1
		);
	}
	template<typename T, typename U>
	bool HasCase() const
	{
		return handlers.count( { typeid(T),typeid(U) } ) > 0;
	}
	template<typename T, typename U>
	void ClearCase()
	{
		handlers.erase( { typeid(T),typeid(U) } );
		handlers.erase( { typeid(U),typeid(T) } );
	}
	template<typename F>
	void Default( F f )
	{
		def = f;
	}
	inline void Switch( C& a, C& b )
	{
		auto i = handlers.find( { typeid(a.GetTypeTrait()),typeid(b.GetTypeTrait()) } );
		if ( i != handlers.end() )
		{
			i->second( a, b );
		}
		else
		{
			def( a, b );
		}
	}

private:
	std::unordered_map<TypePair, std::function<void( C&, C& )>> handlers;
	std::function<void( C&, C& )> def = []( C&, C& ) {};
};