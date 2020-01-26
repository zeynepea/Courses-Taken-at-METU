#include "bookstore.hpp"

BookStore::BookStore( ) //implemented, do not change
{
}
/*private:
    typedef Book::SecondaryKey SKey;
    typedef BinarySearchTree<std::string, Book> BSTP;
    typedef BinarySearchTree<SKey, const Book *,AuthorComparator> BSTS;
    typedef BinarySearchTree<SKey, Book *, TitleComparator> BSTT;

  private:
    BSTP primaryIndex;
    BSTS secondaryIndex;
    BSTT ternaryIndex;

template <typename Key, typename Object, typename Comparator=std::less<Key> >
class BinarySearchTree 
{
  private: //do not change
    struct BinaryNode //node definition: a dependent type
    {
      Key key; //keys will be kept constant
      Object data; //objects that are referred to by keys may change
      BinaryNode * left;
      BinaryNode * right;
      size_t height; //height info should be updated per insert & delete

      BinaryNode(const Key &, const Object &,
                 BinaryNode *, BinaryNode *, size_t =0);
    };*/
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
