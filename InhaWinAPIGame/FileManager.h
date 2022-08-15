#pragma once

#include <fstream>
#include <string>

class FileManager
{
public:
	template <class Data>
	Data Load( const std::wstring& filename)
	{
		Data data;

		std::wifstream input( filename, std::ios::binary );
		if ( input.is_open() )
		{

			input.read( reinterpret_cast<char*>(&data, sizeof( Data )) );
			input.close();
		}
		return data;
	}

	template <class Data>
	void Save( const std::wstring& filename, const Data& data )
	{
		std::wofstream output( filename, std::ios::binary );
		output.write( reinterpret_cast<const char*>(&data), sizeof( Data ) );
		output.close();
	}
};