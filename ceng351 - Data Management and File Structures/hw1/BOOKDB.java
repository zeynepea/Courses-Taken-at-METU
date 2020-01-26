package ceng.ceng351.bookdb;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Vector;
import java.util.*;

public class BOOKDB implements IBOOKDB {

    private static String user = "2171577"; // TODO: Your userName
    private static String password = "7a28805d"; //  TODO: Your password
    private static String host = "144.122.71.65"; // host name
    private static String database = "db2171577"; // TODO: Your database name
    private static int port = 8084; // port

    private Connection con;

    public BOOKDB() {

    }

    @Override
    public void initialize() {
        String url = "jdbc:mysql://" + this.host + ":" + this.port + "/" + this.database+"?useUnicode=true&useLegacyDatetimeCode=false&serverTimezone=Turkey&useSSL=false&";
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            this.con =  DriverManager.getConnection(url, this.user, this.password);
        }
        catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        }

    }

    @Override
    public int createTables() {

        String createAuthorTable ="CREATE TABLE author(authorID int NOT NULL," +
                                                    "authorName VARCHAR(60), " +
                                                    "PRIMARY KEY(authorID))";
        String createPublisherTable ="CREATE TABLE publisher(publisherID int NOT NULL," +
                                                            "publisherName VARCHAR(50)," +
                                                            "PRIMARY KEY(publisherID))";
        String createBookTable="CREATE TABLE book(isbn CHAR(13) NOT NULL," +
                                                "bookName VARCHAR(120), " +
                                                "publisherID int NOT NULL, " +
                                                "firstPublishYear CHAR(4), " +
                                                "pageCount int, " +
                                                "category VARCHAR(25), " +
                                                "rating float, " +
                                                "PRIMARY KEY(isbn), " +
                                                "FOREIGN KEY(publisherID) REFERENCES publisher(publisherID) on delete cascade on update cascade)";
        String createAuthorOfTable="CREATE TABLE authorOf(isbn CHAR(13) NOT NULL, " +
                                                        "authorID int NOT NULL, " +
                                                        "PRIMARY KEY(isbn, authorID), " +
                                                        "FOREIGN KEY(isbn) REFERENCES book(isbn) on delete cascade on update cascade, " +
                                                        "FOREIGN KEY(authorID) REFERENCES author(authorID) on delete cascade on update cascade)";
        String createPhw1Table="CREATE TABLE phw1(isbn CHAR(13) NOT NULL, " +
                                                "bookName VARCHAR(120), " +
                                                "rating float, " +
                                                "PRIMARY KEY(isbn))";
        String[] createQueries= new String[5];
        createQueries[0]=createAuthorTable;
        createQueries[1]=createPublisherTable;
        createQueries[2]=createBookTable;
        createQueries[3]=createAuthorOfTable;
        createQueries[4]=createPhw1Table;
        int numberofTablesInserted=0;
        for(String query: createQueries){
            try {
                Statement statement = this.con.createStatement();
                statement.executeUpdate(query);
                numberofTablesInserted++;
                //close
                statement.close();
            }
            catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return numberofTablesInserted;
    }

    @Override
    public int dropTables() {

        String dropAuthorTable="drop table if exists author";
        String dropPublisherTable="drop table if exists publisher";
        String dropBookTable="drop table if exists book";
        String dropAuthorOfTable="drop table if exists authorOf";
        String dropPhw1Table="drop table if exists phw1";
        String[] createQueries= new String[5];
        createQueries[4]=dropAuthorTable;
        createQueries[3]=dropPublisherTable;
        createQueries[2]=dropBookTable;
        createQueries[1]=dropAuthorOfTable;
        createQueries[0]=dropPhw1Table;
        int numberofTablesDropped=0;
        for(String query: createQueries){
            try {
                Statement statement = this.con.createStatement();
                statement.executeUpdate(query);
                numberofTablesDropped++;
                //close
                statement.close();
            }
            catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return numberofTablesDropped;
    }

    @Override
    public int insertAuthor(Author[] authors) {

        int i=0;
        for(Author author:authors){
            String query="insert into author values (" + "\"" + author.getAuthor_id() +
                                            "\"" + "," + "\"" + author.getAuthor_name() +
                                            "\"" + ")";
            try {
                Statement st = this.con.createStatement();
                st.executeUpdate(query);
                i++;
                //Close
                st.close();
            }
            catch (SQLException e) {
                //e.printStackTrace();
                e.printStackTrace();
            }
        }
        return i;
    }

    @Override
    public int insertBook(Book[] books) {

        int i=0;
        for(Book book:books){
            String query="insert into book values (" +"\"" + book.getIsbn() +
                                         "\"" + "," + "\"" + book.getBook_name() +
                                         "\"" + "," + "\"" + book.getPublisher_id() +
                                         "\"" + "," + "\"" + book.getFirst_publish_year() +
                                         "\"" + "," + "\"" + book.getPage_count() +
                                         "\"" + "," + "\"" + book.getCategory() +
                                         "\"" + "," + "\"" + book.getRating() +
                                         "\"" + ")";
            try {
                Statement st = this.con.createStatement();
                st.executeUpdate(query);
                i++;
                st.close();
            }
            catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return i;
    }

    @Override
    public int insertPublisher(Publisher[] publishers) {

        int i=0;
        for(Publisher publisher:publishers){
            String query="insert into publisher values (" +"\"" + publisher.getPublisher_id() +
                                              "\"" + "," + "\"" + publisher.getPublisher_name() +
                                              "\"" + ")";
            try {
                Statement st = this.con.createStatement();
                st.executeUpdate(query);
                i++;
                st.close();
            }
            catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return i;
    }

    @Override
    public int insertAuthor_of(Author_of[] author_ofs) {

        int i=0;
        for(Author_of author_of:author_ofs){
            String query="insert into authorOf values (" +"\"" + author_of.getIsbn() +
                                             "\"" + "," + "\"" + author_of.getAuthor_id() +
                                             "\"" + ")";
            try {
                Statement st = this.con.createStatement();
                st.executeUpdate(query);
                i++;
                st.close();
            }
            catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return i;
    }

    /**
     * Should return isbn, first_publish_year, page_count and publisher_name of
     * the books which have the maximum number of pages.
     * @return QueryResult.ResultQ1[]
     */

    @Override
    public QueryResult.ResultQ1[] functionQ1() {
        String query ="select bk.isbn, bk.firstPublishYear, bk.pageCount, p.publisherName\n" +
                        "from book bk, publisher p\n" +
                        "where bk.pageCount = (Select MAX(b.pageCount) from book b) and bk.publisherID=p.publisherID \n" +
                        "order by bk.isbn";
        ResultSet rs;
        List<QueryResult.ResultQ1> list = new ArrayList<>();
        try{
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            while(rs.next()){
                QueryResult.ResultQ1 put=new QueryResult.ResultQ1(rs.getString("isbn"),
                                                                  rs.getString("firstPublishYear"),
                                                                  rs.getInt("pageCount"),
                                                                  rs.getString("publisherName"));
                list.add(put);
            }
            st.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }

        QueryResult.ResultQ1[] array = (new QueryResult.ResultQ1[list.size()]);
        return list.toArray(array);

    }

    /**
     * author(author id:int, author name:varchar(60))
     *
     * publisher(publisher id:int, publisher name:varchar(50))
     *
     * book(isbn:char(13), book name:varchar(120), publisher id:int, first publish year:char(4), page count:int, category:varchar(25),
     * rating:float) REFERENCES publisher(publisher id)
     *
     * author of (isbn:char(13),author id:int) REFERENCES book(isbn) author(author id)
     *
     * phw1(isbn:char(13), book name:varchar(120), rating:float)
     *
     * For the publishers who have published books that were co-authored by both of the given authors(author1 and author2); list
     * publisher id(s) and average page count(s) of all the books these publishers have published.
     * Order the results by publisher id in ascending order.
     *
     * For the publishers who have published books that were co-authored by both
     * of the given authors(author1 and author2); list publisher_id(s) and average
     * page_count(s)  of all the books these publishers have published.
     * @param author_id1
     * @param author_id2
     * @return QueryResult.ResultQ2[]
     */

    @Override
    public QueryResult.ResultQ2[] functionQ2(int author_id1, int author_id2) {
        String query="select b.publisherID, AVG(b.pageCount) as avgPageCount \n" +
                     "from authorOf ao1, authorOf ao2, book b \n" +
                     "where ao1.authorID!=ao2.authorID " +
                        "and ao1.isbn=ao2.isbn " +
                        "and ao1.isbn=b.isbn " +
                        "and ao1.authorID=" +"\""+author_id1+"\""+
                        "and ao2.authorID=" +"\""+author_id2+"\""+
                     "GROUP BY b.publisherID " +
                     "order by 1";
        ResultSet rs;
        List<QueryResult.ResultQ2> list = new ArrayList<>();
        try{
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            while(rs.next()){
                QueryResult.ResultQ2 put=new QueryResult.ResultQ2(rs.getInt("publisherID"),
                                                                  rs.getInt("avgPageCount"));
                list.add(put);
            }
            st.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        QueryResult.ResultQ2[] array = (new QueryResult.ResultQ2[list.size()]);
        return list.toArray(array);
    }

    /**
     * author(author id:int, author name:varchar(60))
     *
     * publisher(publisher id:int, publisher name:varchar(50))
     *
     * book(isbn:char(13), book name:varchar(120), publisher id:int, first publish year:char(4), page count:int, category:varchar(25),
     * rating:float) REFERENCES publisher(publisher id)
     *
     * author of (isbn:char(13),author id:int) REFERENCES book(isbn) author(author id)
     *
     * phw1(isbn:char(13), book name:varchar(120), rating:float)
     *
     * List book_name, category and first_publish_year of the earliest
     * published book(s) of the author(s) whose author_name is given.
     * @param author_name
     * @return QueryResult.ResultQ3[]
     */

    @Override
    public QueryResult.ResultQ3[] functionQ3(String author_name) {

        String query ="select bk.bookName, bk.category, bk.firstPublishYear " +
                      "from book bk, author a, authorOf aof " +
                      "where a.authorName =" +"\""+author_name+"\""+ " " +
                            "and a.authorID = aof.authorID " +
                            "and aof.isbn = bk.isbn " +
                            "and bk.firstPublishYear <= ALL (" +
                                "select bk.firstPublishYear " +
                                "from book bk, author a, authorOf aof " +
                                "where a.authorName ="  +"\""+author_name+"\""+ " " +
                                    "and a.authorID = aof.authorID " +
                                    "and aof.isbn = bk.isbn) " +
                      "order by 1,2,3";
        ResultSet rs;
        List<QueryResult.ResultQ3> list = new ArrayList<>();
        try{
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            while(rs.next()){
                QueryResult.ResultQ3 put=new QueryResult.ResultQ3(rs.getString("bookName"),
                        rs.getString("category"),
                        rs.getString("firstPublishYear"));
                list.add(put);
            }
            st.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }

        QueryResult.ResultQ3[] array = (new QueryResult.ResultQ3[list.size()]);
        return list.toArray(array);
    }

    @Override
    public QueryResult.ResultQ4[] functionQ4() {
        String query ="select distinct bk.publisherID, bk.category " +
                        "from book bk, publisher p " +
                        "where bk.publisherID = p.publisherID " +
                            "and p.publisherName like '% % %' "+
                            "and bk.publisherID in (select bk.publisherID from book bk group by bk.publisherID having AVG(bk.rating)>3 and count(*)>3) "+
                        "order by 1,2";
        ResultSet rs;
        List<QueryResult.ResultQ4> list = new ArrayList<>();
        try{
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            while(rs.next()){
                QueryResult.ResultQ4 put=new QueryResult.ResultQ4(rs.getInt("publisherID"),
                                                                  rs.getString("category"));
                list.add(put);
            }
            st.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }

        QueryResult.ResultQ4[] array = (new QueryResult.ResultQ4[list.size()]);
        return list.toArray(array);
    }

    @Override
    public QueryResult.ResultQ5[] functionQ5(int author_id) {

        String query ="select distinct a.authorID, a.authorName " +
                        "from authorOf aof, author a " +
                        "where aof.authorID = a.authorID " +
                            "and not exists(" +
                                "select bk2.publisherID " +
                                "from book bk2, authorOf aof2 " +
                                "where bk2.isbn = aof2.isbn " +
                                    "and aof2.authorID ="  +"\""+author_id+"\""+ " "+
                                    "and not exists( " +
                                        "select bk3.publisherID " +
                                        "from book bk3 " +
                                        "where bk2.publisherID = bk3.publisherID " +
                                            "and bk3.isbn = aof.isbn)) " +
                        "order by 1";
        ResultSet rs;
        List<QueryResult.ResultQ5> list = new ArrayList<>();
        try{
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            while(rs.next()){
                QueryResult.ResultQ5 put=new QueryResult.ResultQ5(rs.getInt("authorID"),
                                                                  rs.getString("authorName"));
                list.add(put);
            }
            st.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }

        QueryResult.ResultQ5[] array = (new QueryResult.ResultQ5[list.size()]);
        return list.toArray(array);
    }

    @Override
    public QueryResult.ResultQ6[] functionQ6() {
        String query ="select aof.authorID, aof.isbn " +
                        "from authorOf aof, book bk " +
                        "where aof.isbn = bk.isbn " +
                            "and bk.publisherID not in (select bk2.publisherID " +
                                                        "from book bk2, book bk3, authorOf aof2, authorOf aof3 " +
                                                        "where bk2.isbn = aof2.isbn " +
                                                            "and bk3.isbn = aof3.isbn " +
                                                            "and aof2.authorID = aof3.authorID " +
                                                            "and bk2.publisherID != bk3.publisherID) " +
                        "order by 1,2";
        ResultSet rs;
        List<QueryResult.ResultQ6> list = new ArrayList<>();
        try{
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            while(rs.next()){
                QueryResult.ResultQ6 put=new QueryResult.ResultQ6(rs.getInt("authorID"),
                                                                  rs.getString("isbn"));
                list.add(put);
            }
            st.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }

        QueryResult.ResultQ6[] array = (new QueryResult.ResultQ6[list.size()]);
        return list.toArray(array);
    }

    @Override
    public QueryResult.ResultQ7[] functionQ7(double rating) {
        String query ="select p.publisherID, p.publisherName " +
                      "from publisher p " +
                      "where p.publisherID in (select bk.publisherID from book bk group by bk.publisherID having AVG(bk.rating)>"+"\""+rating+"\""+") " +
                          "and p.publisherID in (select bk.publisherID from book bk where bk.category like '%Roman%' group by bk.publisherID having count(*)>1) "+
                      "order by 1";
        ResultSet rs;
        List<QueryResult.ResultQ7> list = new ArrayList<>();
        try{
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            while(rs.next()){
                QueryResult.ResultQ7 put=new QueryResult.ResultQ7(rs.getInt("publisherID"),
                                                                  rs.getString("publisherName"));
                list.add(put);
            }
            st.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }

        QueryResult.ResultQ7[] array = (new QueryResult.ResultQ7[list.size()]);
        return list.toArray(array);
    }


    @Override
    public QueryResult.ResultQ8[] functionQ8(){
        return new QueryResult.ResultQ8[1];
    }

    @Override
    public double functionQ9(String keyword) {
        String query="update book set rating = rating + 1.0" +
                        "where bookName like '%"+"\""+keyword+"\""+"%'"+
                        "and rating < 5";
        String query2="select sum(rating) as res from book";
        int numberofTables=0;
        try {
            Statement statement = this.con.createStatement();
            statement.executeUpdate(query);
            numberofTables++;
            statement.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        ResultSet rs1;
        try{
            Statement st = this.con.createStatement();
            rs1=st.executeQuery(query2);
            rs1.next();
            numberofTables=rs1.getInt("res");
            st.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return numberofTables;
    }

    @Override
    public int function10() {
        String query="delete from publisher WHERE publisherID not in\n" +
                "(select publisherID from book)";
        String query2="select count(*) as res from publisher";
        int numberofTables=0;
        try {
            Statement statement = this.con.createStatement();
            statement.executeUpdate(query);
            numberofTables++;
            statement.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        ResultSet rs1;
        try{
            Statement st = this.con.createStatement();
            rs1=st.executeQuery(query2);
            rs1.next();
            numberofTables=rs1.getInt("res");
            st.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return numberofTables;
    }
}
