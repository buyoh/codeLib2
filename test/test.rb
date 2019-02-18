
module Tester
  def self.tempdir=(t)
    @tempdir = t
  end
end

def convert_testpath_to_codepath(testpath)
  testpath.gsub("/test/desc/","/src/")
end

Tester.tempdir = "./tmp/"

Dir.glob("./test/desc/**/*").reject{|f| File.directory?(f)}.each do |filepath|
  ext = File.extname(filepath)[1..-1]
  require "./test/src/checker/#{ext}.rb"
  abort "test failed: #{filepath}" unless Tester.cpp("", IO.read(convert_testpath_to_codepath(filepath)), IO.read(filepath))
  puts "pass: #{filepath}"
end
