#include "Article.h"

void ppp( std::vector<int> & path )
{
	std::cout<<"Path : ";
	for( int i=0; i<path.size(); i++ )
		std::cout<< path[i] << "->";
	std::cout<<"End"<<std::endl;
}

int main()
{
	std::cout << std::endl << "---------------- phase 0 -----------------" << std::endl << std::endl;
	Article article(7, 3112327, 3);
	std::vector<int> path;
    article.printTable();
	std::cout << std::endl << "---------------- phase 1 -----------------" << std::endl << std::endl;
	
	
	for( int i=0; i<5; i++ )
	{
		std::cout << "mert" << " is inserted with probe count " << article.insert("mert",i+1) << std::endl;
	}
	
	std::cout << std::endl << "---------------- phase 2 -----------------" << std::endl << std::endl;
	
	std::cout << "1 th mert" << " is removed " << article.remove(std::string("mert"),1) << std::endl;
	article.printTable();
	
	std::cout << "4 th mert" << " is removed " << article.remove(std::string("mert"),4) << std::endl;
	article.printTable();
	
	std::cout << "2 th mert" << " is removed " << article.remove(std::string("mert"),2) << std::endl;
	article.printTable();
	
	std::cout << "1 th mert" << " is removed " << article.remove(std::string("mert"),1) << std::endl;
	article.printTable();
	
	std::cout << "1 th mert" << " is removed " << article.remove(std::string("mert"),1) << std::endl;
	article.printTable();
	
	
	return 0;
}
