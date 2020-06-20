Dir.chdir __dir__
require 'optparse'
require_relative './dbhelper/collector.rb'
require_relative './test/test.rb'

@filter = nil
optparser = OptionParser.new
optparser.on('--filter regexp'){|e| @filter = Regexp.new(e) }
optparser.parse!(ARGV)

@tempdir = '/tmp'

failed = false
Dir.chdir('../') do
  Dir.mkdir @tempdir unless Dir.exist? @tempdir

  Document.test_files.each do |path|
    next if @filter && !(path =~ @filter)
    puts "test: #{path}"
    lang = path.split('/')[1]
    langc = Test.const_get(lang.upcase)
    unless langc
      puts 'not implemented language: ' + lang
      next
    end
    tester = langc.new(path, @tempdir)
    if tester.compile && tester.execute
      puts 'ok'
    else
      puts 'failed!!'
    end
  end
end

abort if failed
exit 0
