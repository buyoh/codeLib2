require './dbhelper/dbwrapper.rb'

class DBHelper

  def initialize
    @dbwrapper = DBWrapperSQLite3.new()
  end

  # テーブルを再作成する
  def create_db()
    @dbwrapper.create_db
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
        @dbwrapper.insert_article(doc[:path], doc[:title], doc[:words])
        id = @dbwrapper.getid_article(doc[:path])
      else
        existed[0][:checked] = true
        id = existed[0][:id]
        @dbwrapper.update_article(id, doc[:path], doc[:title], doc[:words])
      end

      doc.each do |key, val|
        next if key == :title || key == :path || key == :words
        if @dbwrapper.get_descriptor_value(id, doc[:key]).nil?
          @dbwrapper.insert_descriptor(id, key.to_s, val)
        else
          @dbwrapper.update_descriptor(id, key.to_s, val)
        end
      end
    end

    index.each do |article|
      unless article[:checked]
        @dbwrapper.delete_article(article[:id])
        @dbwrapper.delete_descriptors_by_article_id(article[:id])
      end
    end
  end


  # すべての {id, path, title, words} を配列に返す
  def all_index_db()
    @dbwrapper.all_articles() \
      .map{|id,path,title,keyword| {id:id, path:path, title:title, words:keyword} }
  end


  # 検索に一致した {id, path, title, words} の配列を返す
  def search_index_db(path, title, keyword, union="or")
    @dbwrapper.search_articles(path, title, keyword, union) \
      .map{|id,path,title,keyword| {id:id, path:path, title:title, words:keyword} }
  end


  # descriptors テーブルに含まれる情報を集める
  def get_detail(id)
    pairs = @dbwrapper.select_descriptors(id)
    h = {}
    pairs.each do |key, val|
      h[key.to_sym] = val
    end
    h
  end


  # idの値から {id, path, title, words} を求める
  def find_db_by_index(id)
    r = @dbwrapper.get_article(id)
    return nil unless r
    path, title, keyword = r
    { id: id, path: path, title: title, words: keyword }.update(self.get_detail(id))
  end


  # パスの値から id, path, title, words を求める
  def find_db_by_path(path)
    r = @dbwrapper.get_article_path(path)
    return nil unless r
    id, path, title, keyword = r
    id = id.to_i
    { id: id, path: path, title: title, words: keyword }.update(self.get_detail(id))
  end


end
