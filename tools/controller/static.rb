
get '/style.css' do
  content_type 'text/css'
  scss :style
end
