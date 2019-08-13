
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


get '/cart/add/:id' do # TODO: なんでこれGETなんですか？
  redirect '/', 303 unless params[:id]
  id = params[:id].to_i
  @cart['i'].push(id).uniq!
  set_cart(@cart)
  redirect '/', 303
end


get '/cart/rem/:id' do # TODO: なんでこれGETなんですか？
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
