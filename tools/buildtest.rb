Dir.chdir __dir__
require 'optparse'
require './dbhelper/collector.rb'

require './test/test.rb'

@filter = nil
optparser = OptionParser.new
optparser.on('--filter regexp'){|e| @filter = Regexp.new(e) }
optparser.on('--log-path path'){|e| @log_path = e }
optparser.parse!(ARGV)


@tempdir = '/tmp'

logio = @log_path ? open(@log_path) : nil

failed = false
Dir.chdir('../') do
  Dir.mkdir @tempdir unless Dir.exist? @tempdir

  Document.src_files.each do |path|
    next if @filter && !(path =~ @filter)
    puts "test: #{path}"
    lang = path.split('/')[1]
    unless Test.const_defined?(lang.upcase)
      puts 'not implemented language: ' + lang
      next
    end
    langc = Test.const_get(lang.upcase)
    tester = langc.new(path, @tempdir)
    result = tester.test_compilable
    if result['default']
      if result.all?{|k,v| v}
        puts 'ok'
      else
        puts 'partially ok'
      end
    else
      puts 'failed!!'
      failed = true
    end
  end
end

logio.close if logio

abort if failed
exit 0
