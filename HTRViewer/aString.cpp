
/************************************************************************
 * aString.cpp
 ************************************************************************/

#include "aString.h"

/**
 * non case sensitive comparation
 */
int nocase_cmp(const string & s1, const string& s2) 
{
  string::const_iterator it1=s1.begin();
  string::const_iterator it2=s2.begin();

  //stop when either string's end has been reached
  while ( (it1!=s1.end()) && (it2!=s2.end()) ) 
  { 
    if(::toupper(*it1) != ::toupper(*it2)) //letters differ?
     // return -1 to indicate smaller than, 1 otherwise
      return (::toupper(*it1)  < ::toupper(*it2)) ? -1 : 1; 
    //proceed to the next character in each string
    ++it1;
    ++it2;
  }
  size_t size1=s1.size(), size2=s2.size();// cache lengths
   //return -1,0 or 1 according to strings' lengths
    if (size1==size2) 
      return 0;
    return (size1<size2) ? -1 : 1;
}


/**
 * delete empty part of a string
 */
void deleteEmpty(string &s)
{
	char c;
	for (int i = 0; i < s.length(); i++)
	{
		c = s[i];

		if (c == ' ' || c == '\t' || c == '\0')
		{
			s.erase(i, s.length()-i);
			break;
		}
	}
}
