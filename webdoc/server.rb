require 'sinatra'
require 'sass'
require 'json'

require './back/collector'


configure do

  docs = nil
  Dir.chdir('../') do
    docs = Document.collect_documents()
  end
  docs.each_index do |idx|
    docs[idx][:id] = idx
  end
  set docs: docs

end


get '/' do
  @docs = settings.docs
  erb :index
end


get '/search' do
  redirect "/", 303 unless params[:word]
  @searchKeyword = params[:word]
  r = Regexp.new(@searchKeyword, Regexp::IGNORECASE)
  @docs = settings.docs.select{|d|d[:words]=~r || d[:path]=~r || d[:title]=~r}

  redirect to("/view/%d"%[@docs[0][:id]]) if @docs.size == 1
  erb :index
end


get '/view/:id' do
  redirect "/", 303 unless params[:id]
  id = params[:id].to_i
  redirect "/", 303 if id < 0 || settings.docs.size <= id 
  @doc = settings.docs[id]
  erb :view
end


get '/index.json' do
  content_type :json
  @docs = settings.docs
  @docs.to_json
end


get '/style.css' do
  content_type 'text/css'
  scss :style
end
