
/************************************************************************
 * Amy Project - part of vision based human motion capture project
 * 
 * Nurul Arif Setiawan
 * Intelligent Image Media Laboratory
 * Dept. of Computer and Engineering
 * Chonnam National University, Gwangju, South Korea
 *
 * aString.h
 * Some string utility
 * 
 * History : - May 19 2005 - add non-case sensitive comparation of string
 *						     add function to delete empty string
 *							 both found after searching high and low over google
 ************************************************************************/

#ifndef _AMY_STRING_H_
#define _AMY_STRING_H_


#include <string>

using namespace std;

/************************************************************************
 *  String utility
 ************************************************************************/

// perform case insensitive comparison
int nocase_cmp(const string & s1, const string& s2);

// delete empty string
void deleteEmpty(string &s);

#endif