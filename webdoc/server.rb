require 'sinatra'
require 'sinatra/reloader' if development?
# require 'sinatra/cookies'
require 'sass'
require 'json'
require 'uri'
require 'logger'

require './dbhelper/dbwrapper'
require './dbhelper/collector'
require './dbhelper/dbutil'

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


Dir.glob('./controller/*.rb') do |c|
  load c
end

