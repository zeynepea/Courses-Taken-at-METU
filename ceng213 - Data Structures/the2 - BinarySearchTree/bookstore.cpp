#include "bookstore.hpp"

BookStore::BookStore( ) //implemented, do not change
{
}
void
BookStore::insert(const Book & book)
{
  primaryIndex.insert(book.getISBN(),book);
  secondaryIndex.insert(SKey(book),&(*(primaryIndex.find(book.getISBN()))));
  ternaryIndex.insert(SKey(book),&(*(primaryIndex.find(book.getISBN()))));

}

void
BookStore::remove(const std::string & isbn)
{
}

void
BookStore::remove(const std::string & title,
                  const std::string & author)
{
}

void
BookStore::removeAllBooksWithTitle(const std::string & title)
{
}

void
BookStore::makeAvailable(const std::string & isbn)
{
  (*primaryIndex.find(isbn)).setAvailable();
}

void
BookStore::makeUnavailable(const std::string & title,
                           const std::string & author){
  BSTT::Iterator it;
  for(it=ternaryIndex.begin();it!=ternaryIndex.end();++it){
    if((*(*it)).getTitle()==title && (*(*it)).getAuthor()==author) 
      (*(*it)).setUnavailable();
  }
}

void
BookStore::updatePublisher(const std::string & author, 
                           const std::string & publisher)
{
}

void
BookStore::printBooksWithISBN(const std::string & isbn1,
                              const std::string & isbn2,
                              unsigned short since) const
{
  std::list<typename BinarySearchTree<std::string, Book>::Iterator> lst=primaryIndex.find(isbn1,isbn2);
    typename 
  std::list<typename BinarySearchTree<std::string, Book>::Iterator>::const_iterator it;

  for (it = lst.begin(); it != lst.end(); ++it)
  {
    if((*(*it)).getYear()>since)
        std::cout << *(*it) << std::endl;
  }
}


void
BookStore::printBooksOfAuthor(const std::string & author,
                              const std::string & first,
                              const std::string & last) const
{
  BSTS::Iterator it;
  AuthorComparator isLessThan;
  for(it=secondaryIndex.begin();it!=secondaryIndex.end();++it){
    if(author==(*(*it)).getAuthor()&&isLessThan(SKey((*(*it)).getAuthor(),(*(*it)).getTitle()),SKey(author,last))&&isLessThan(SKey(author,first),SKey((*(*it)).getAuthor(),(*(*it)).getTitle())))
      std::cout<<*(*it)<<std::endl;
  }


}

void //implemented, do not change
BookStore::printPrimarySorted( ) const
{
  BSTP::Iterator it;

  for (it=primaryIndex.begin(); it != primaryIndex.end(); ++it)
  {
    std::cout << (*it) << std::endl;
  }
}

void //implemented, do not change
BookStore::printSecondarySorted( ) const
{
  BSTS::Iterator it;

  for (it = secondaryIndex.begin(); it != secondaryIndex.end(); ++it)
  {
    std::cout << *(*it) << std::endl;
  }
}

void //implemented, do not change
BookStore::printTernarySorted( ) const
{
  BSTT::Iterator it;

  for (it = ternaryIndex.begin(); it != ternaryIndex.end(); ++it)
  {
    std::cout << *(*it) << std::endl;
  }
}

