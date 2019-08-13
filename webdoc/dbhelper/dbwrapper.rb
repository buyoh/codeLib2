require 'fileutils'
require 'sqlite3'

class DBWrapper

  def initialize
    Dir.mkdir('tmp') unless Dir.exist?('tmp')
    FileUtils::touch('tmp/codes.db') unless File.exist?('tmp/codes.db')
    @sqldb = SQLite3::Database.new('tmp/codes.db')
  end

  # テーブルを再作成する
  # 必要に応じてdropする
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

  # DBを更新する
  # 必要に応じてレコードの更新・削除など．
  def update_db()

    docs = nil
    Dir.chdir('../') do
      docs = Document.collect_documents()
    end

    index = all_index_db()

    docs.each do |doc|
      doc[:words] = '' unless doc[:words]

      # existed = @sqldb.execute("select id from articles where path=?;", doc[:path])
      
      existed = index.select{|article| article[:path] == doc[:path]}

      id = nil
      if existed.empty?
        @sqldb.execute("insert into articles(path, title, keyword) values(?,?,?);", [doc[:path], doc[:title], doc[:words]])
        id = @sqldb.execute("select id from articles where path=?;", doc[:path])[0][0]
      else
        existed[0][:checked] = true
        id = existed[0][:id]
        @sqldb.execute("update articles set path=?, title=?, keyword=? where id=#{id};", [doc[:path], doc[:title], doc[:words]])
      end

      doc.each do |key, val|
        next if key == :title || key == :path || key == :words
        if @sqldb.execute("select keyStr from descriptors where articleId=#{id} and keyStr=?;", doc[:key]).empty?
          @sqldb.execute("insert into descriptors(articleId, keyStr, valueStr) values(?,?,?);", [id, key.to_s, val])
        else
          @sqldb.execute("update descriptors set valueStr=? where articleId=#{id} and keyStr=?;", [val, key.to_s])
        end
      end
    end

    index.each do |article|
      unless article[:checked]
        @sqldb.execute("delete from articles where id=?", article[:id])
        @sqldb.execute("delete from descriptors where articleId=?", article[:id])
      end
    end
  end


  # すべての {id, path, title, words} を配列に返す
  def all_index_db()
    @sqldb.execute("select id,path,title,keyword from articles;") \
      .map{|id,path,title,keyword| {id:id, path:path, title:title, words:keyword} }
  end


  # 検索に一致した {id, path, title, words} の配列を返す
  # memo: %が*，_が?に該当．部分判定は '%query%'
  def search_index_db(path, title, keyword, union="or")
    where_query = []
    where_value = []
    if path;    where_query << "path like ?";    where_value << path;    end
    if title;   where_query << "title like ?";   where_value << title;   end
    if keyword; where_query << "keyword like ?"; where_value << keyword; end
    return [] if where_value.empty?
    @sqldb.execute("select id,path,title,keyword from articles where #{where_query*" #{union} "};", where_value) \
      .map{|id,path,title,keyword| {id:id, path:path, title:title, words:keyword} }
  end


  # descriptors テーブルに含まれる情報を集める
  def get_detail(id)
    pairs = @sqldb.execute("select keyStr,valueStr from descriptors where articleId=?;", id)
    h = {}
    pairs.each do |key, val|
      h[key.to_sym] = val
    end
    h
  end


  # idの値から {id, path, title, words} を求める
  def find_db_by_index(id)
    r = @sqldb.execute("select path,title,keyword from articles where id=?;", id)
    return nil if r.empty?
    path, title, keyword = r[0]
    { id: id, path: path, title: title, words: keyword }.update(self.get_detail(id))
  end


  # パスの値から id, path, title, words を求める
  def find_db_by_path(path)
    r = @sqldb.execute("select id,path,title,keyword from articles where path like ?;", '%'+path)
    return nil if r.empty?
    id, path, title, keyword = r[0]
    id = id.to_i
    { id: id, path: path, title: title, words: keyword }.update(self.get_detail(id))
  end


end
