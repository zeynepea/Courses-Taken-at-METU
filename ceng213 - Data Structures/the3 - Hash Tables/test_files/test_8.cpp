#include "Article.h"
void ppp( std::vector<int> & path )
{
        std::cout<<"Path : ";
        for( int i=0; i<path.size(); i++ )
                std::cout<< path[i] << "->";
        std::cout<<std::endl;
}

int main()
{
	Article article(13, 101, 11);
	
	std::cout << "----------------" << std::endl;
	article.getAllWordsFromFile("lorem.txt");
	std::cout << "----------------" << std::endl;
	article.printTable();
	std::cout << "----------------" << std::endl;
	std::cout << "----------------" << std::endl;
        std::vector<int> path;
        for (int i = 66; i > 0; i--)
        {
                std::cout << article.get("lorem", i, path) << std::endl;
                ppp(path);
                path.clear();
        }
	std::cout << "----------------" << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << article.getLoadFactor() << std::endl;
	return 0;
}
