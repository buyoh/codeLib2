
get '/reload' do
  file = 'tmp/reload.log'
  if !File.exist?(file) || Time.now - File.mtime(file) > 300
    FileUtils.touch file
    $sqldb.update_db
  end
  redirect "/", 303
end


get '/version.json' do
  content_type :json
  {mtime: File.mtime('./server.rb').to_s}.to_json
end
