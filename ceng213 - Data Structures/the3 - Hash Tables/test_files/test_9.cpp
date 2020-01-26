#include "Article.h"

void delFromFile( std::string filepath, Article* article ) // it returns probe count
{
	std::ifstream file;
    file.open (filepath.c_str());
    if (!file.is_open()) return;

    std::string word;
    int i = 0;
    while (file >> word)
    {
        std::cout <<word << " is being deleted : " << article->remove( word, 1) << std::endl;
    }
}

int main()
{
	Article article(7, 3112327, 3);
	
	std::cout << "----------------" << std::endl;
	article.getAllWordsFromFile("magic.txt");
	std::cout << "----------------" << std::endl;
	article.printTable();
	std::cout << "----------------" << std::endl;
	std::cout << article.getLoadFactor() << std::endl;
	
	
	std::cout << "----------------" << std::endl;
	delFromFile("magic.txt", &article);
	std::cout << "----------------" << std::endl;
	article.printTable();
	std::cout << "----------------" << std::endl;
	std::cout << article.getLoadFactor() << std::endl;
	return 0;
}
