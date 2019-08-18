require 'fileutils'
require 'sqlite3'

class DBWrapperSQLite3

  def initialize
    Dir.mkdir('tmp') unless Dir.exist?('tmp')
    FileUtils::touch('tmp/codes.db') unless File.exist?('tmp/codes.db')
    @sqldb = SQLite3::Database.new('tmp/codes.db')
  end

  def create_db()

    count_a = @sqldb.execute("select count(*) from sqlite_master where type='table' and name='articles';")[0][0]
    count_d = @sqldb.execute("select count(*) from sqlite_master where type='table' and name='descriptors';")[0][0]

    if count_a > 0
      if @sqldb.execute("pragma table_info(articles);").sort != [[0, "id", "integer", 0, nil, 1], [1, "path", "text", 0, nil, 0], [2, "title", "text", 0, nil, 0], [3, "keyword", "text", 0, nil, 0]]
        @sqldb.execute("drop table articles")
        count_a = 0
      end
    end
    if count_d > 0
      if @sqldb.execute("pragma table_info(descriptors);").sort != [[0, "articleId", "integer", 0, nil, 0], [1, "keyStr", "text", 0, nil, 0], [2, "valueStr", "text", 0, nil, 0]]
        @sqldb.execute("drop table descriptors")
        count_d = 0
      end
    end

    @sqldb.execute(<<-ENDSQL) if count_a == 0
      create table articles (
        id integer primary key autoincrement,
        path text unique,
        title text,
        keyword text
      );
    ENDSQL
    @sqldb.execute(<<-ENDSQL) if count_d == 0
    create table descriptors (
      articleId integer ,
      keyStr text,
      valueStr text
    );
    ENDSQL
  end

  # 検索に一致した {id, path, title, words} の配列を返す
  # memo: %が*，_が?に該当．部分判定は '%query%'
  def search_articles(path, title, keyword, union="or")
    where_query = []
    where_value = []
    if path;    where_query << "path like ?";    where_value << path;    end
    if title;   where_query << "title like ?";   where_value << title;   end
    if keyword; where_query << "keyword like ?"; where_value << keyword; end
    return [] if where_value.empty?
    @sqldb.execute("select id,path,title,keyword from articles where #{where_query*" #{union} "};", where_value)
  end

  def all_articles()
    @sqldb.execute("select id, path, title, keyword from articles;")
  end

  def get_article(id)
    @sqldb.execute("select path, title, keyword from articles where id=?;", id)[0]
  end

  def get_article_path(path)
    @sqldb.execute("select id, path, title, keyword from articles where path like ?;", '%'+path)[0]
  end

  def getid_article(path)
    @sqldb.execute("select id from articles where path=?;", path)[0][0]
  end

  def insert_article(path, title, keyword)
    @sqldb.execute("insert into articles(path, title, keyword) values(?,?,?);", [path, title, keyword])
  end

  def update_article(id, path, title, keyword)
    @sqldb.execute("update articles set path=?, title=?, keyword=? where id=#{id};", [path, title, keyword])
  end

  def delete_article(id)
    @sqldb.execute("delete from articles where id=?", id)
  end

  def select_descriptors(article_id)
    @sqldb.execute("select keyStr,valueStr from descriptors where articleId=?;", article_id)
  end

  def get_descriptor_value(article_id, key)
    @sqldb.execute("select keyStr from descriptors where articleId=#{article_id} and keyStr=?;", key)[0]
  end

  def insert_descriptor(article_id, key, val)
    @sqldb.execute("insert into descriptors(articleId, keyStr, valueStr) values(?,?,?);", [article_id, key, val])
  end

  def update_descriptor(article_id, key, val)
    @sqldb.execute("update descriptors set valueStr=? where articleId=#{article_id} and keyStr=?;", [val, key])
  end

  def delete_descriptors_by_article_id(article_id)
    @sqldb.execute("delete from descriptors where articleId=?", article_id)
  end
  

end