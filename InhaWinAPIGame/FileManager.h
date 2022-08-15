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

	std::vector<std::string> GetLineVector() const
	{
		std::vector<std::string> list;
		std::ifstream input( filename, std::ios::binary );

		assert( !input.fail() );

		for ( std::string str; std::getline( input, str ); )
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

	std::string GetSingleLine() const
	{
		std::ifstream input( filename, std::ios::binary );
		assert( !input.fail() );

		std::string str;
		std::getline( input, str );
		return std::move( str );
	}

	void SaveToFile( const std::vector<std::string>& data )
	{
		std::ofstream output( filename, std::ios::binary );

		for ( auto str : data )
		{
			output << str;
		}
	}

private:
	std::wstring filename;
};

