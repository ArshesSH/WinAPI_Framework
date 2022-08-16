#pragma once

#include <fstream>
#include <string>
#include <vector>

class FileManager
{
public:
	enum class Mode
	{
		Read,
		Write
	};

public:
	FileManager(const std::wstring& filePath, Mode mode)
		:
		filePath(filePath),
		mode(mode)
	{
		switch ( mode )
		{
		case FileManager::Mode::Read:
			in.open( filePath, std::ios::binary );
			break;
		case FileManager::Mode::Write:
			out.open( filePath, std::ios::binary );
			break;
		}
	}
	~FileManager()
	{
		FinishRead();
		FinishWrite();
	}
	void FinishRead()
	{
		if ( in.is_open() )
		{
			in.close();
		}
	}
	void FinishWrite()
	{
		if ( out.is_open() )
		{
			out.close();
		}
	}

	bool CanLoad() const
	{
		return in.is_open();
	}

	template <class Data>
	bool Load(Data& data)
	{
		if ( in.read( reinterpret_cast<char*>(&data), sizeof( Data ) ) )
		{
			return true;
		}
		return false;
	}

	template <class Data>
	void Save(const Data& data)
	{
		out.write( reinterpret_cast<const char*>(&data), sizeof( Data ) );
	}

	template <class Data>
	void LoadVector( std::vector<Data>& datas )
	{
		for ( Data data; Load( data ); )
		{
			datas.push_back( data );
		}
	}

	template <class Data>
	void SaveVector( const std::vector<Data>& datas )
	{
		for ( const auto& data : datas )
		{
			Save( data );
		}
	}

private:
	std::wstring filePath;
	Mode mode;
	std::ifstream in;
	std::ofstream out;
};