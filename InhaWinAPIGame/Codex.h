#pragma once

#include <vector>
#include <string>

template<class T>
class Codex
{
private:
	class Entry
	{
	public:
		Entry( const std::wstring& key, T* pResource)
			:
			key(key),
			pResource(pResource)
		{}
		std::wstring key;
		T* pResource;
	};

public:
	// Retrive a ptr to resource based on string
	static T* Retrieve( const std::wstring& key )
	{
		return Get()._Retrieve(key);
	}
	// Remove all entries from codex
	static void RemoveAll()
	{
		Get()._RemoveAll();
	}

private:
	Codex() = default;
	~Codex()
	{
		for ( auto& e : entries )
		{
			delete e.pResource;
		}
	}
	// find ptr to resoruce based on string
	T* _Retrieve( const std::wstring& key )
	{
		// find position of resource or when resource should be ( binary search )
		auto i = std::lower_bound( entries.begin(), entries.end(), key,
			[]( const Entry& e, const std::wstring& key )
			{
				return e.key < key;
			}
		);

		// if resource does not exist, load and store in sorted pos in coex, and set i to point to it
		if ( i == entries.end() || i->key != key )
		{
			// construct entry in i
			i = entries.emplace( i, key, new T(key.c_str()) );
		}

		// Return ptr to resource in codex
		return i->pResource;
	}

	void _RemoveAll()
	{
		for ( auto& e : entries )
		{
			delete e.pResource;
		}
		entries.clear();
	}

	// Get Singleton Instance
	static Codex& Get()
	{
		static Codex codex;
		return codex;
	}

private:
	std::vector<Entry> entries;
};