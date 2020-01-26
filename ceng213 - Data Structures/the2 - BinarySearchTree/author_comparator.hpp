#ifndef _author_h__
#define _author_h__

#include "book.hpp"
#include <cstring>

class AuthorComparator
{
  public:
    bool operator( ) (const Book::SecondaryKey & key1, 
                      const Book::SecondaryKey & key2) const
    {
      //if key1 is less than key2 wrt specifications
      //return true
      //otherwise
      //return false
    	for(int i=0;i<key1.getAuthor().size();i++){
            if(std::tolower( (key1.getAuthor())[i])<std::tolower((key2.getAuthor())[i])) return true;
            else if(std::tolower(key1.getAuthor()[i])>std::tolower(key2.getAuthor()[i])) return false;
    	}
    	if(key1.getAuthor().size()==key2.getAuthor().size()) {
    		for(int i=0;i<key1.getTitle().size();i++){
                if(std::tolower( (key1.getTitle())[i])<std::tolower((key2.getTitle())[i])) return true;
                else if(std::tolower(key1.getTitle()[i])>std::tolower(key2.getTitle()[i])) return false;
            }
            return false;
    	}
    	return false;

    }

};

#endif
