#ifndef _title_h__
#define _title_h__

#include "book.hpp"
#include <cstring>

class TitleComparator
{
    public:
    bool operator( ) (const Book::SecondaryKey & key1, 
                      const Book::SecondaryKey & key2) const{
        for(int i=0;i<key1.getTitle().size();i++){
            if(std::tolower( (key1.getTitle())[i])<std::tolower((key2.getTitle())[i])) return true;
            else if(std::tolower(key1.getTitle()[i])>std::tolower(key2.getTitle()[i])) return false;
        }
        if(key1.getTitle().size()==key2.getTitle().size()) {
        	for(int i=0;i<key1.getAuthor().size();i++){
                if(std::tolower( (key1.getAuthor())[i])<std::tolower((key2.getAuthor())[i])) return true;
                else if(std::tolower(key1.getAuthor()[i])>std::tolower(key2.getAuthor()[i])) return false;
    	    }
    	    if(key1.getAuthor().size()==key2.getAuthor().size()) return true;
    	    return false;
        }

        return false;
    }
};

#endif
