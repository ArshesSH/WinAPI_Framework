#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <cassert>

class FileManager
{
public:
	FileManager( const std::wstring& filename )
		:
		filename( filename )
	{}

	std::vector<std::wstring> GetLineVector() const
	{
		std::vector<std::wstring> list;
		std::wifstream input( filename );

		assert( !input.fail() );

		for ( std::wstring str; std::getline( input, str ); )
		{
			if ( str.empty() )
			{
				continue;
			}
			list.push_back( str );
		}
		input.close();

		return list;
	}

	std::wstring GetSingleLine() const
	{
		std::wifstream input( filename );
		assert( !input.fail() );

		std::wstring str;
		std::getline( input, str );
		return std::move( str );
	}

	void SaveToFile(const std::vector<std::wstring>& data)
	{
		std::wofstream output( filename );

		for ( auto str : data )
		{
			output << str;
		}
	}

private:
	std::wstring filename;
};

