
get '/' do
  @docs = $sqldb.all_index_db
  erb :index
end


get '/index.json' do
  content_type :json
  @docs = $sqldb.all_index_db
  @docs.to_json
end
