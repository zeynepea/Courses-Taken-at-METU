#include "Article.h"

/*#############################
#               NOTE!         #
#    Please don't forget to   #
#     check PDF. Fuctions     #
#    may have more detailed   #
#     tasks which are not     #
#       mentioned here.       #
#############################*/
void Article::SetArticle(int t, int h1,int h2){
	table_size=t;
	h1_param=h1;
	h2_param=h2;
}
Article::Article(  int table_size,int h1_param, int h2_param )
{    
    /*#############################
    #            TO-DO            #
    # Write a default constructor #
    #   for the Article Class     #
    #############################*/
    n=0;
    SetArticle(table_size,h1_param,h2_param);
    table=new std::pair<std::string, int>[table_size];
    for(int i=0;i<table_size;i++){
    	table[i].first="";
    	table[i].second=-1;
    }
}

Article::~Article()
{
    /*#############################
    #             TO-DO           #
    #  Write a deconstructor for  #
    #     the Article Class       #
    #############################*/
    delete [] table;
    table=NULL;
}

int Article::get( std::string key, int nth, std::vector<int> &path ) const
{
    /*#############################################
    #                    TO-DO                    #
    #      Your get function should return        #
    # the original index (index in the text file) #
    #    of the nth 'key' in the hash table.      #
    #    If there is no such a key, return -1     #
    #    If there is, return the original index   #
    #     In both cases, you need to push each    #
    #          visited index while probing        #
    #           that you calculated until         #
    #      finding the key, to the path vector.   #
    #############################################*/
    int y=0,a=0;
    for(int i=0;i<table_size;i++){
    	if(y==nth)
    		return table[a].second; 
    	a=hash_function(key,i);
    	if(table[a].first==key ){
    		if(i!=0) path.push_back(a);
    		y++;
    	}
    	else if(table[a].second==MARKED_INDEX){
    		for(int j=i;j<table_size;j++){
    			a=hash_function(key,j);
    			if(j!=0) path.push_back(a);
    			if(table[a].second==EMPTY_INDEX) return -1;
    			if(table[a].first==key){
    				y++;
    				if(y==nth)
    				   return table[a].second;
    			} 
    		}
    		return -1;    		
    	}
    	else if(i!=0) path.push_back(a);
    }
    return -1;
}

int Article::insert( std::string key, int original_index )
{
    /*#########################################
    #                 TO-DO                   #
    #      Insert the given key, with the     #
    # original index value (at the text file) #
    #           to the hash table.            #
    #  Return the total number of probes you  #
    #      encountered while inserting.       #
    #########################################*/
    if(getLoadFactor()>MAX_LOAD_FACTOR){
    	expand_table();}
    int probe=0;
    for(int i=0;i<table_size;i++){
    	int a=hash_function(key,i);
    	if(table[a].first==EMPTY_KEY){
    		table[a].first=key;
    		table[a].second=original_index;
    		++n;
    		break;
    	}
    	else if(table[a].first==key){
    		if(table[a].second> original_index){
    			int b=table[a].second;
    			table[a].second=original_index;
    			original_index=b;
    			i=0;                
    		}
    		probe++;
    	}
    	else probe++;
    }
    return probe;
}


int Article::remove( std::string key, int nth )
{
    /*#########################################
    #                  TO-DO                  #
    #      Remove the nth key at the hash     #
    #                  table.                 #
    #  Return the total number of probes you  #
    #      encountered while inserting.       #
    #   If there is no such a key, return -1  #
    #     If there, put a mark to the table   #
    #########################################*/
    int y=0,x=0,a=0;
    for(int i=0;i<table_size;i++){
        if(x==nth&&table[a].first==key){
        	table[a].first=EMPTY_KEY;
        	table[a].second=MARKED_INDEX;
        	--n;
        	return y;
        } 
        a=hash_function(key,i);
    	if(table[a].first==key ){
           if(i!=0) y++;
           x++;   
       	}
    	else if(i!=0) y++;
    }
    return -1;
}

double Article::getLoadFactor() const
{
    /*#########################################
    #                TO-DO                    #
    #      Return the load factor of the      #
    #                table                    #
    #########################################*/

    return (double )n/ (double )table_size;
}

void Article::getAllWordsFromFile( std::string filepath )
{
    /*#########################################
    #                  TO-DO                  #
    #       Parse the words from the file     #
    #      word by word and insert them to    #
    #                hash table.              #
    #   For your own inputs, you can use the  #
    #  'inputify.py' script to fix them to    #
    #            the correct format.          #
    #########################################*/
    std::ifstream file;
    file.open(filepath.c_str());
    if (!file.is_open()) return;
    std::string word;
    int a=1;
    while (file >> word)
    {
        insert(word,a);
        a++;
    }
}

void Article::expand_table()
{
    /*#########################################
    #                  TO-DO                  #
    #   Implement the expand table function   #
    #   in order to increase the table size   #
    #   to the very first prime number after  #
    #      the value of (2*table size).       #
    #         Re-hash all the data.           #
    #       Update h2_param accordingly.      #
    #########################################*/
    std::pair<std::string, int>* oldtable=table;
    int a=table_size;
    table_size=nextPrimeAfter(2*table_size);
    h2_param=firstPrimeBefore(table_size);
    table=new std::pair<std::string, int>[table_size];
    for(int i=0;i<table_size;i++){
    	table[i].first=EMPTY_KEY;
    	table[i].second=EMPTY_INDEX;
    }
    for(int i=0;i<a;i++)
    	if(oldtable[i].first!=EMPTY_KEY){
    		--n;
    		insert(oldtable[i].first,oldtable[i].second);
    	}
    delete [] oldtable;
    oldtable=NULL;
}

int Article::hash_function( std::string& key, int i ) const
{
    /*#########################################
    #                TO-DO                    #
    #       Implement the main hashing        #
    #    function. Firstly, convert key to    #
    #    integer as stated in the homework    #
    #      text. Then implement the double    #
    #            hashing function.            #
    #      use convertStrToInt function to    #
    #      convert key to a integer for       #
    #         using it on h1 and h2           #
    #               reminder:                 #
    #            you should return            #
    #    ( h1(keyToInt) + i*h2(keyToınt) )    #
    #            modulo table_size            #
    #########################################*/
    int a=convertStrToInt(key);
    return ( h1(a) + i*h2(a) ) % table_size ;
}

int Article::h1( int key ) const
{
    /*###############################
    #              TO-DO            #
    #      First Hash function      #
    # Don't forget to use h1_param. #
    ###############################*/
    int i=0;
    while(key>0){
        if(key%2==1)
        	++i;
        key/=2;
    }
    return i*h1_param;
}

int Article::h2( int key ) const
{
    /*###############################
    #              TO-DO            #
    #     Second Hash function      #
    # Don't forget to use h2_param. #
    ###############################*/
    return h2_param-(key%h2_param);
}
