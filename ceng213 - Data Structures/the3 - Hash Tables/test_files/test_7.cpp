#include "Article.h"

void ppp( std::vector<int> & path )
{
	std::cout<<"Path : ";
	for( int i=0; i<path.size(); i++ )
		std::cout<< path[i] << "->";
	std::cout<< "End" << std::endl;
}

int main()
{
	std::cout << std::endl << "---------------- phase 0 -----------------" << std::endl << std::endl;
	Article article(7, 3112327, 3);
	std::cout << "Load Factor is " << article.getLoadFactor() << std::endl;
	article.printTable();
	
	std::cout << std::endl << "---------------- phase 1 -----------------" << std::endl << std::endl;
	std::vector<int> path;
	
	std::cout << std::endl << "---------------- phase 3 -----------------" << std::endl << std::endl;
	
	
	for( int i=0; i<26; i++ )
		std::cout << ( std::string(1,'a'+i)) << " is inserted with probe count " << article.insert(std::string(1,'a'+i),i+1) << std::endl;
		
	article.printTable();
	
	
	std::cout << std::endl << "---------------- phase 4 -----------------" << std::endl << std::endl;
	
	
	std::cout << "Trying to get \"asd\", 1th occurance " << article.get("asd",1,path) << std::endl;
	std::cout << "Length of path for previous query : " << path.size() << std::endl;
	ppp(path);
	path.clear();
	
	for( int i=0; i<26; i++ )
	{
		std::cout << "Trying to get \"" << ( std::string(1,'a'+i)) << "\", 1th occurance " << article.get(( std::string(1,'a'+i)),1,path) << std::endl;
		std::cout << "Length of path for previous query : " << path.size() << std::endl;
		ppp(path);
		path.clear();
	}
	
	std::cout << "Trying to get \"m\", 2th occurance " << article.get("m",2,path) << std::endl;
	std::cout << "Length of path for previous query : " << path.size() << std::endl;
	ppp(path);
	path.clear();
	
	
	std::cout << std::endl << "---------------- phase 5 -----------------" << std::endl << std::endl;
	
	for( int i=1; i<26; i+=3 )
	{
		std::cout << "Removing \"" << ( std::string(1,'a'+i)) << "\", 1st occurance " << article.remove(( std::string(1,'a'+i)),1) << std::endl;
	}
	
	for( int i=0; i<26; i++ )
	{
		std::cout << "Trying to get \"" << ( std::string(1,'a'+i)) << "\", 1th occurance " << article.get(( std::string(1,'a'+i)),1,path) << std::endl;
		std::cout << "Length of path for previous query : " << path.size() << std::endl;
		ppp(path);
		path.clear();
	}
	
	std::cout << std::endl << "---------------- phase 6 -----------------" << std::endl << std::endl;
	
	for( int i=0; i<26; i+=1 )
	{
		std::cout << "Removing \"" << ( std::string(1,'a'+i)) << "\", 1st occurance " << article.remove(( std::string(1,'a'+i)),1) << std::endl;
	}
	
	article.printTable();
	
	
	std::cout << std::endl << "---------------- phase 7 -----------------" << std::endl << std::endl;
	
	for( int i=0; i<26; i++ )
		std::cout << ( std::string(1,'a')) << " is inserted with probe count " << article.insert(std::string(1,'a'),i+1) << std::endl;
	
	article.printTable();
	
	std::cout << std::endl << "---------------- phase 8 -----------------" << std::endl << std::endl;
	
	for( int i=0; i<26; i++ )
	{
		std::cout << "Trying to get \"" << ( std::string(1,'a')) << "\""<< (i+1) << "th occurance" << article.get(( std::string(1,'a')),i+1,path) << std::endl;
		std::cout << "Length of path for previous query : " << path.size() << std::endl;
		ppp(path);
		path.clear();
	}
	
	std::cout << std::endl << "---------------- phase 9 -----------------" << std::endl << std::endl;
	
	for( int i=25; i>=1; i-- )
	{
		std::cout << "Removing \"" << ( std::string(1,'a')) << "\", 1st occurance " << article.remove(( std::string(1,'a')),1) << std::endl;
		
		std::cout << "Trying to get \"" << ( std::string(1,'a')) << "\" "<< i << "th occurance" << article.get(( std::string(1,'a')),i,path) << std::endl;
		std::cout << "Length of path for previous query : " << path.size() << std::endl;
		ppp(path);
		path.clear();
	}
	
	std::cout << std::endl << "---------------- phase 10 -----------------" << std::endl << std::endl;
	
	for( int i=0; i<26; i++ )
	{
		std::cout << "Trying to get \"" << ( std::string(1,'a')) << "\""<< (i+1) << "th occurance" << article.get(( std::string(1,'a')),i+1,path) << std::endl;
		std::cout << "Length of path for previous query : " << path.size() << std::endl;
		ppp(path);
		path.clear();
	}
	
	std::cout << std::endl << "---------------- phase 11 -----------------" << std::endl << std::endl;
	
	std::cout << ( std::string(1,'c')) << " is inserted with probe count " << article.insert(std::string(1,'c'),2) << std::endl;
	std::cout << ( std::string(1,'c')) << " is inserted with probe count " << article.insert(std::string(1,'c'),3) << std::endl;
	std::cout << ( std::string(1,'c')) << " is inserted with probe count " << article.insert(std::string(1,'c'),4) << std::endl;
	
	for( int i=0; i<26; i++ )
	{
		std::cout << "Trying to get \"" << ( std::string(1,'a')) << "\""<< (i+1) << "th occurance" << article.get(( std::string(1,'a')),i+1,path) << std::endl;
		std::cout << "Length of path for previous query : " << path.size() << std::endl;
		ppp(path);
		path.clear();
	}
	
	article.printTable();
	
	return 0;
}
