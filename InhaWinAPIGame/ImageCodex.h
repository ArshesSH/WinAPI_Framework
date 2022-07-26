#pragma once

#include <vector>
#include <string>
#include <memory>
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
#include "GDIPlusManager.h"

class ImageCodex
{
private:
	class Entry
	{
	public:
		Entry( const std::wstring& key )
			:
			key( key ),
			pResource( Gdiplus::Image::FromFile( key.c_str() ) )
		{
			//pResource = std::make_unique<Gdiplus::Image>( Gdiplus::Image::FromFile( key.c_str() ) );
		}
		std::wstring key;
		std::shared_ptr<Gdiplus::Image> pResource;
	};

public:
	// Retrive a ptr to resource based on string
	//static Gdiplus::Image* Retrieve( const std::wstring& key )
	static std::shared_ptr<Gdiplus::Image> Retrieve( const std::wstring& key )
	{
		return Get()._Retrieve( key );
	}
	// Remove all entries from codex
	static void RemoveAll()
	{
		Get()._RemoveAll();
	}

private:
	ImageCodex() = default;
	~ImageCodex()
	{
		//for ( auto& e : entries )
		{
			//delete e.pResource;
		}
	}
	// find ptr to resoruce based on string
	//Gdiplus::Image* _Retrieve( const std::wstring& key )
	std::shared_ptr<Gdiplus::Image> _Retrieve( const std::wstring& key )
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
			i = entries.emplace( i, key );
		}

		// Return ptr to resource in codex
		return i->pResource;
	}

	void _RemoveAll()
	{
		//for ( auto& e : entries )
		{
			//delete e.pResource;
		}
		entries.clear();
	}

	// Get Singleton Instance
	static ImageCodex& Get()
	{
		static ImageCodex codex;
		return codex;
	}

private:
	std::vector<Entry> entries;
};