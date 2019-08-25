
get '/view/:id' do
  redirect "/", 303 unless params[:id]
  id = params[:id].to_i
  redirect "/", 404 if id < 0
  @doc = $sqldb.find_db_by_index(id)
  redirect "/", 404 unless @doc
  @id = id
  @enable_replacepath = true
  erb :view
end


get '/view/src/*' do
  redirect "/", 303 unless params[:splat][0]
  path = params[:splat][0]
  @doc = $sqldb.find_db_by_path(path)
  redirect "/", 404 unless @doc
  @id = @doc[:id]
  erb :view
end
