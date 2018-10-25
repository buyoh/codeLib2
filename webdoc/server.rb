require 'sinatra'
require 'sinatra/reloader' if development?
# require 'sinatra/cookies'
require 'sass'
require 'json'
require 'fileutils'
require 'uri'
require 'logger'

require './back/collector'

require 'sqlite3'

Dir.mkdir('tmp') unless Dir.exist?('tmp')
FileUtils::touch('tmp/codes.db') unless File.exist?('tmp/codes.db')
sqldb = SQLite3::Database.new('tmp/codes.db')


# テーブルを再作成する
# 必要に応じてdropする
def create_db(sqldb)

  count_a = sqldb.execute("select count(*) from sqlite_master where type='table' and name='articles';")[0][0]
  count_d = sqldb.execute("select count(*) from sqlite_master where type='table' and name='descriptors';")[0][0]

  if count_a > 0
    if sqldb.execute("pragma table_info(articles);").sort != [[0, "id", "integer", 0, nil, 1], [1, "path", "text", 0, nil, 0], [2, "title", "text", 0, nil, 0], [3, "keyword", "text", 0, nil, 0]]
      sqldb.execute("drop table articles")
      count_a = 0
    end
  end
  if count_d > 0
    if sqldb.execute("pragma table_info(descriptors);").sort != [[0, "articleId", "integer", 0, nil, 0], [1, "keyStr", "text", 0, nil, 0], [2, "valueStr", "text", 0, nil, 0]]
      sqldb.execute("drop table descriptors")
      count_d = 0
    end
  end

  sqldb.execute(<<-ENDSQL) if count_a == 0
    create table articles (
      id integer primary key autoincrement,
      path text unique,
      title text,
      keyword text
    );
  ENDSQL
  sqldb.execute(<<-ENDSQL) if count_d == 0
  create table descriptors (
    articleId integer ,
    keyStr text,
    valueStr text
  );
  ENDSQL
end


# DBを更新する
# 必要に応じてレコードの更新・削除など．
def update_db(sqldb)

  docs = nil
  Dir.chdir('../') do
    docs = Document.collect_documents()
  end

  index = all_index_db(sqldb)

  docs.each do |doc|
    doc[:words] = '' unless doc[:words]

    # existed = sqldb.execute("select id from articles where path=?;", doc[:path])
    
    existed = index.select{|article| article[:path] == doc[:path]}

    id = nil
    if existed.empty?
      sqldb.execute("insert into articles(path, title, keyword) values(?,?,?);", [doc[:path], doc[:title], doc[:words]])
      id = sqldb.execute("select id from articles where path=?;", doc[:path])[0][0]
    else
      existed[0][:checked] = true
      id = existed[0][:id]
      sqldb.execute("update articles set path=?, title=?, keyword=? where id=#{id};", [doc[:path], doc[:title], doc[:words]])
    end

    doc.each do |key, val|
      next if key == :title || key == :path || key == :words
      if sqldb.execute("select keyStr from descriptors where articleId=#{id} and keyStr=?;", doc[:key]).empty?
        sqldb.execute("insert into descriptors(articleId, keyStr, valueStr) values(?,?,?);", [id, key.to_s, val])
      else
        sqldb.execute("update descriptors set valueStr=? where articleId=#{id} and keyStr=?;", [val, key.to_s])
      end
    end
  end

  index.each do |article|
    unless article[:checked]
      sqldb.execute("delete from articles where id=?", article[:id])
      sqldb.execute("delete from descriptors where articleId=?", article[:id])
    end
  end
end


# すべての {id, path, title, words} を配列に返す
def all_index_db(sqldb)
  sqldb.execute("select id,path,title,keyword from articles;") \
    .map{|id,path,title,keyword| {id:id, path:path, title:title, words:keyword} }
end


# 検索に一致した {id, path, title, words} の配列を返す
# memo: %が*，_が?に該当．部分判定は '%query%'
def search_index_db(sqldb, path, title, keyword, union="or")
  where_query = []
  where_value = []
  if path;    where_query << "path like ?";    where_value << path;    end
  if title;   where_query << "title like ?";   where_value << title;   end
  if keyword; where_query << "keyword like ?"; where_value << keyword; end
  return [] if where_value.empty?
  sqldb.execute("select id,path,title,keyword from articles where #{where_query*" #{union} "};", where_value) \
    .map{|id,path,title,keyword| {id:id, path:path, title:title, words:keyword} }
end


# descriptors テーブルに含まれる情報を集める
def get_detail(sqldb, id)
  pairs = sqldb.execute("select keyStr,valueStr from descriptors where articleId=?;", id)
  h = {}
  pairs.each do |key, val|
    h[key.to_sym] = val
  end
  h
end


# idの値から {id, path, title, words} を求める
def find_db_by_index(sqldb, id)
  r = sqldb.execute("select path,title,keyword from articles where id=?;", id)
  return nil if r.empty?
  path, title, keyword = r[0]
  { id: id, path: path, title: title, words: keyword }.update(get_detail(sqldb, id))
end


# パスの値から id, path, title, words を求める
def find_db_by_path(sqldb, path)
  r = sqldb.execute("select id,path,title,keyword from articles where path like ?;", '%'+path)
  return nil if r.empty?
  id, path, title, keyword = r[0]
  id = id.to_i
  { id: id, path: path, title: title, words: keyword }.update(get_detail(sqldb, id))
end


# - - - - - - - - - - - - - - - - - - - - - - - 

# idxs: Array
# idxs に含まれる id について 記事を収集してdocsにまとめる
# return: {idxs: solved_idxs, docs: solved_docs}
def solve_idxs(sqldb, idxs)
  docs = []
  solved_idxs = idxs.clone

  dfs = lambda do |path|
    doc = find_db_by_path(sqldb, path)
    next unless doc
    id = doc[:id]

    next if solved_idxs.include?(id)
    docs << doc
    solved_idxs.unshift(id)

    next unless doc[:require]
    doc[:require].split.each do |pa|
      dfs.call(pa.strip)
    end
  end

  idxs.each do |idx|
    doc = find_db_by_index(sqldb, idx)
    next unless doc
    docs << doc
    next unless doc[:require]
    doc[:require].split.each do |path|
      dfs.call(path.strip)
    end
  end
  { idxs: solved_idxs, docs: docs }
end


def generate_merged_code(idxs, docs)
  code = ''
  idxs.each do |id|
    doc = docs.find{|d| d[:id] == id}
    next unless doc
    code += doc[:code] + "\n"
  end
  code
end


# - - - - - - - - - - - - - - - - - - - - - - - 


def set_cookie(name, value, expires = 3600*3)
  value = JSON::generate(value) unless value.is_a? String
  response.set_cookie(name, :value => value, :expires => Time.now + expires, :path => '/')
end

def get_cookie(name)
  j = request.cookies[name]
  h = nil
  if j
    begin
      h = JSON::parse(URI::unescape(j))
    rescue JSON::JSONError
      h = nil
    end
  end
  h
end


# - - - - - - - - - - - - - - - - - - - - - - - 


def init_cart()
  set_cookie('cart', '{"i":[]}', 3600*3)
  return {'i'=>[]}
end

def get_cart()
  cart = get_cookie('cart')
  cart = init_cart() if cart.nil?
  cart
end

def set_cart(cart)
  set_cookie('cart', cart)
end

def include_cart(cart, id)
  cart['i'].include?(id)
end


# - - - - - - - - - - - - - - - - - - - - - - - 


configure do

  create_db sqldb
  update_db sqldb

end


before do
  @cart = get_cart()

end


get '/reload' do
  file = 'tmp/reload.log'
  if !File.exist?(file) || Time.now - File.mtime(file) > 300
    FileUtils.touch file
    update_db sqldb
  end
  redirect "/", 303
end


get '/' do
  @docs = all_index_db(sqldb)
  erb :index
end


get '/search' do
  redirect "/", 303 unless params[:word]
  @searchKeyword = params[:word]
  q = "%#{@searchKeyword}%"
  @docs = search_index_db(sqldb, q, q, q)
  
  redirect to("/view/%d"%[@docs[0][:id]]) if @docs.size == 1
  erb :index
end


get '/view/:id' do
  redirect "/", 303 unless params[:id]
  id = params[:id].to_i
  redirect "/", 303 if id < 0
  @doc = find_db_by_index(sqldb, id)
  @id = id
  erb :view
end


get '/cart' do
  @docs = []
  err = []
  @cart['i'].each do |id|
    doc = find_db_by_index(sqldb, id)
    if doc.nil?
      err << id
    else
      doc[:id] = id
      @docs << doc
    end
  end

  unless err.empty?
    @cart['i'] = @cart['i'] - err
    set_cart(@cart)
  end

  solved = solve_idxs(sqldb, @cart['i'])
  @gen_code = generate_merged_code(solved[:idxs], solved[:docs])

  erb :cart
end


get '/cart/add/:id' do
  redirect '/', 303 unless params[:id]
  id = params[:id].to_i
  @cart['i'].push(id).uniq!
  set_cart(@cart)
  redirect '/', 303
end


get '/cart/rem/:id' do
  redirect '/', 303 unless params[:id]
  id = params[:id].to_i
  if id < 0
    @cart['i'] = []
  else
    @cart['i'] = @cart['i'] - [id]
  end
  set_cart(@cart)
  redirect '/', 303
end


get '/index.json' do
  content_type :json
  @docs = all_index_db(sqldb)
  @docs.to_json
end


get '/version.json' do
  content_type :json
  {mtime: File.mtime('./server.rb').to_s}.to_json
end


get '/style.css' do
  content_type 'text/css'
  scss :style
end
