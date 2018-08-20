require 'sinatra'
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


# check and recreate
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


def update_db(sqldb)

  docs = nil
  Dir.chdir('../') do
    docs = Document.collect_documents()
  end

  docs.each do |doc|
    doc[:keyword] = "" unless doc[:keyword]

    existed = sqldb.execute("select id from articles where path=?;",doc[:path])

    id = nil
    if existed.empty?
      sqldb.execute("insert into articles(path, title, keyword) values(?,?,?);", [doc[:path],doc[:title],doc[:keyword]])
      id = sqldb.execute("select id from articles where path=?;", doc[:path])[0][0]
    else
      id = existed[0][0]
      sqldb.execute("update articles set path=?, title=?, keyword=? where id=#{id};", [doc[:path],doc[:title],doc[:keyword]])
    end

    doc.each do |key, val|
      next if key == :title || key == :path || key == :keyword
      if sqldb.execute("select keyStr from descriptors where articleId=#{id} and keyStr=?;", doc[:key]).empty?
        sqldb.execute("insert into descriptors(articleId, keyStr, valueStr) values(?,?,?);", [id, key.to_s, val])
      else
        sqldb.execute("update descriptors set valueStr=? where articleId=#{id} and keyStr=?;", [val, key.to_s])
      end
    end
  end

end


def all_index_db(sqldb)
  sqldb.execute("select id,path,title,keyword from articles;") \
    .map{|id,path,title,keyword| {id:id, path:path, title:title, keyword:keyword} }
end


def search_index_db(sqldb, path, title, keyword, union="or")
  # memo: %が*，_が?に該当．部分判定は '%query%'
  where_query = []
  where_value = []
  if path;    where_query << "path like ?";    where_value << path;    end
  if title;   where_query << "title like ?";   where_value << title;   end
  if keyword; where_query << "keyword like ?"; where_value << keyword; end
  return [] if where_value.empty?
  sqldb.execute("select id,path,title,keyword from articles where #{where_query*" #{union} "};", where_value) \
    .map{|id,path,title,keyword| {id:id, path:path, title:title, keyword:keyword} }
end


def find_db(sqldb, id)
  r = sqldb.execute("select path,title,keyword from articles where id=?;", id)
  return nil if r.empty?
  path, title, keyword = r[0]
  pairs = sqldb.execute("select keyStr,valueStr from descriptors where articleId=?", id)

  h = {path:path, title:title, keyword:keyword}
  pairs.each do |key, val|
    h[key.to_sym] = val
  end
  h
end


# - - - - - - - - - - - - - - - - - - - - - - - 


def set_cookie(name, value, expires = 3600*3)
  value = JSON::generate(value) unless value.is_a? String
  logger.info value
  response.set_cookie(name, :value => value, :expires => Time.now + expires, :path => '/')
end

def get_cookie(name)
  j = request.cookies[name]
  h = nil
  logger.info j
  if j
    begin
      logger.info URI::unescape(j)
      h = JSON::parse(URI::unescape(j))
    rescue JSON::JSONError
      h = nil
    end
  end
  h
end


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
  @doc = find_db(sqldb, id)
  @id = id
  erb :view
end


get '/cart' do
  @docs = []
  err = []
  @cart['i'].each do |id|
    doc = find_db(sqldb, id)
    if doc.nil?
      err << id
    else
      doc[:id] = id
      @docs << doc
    end
  end
  # todo: remove err
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
  @cart['i'] = @cart['i'] - [id]
  set_cart(@cart)
  redirect '/', 303
end


get '/index.json' do
  content_type :json
  @docs = all_index_db(sqldb)
  @docs.to_json
end


get '/style.css' do
  content_type 'text/css'
  scss :style
end
