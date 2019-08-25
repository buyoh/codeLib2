
get '/search' do
  redirect "/", 303 unless params[:word]
  @searchKeyword = params[:word]
  q = "%#{@searchKeyword}%"
  @docs = $sqldb.search_index_db(q, q, q)
  
  redirect "/view/%s" % [@docs[0][:path]] if @docs.size == 1
  erb :index
end

