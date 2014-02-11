
/************************************************************************
 * Amy Project - part of vision based human motion capture project
 * 
 * Nurul Arif Setiawan
 * Intelligent Image Media Laboratory
 * Dept. of Computer and Engineering
 * Chonnam National University, Gwangju, South Korea
 *
 * aDelim.h
 * Read delimiter type ascii text file. 
 * 
 * History : - May 19 2005 - taken from C++ Timesaving Techniques for Dummies by Matthew Telles.
 *			 - May 2 2005 - Add function to delete empty column
 ************************************************************************/

#ifndef _AMY_DELIM_H_
#define _AMY_DELIM_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

// Avoid having to type out std:: for all
// STL classes
using namespace std;

// This class manages a list of delimiters
class Delimiters
{
private:
	// Array of possible delimiters. Use a vector,
	// since the characters could include NULL byte
	// or other characters that string can't handle
	vector< char > _delimiters; 

protected:
	virtual void Copy ( const Delimiters& aCopy )
	{
		vector<char>::const_iterator iter;
		for ( iter = aCopy._delimiters.begin(); iter != aCopy._delimiters.end(); ++iter )
			_delimiters.insert( _delimiters.end(), (*iter) );
	}
public:
	Delimiters(void)
	{
	}
	Delimiters( const char *delimiterList )
	{
		for ( int i=0; i<strlen(delimiterList); ++i )
		{
			_delimiters.insert( _delimiters.end(), delimiterList[i] );
		}
	}
	Delimiters( const Delimiters& aCopy )
	{
		Copy ( aCopy );
	}
	Delimiters operator=( const Delimiters& aCopy )
	{
		Copy( aCopy );
		return *this;
	}
	virtual ~Delimiters(void)
	{
	}

	// Clear out the entire list
	virtual void Clear()
	{
		_delimiters.erase( _delimiters.begin(), _delimiters.end() );
	}
	// Add a delimiter to the list
	virtual void Add( char c )
	{
		_delimiters.insert( _delimiters.end(), c );
	}
	// See if a given character is in the list
	virtual bool Contains( char c )
	{
		vector<char>::const_iterator iter;
		for ( iter = _delimiters.begin(); iter != _delimiters.end(); ++iter )
			if ( c == (*iter) )
				return true;
		return false;
	}
	// Remove a given delimiter
	virtual bool Remove( char c )
	{
		vector<char>::iterator iter;
		for ( iter = _delimiters.begin(); iter != _delimiters.end(); ++iter )
			if ( c == (*iter) )
			{
				_delimiters.erase( iter );
				return true;
			}
		return false;
	}

};

// This class manages the data for a given row of a 
// delimited file
class DelimitedRow
{
private:
	vector< string > _columns;
protected:
	virtual void Copy( const DelimitedRow& aCopy )
	{
		vector<string>::const_iterator iter;
		for ( iter = aCopy._columns.begin(); iter != aCopy._columns.end(); ++iter )
			_columns.insert( _columns.end(), (*iter) );
	}
public:
	DelimitedRow(void)
	{
	}
	DelimitedRow( const char *col )
	{
		Add( col );
	}
	virtual ~DelimitedRow()
	{
	}
	virtual void Add( const char *col )
	{
		_columns.insert( _columns.end(), col );
	}
	int NumColumns( void )
	{
		return _columns.size();
	}

	string getColumn( int index )
	{
		if ( index < 0 || index > NumColumns()-1 )
			return string("");
		return _columns[index];
	}

	// added, delete empty colums
	// Nurul Arif Setiawan, May 2nd, 2005
	void DelEmpty()
	{
		vector<string>::iterator end = remove (_columns.begin(), _columns.end(), string("")); 
		_columns.erase(end,_columns.end());
	}

};


// This class will handle a single delimited file
class DelimitedFileParser
{
private:
	string					_fileName;
	Delimiters				_delim;
	ifstream				_in;
	vector<DelimitedRow>	_rows;
protected:
	virtual void Copy( const DelimitedFileParser& aCopy )
	{
		_fileName = aCopy._fileName;
		_delim    = aCopy._delim;

		vector<DelimitedRow>::const_iterator iter;
		for ( iter = aCopy._rows.begin(); iter != aCopy._rows.end(); ++iter )
			_rows.insert( _rows.end(), (*iter) );
	}
	virtual void _ParseLine( string sIn )
	{
		// Given a delimiter list, and an input string, parse through
		// the string.
		DelimitedRow row;
		string sCol = "";
		for ( int i=0; i<sIn.length(); ++i )
		{
			if ( _delim.Contains( sIn[i] ) )
			{
				row.Add( sCol.c_str() );
				sCol = "";
			}
			else
				sCol += sIn[i];
		}
		row.Add( sCol.c_str() );
		_rows.insert( _rows.end(), row );

	}

public:
	DelimitedFileParser(void)
	{
	}
	DelimitedFileParser( const char *fileName, const char *delimiters )
		: _delim( delimiters )
	{
		Open( fileName );
	}
	DelimitedFileParser( const DelimitedFileParser& aCopy )
	{
		Copy ( aCopy );
	}
	virtual ~DelimitedFileParser()
	{
		_in.close();
	}
	virtual bool Open( const char *fileName )
	{
		_fileName = fileName;
		_in.open( _fileName.c_str() );
		return !_in.fail();
	}
	virtual bool Parse()
	{
		// Make sure the file is open
		if ( _in.fail() )
		{
			return false;
		}

		while ( !_in.eof() )
		{
			string sIn = "";

			while ( !_in.eof() )
			{
				// Get an input line.
				char c;
				_in.get(c);
				if ( _in.fail() )
					break;
				if ( c != '\r' && c != '\n' )
					sIn += c;
				if ( c == '\n' )
					break;
			}
			// Parse it.
			if ( sIn.length() )
				_ParseLine( sIn );

		}


		return true;
	}
	int NumRows()
	{
		return _rows.size();
	}
	DelimitedRow getRow( int index )
	{
		if ( index < 0 || index >= NumRows() )
			throw "getRow: index out of range";
		return _rows[index];
	}
};



#endif
