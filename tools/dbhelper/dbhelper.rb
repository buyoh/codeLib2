require "#{__dir__}/dbwrapper"
require "#{__dir__}/collector"
require "#{__dir__}/gitlog"

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
    lastcommits = nil
    Dir.chdir('../') do
      docs = Document.collect_documents()
      lastcommits = GitLog.collect_all_latest_nocache(docs.map{|doc| doc[:path]})
    end
    index = all_index_db()
    shas = gather_sha()

    docs.each do |doc|
      doc[:words] = '' unless doc[:words]

      commit = lastcommits[doc[:path]] || {sha: '#invalid', date: Time.local(2000), message: ''}
      sha_exists = shas[commit[:sha]]

      commitId = nil
      if sha_exists
        commitId = sha_exists[:id]
      else
        sha = commit[:sha]
        @dbwrapper.insert_gitcommit(sha, commit[:date].to_i, commit[:message])
        commitId = @dbwrapper.get_gitcommit_sha(sha)[0]
        shas[sha] = {id: commitId, date: commit[:date].to_i, message: commit[:message]}
      end

      article_exists = index.select{|article| article[:path] == doc[:path]}

      id = nil
      if article_exists.empty?
        @dbwrapper.insert_article(doc[:path], doc[:title], doc[:words], commitId)
        id = @dbwrapper.getid_article(doc[:path])
        index << {id: id, path: doc[:path], title: doc[:title], words: doc[:words], checked: true}
      else
        article_exists[0][:checked] = true
        a = article_exists[0]
        id = a[:id]
        if a[:commitId] != commitId || a[:title] != doc[:title] || a[:words] != a[:words]
          @dbwrapper.update_article(id, doc[:path], doc[:title], doc[:words], commitId)
        end
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


  # すべての {id, path, title, words, commitId} を配列に返す
  def all_index_db()
    @dbwrapper.all_articles() \
      .map{|id,path,title,words,commitId| {id: id, path: path, title: title, words: words, commitId: commitId} }
  end

  # すべての {id, path, title, words, date, sha} を配列に返す
  def all_index_db_solved()
    @dbwrapper.all_articles_joined_commit() \
      .map{|id,path,title,words,date,sha| {id: id, path: path, title: title, words: words, date: Time.at(date), sha: sha} }
  end


  # 検索に一致した {id, path, title, words, commitId} の配列を返す
  def search_index_db(path, title, words, union="or")
    @dbwrapper.search_articles(path, title, words, union) \
      .map{|id,path,title,words,commitId| {id: id, path: path, title: title, words: words, commitId: commitId} }
  end

  # commitId のコミット情報をdbから取得する
  def get_gitcommit(id)
    id, sha, date, message = @dbwrapper.get_gitcommit(id)
    {id: id, sha: sha, date: Time.at(date), message: message}
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


  # idの値から {id, path, title, words, commitId, ...details} を求める
  def find_db_by_index(id)
    r = @dbwrapper.get_article(id)
    return nil unless r
    path, title, words, commitId = r
    { id: id, path: path, title: title, words: words, commitId: commitId }.update(self.get_detail(id))
  end


  # パスの値から id, path, title, words を求める
  def find_db_by_path(path)
    r = @dbwrapper.get_article_path(path)
    return nil unless r
    id, path, title, keyword, commitId = r
    id = id.to_i
    { id: id, path: path, title: title, words: keyword, commitId: commitId }.update(self.get_detail(id))
  end


  # 
  def gather_sha()
    h = {}
    @dbwrapper.all_gitcommits.each do |id, sha, date, message|
      h[sha] = {id: id, date: Time.at(date), message: message}
    end
    h
  end


end
