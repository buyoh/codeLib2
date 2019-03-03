require 'sinatra'
require 'sinatra/reloader' if development?
# require 'sinatra/cookies'
require 'sass'
require 'json'
require 'uri'
require 'logger'

require './back/dbwrapper'
require './back/collector'
require './back/dbutil'

sqldb = DBWrapper.new


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

  sqldb.create_db
  sqldb.update_db

end


before do
  @cart = get_cart()
end


get '/reload' do
  file = 'tmp/reload.log'
  if !File.exist?(file) || Time.now - File.mtime(file) > 300
    FileUtils.touch file
    sqldb.update_db
  end
  redirect "/", 303
end


get '/' do
  @docs = sqldb.all_index_db
  erb :index
end


get '/search' do
  redirect "/", 303 unless params[:word]
  @searchKeyword = params[:word]
  q = "%#{@searchKeyword}%"
  @docs = sqldb.search_index_db(q, q, q)
  
  redirect to("/view/%d"%[@docs[0][:path]]) if @docs.size == 1
  erb :index
end


get '/view/:id' do
  redirect "/", 303 unless params[:id]
  id = params[:id].to_i
  redirect "/", 404 if id < 0
  @doc = sqldb.find_db_by_index(id)
  redirect "/", 404 unless @doc
  @id = id
  @enable_replacepath = true
  erb :view
end


get '/view/src/*' do
  redirect "/", 303 unless params[:splat][0]
  path = params[:splat][0]
  @doc = sqldb.find_db_by_path(path)
  redirect "/", 404 unless @doc
  @id = @doc[:id]
  erb :view
end
 

get '/cart' do
  @docs = []
  err = []
  @cart['i'].each do |id|
    doc = sqldb.find_db_by_index(id)
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

  solved = DBUtil.solve_paths(sqldb, @cart['i'].map{|id| DBUtil.id_to_path(sqldb, id)})
  @gen_code = DBUtil.generate_merged_code(solved[:path_sequence], solved[:docs])

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
  @docs = sqldb.all_index_db
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
