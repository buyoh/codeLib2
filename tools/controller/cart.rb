if $enable_cart


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


get '/cart' do
  @docs = []
  err = []
  @cart['i'].each do |id|
    doc = $sqldb.find_db_by_index(id)
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

  solved = DBSolver.solve_paths($sqldb, @cart['i'].map{|id| DBSolver.id_to_path($sqldb, id)})
  @gen_code = DBSolver.generate_merged_code(solved[:path_sequence], solved[:docs])

  erb :cart
end


before do
  @cart = get_cart()
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


end
