Dir.chdir __dir__
require './util/argv.rb'
require './dbhelper/collector.rb'

require './test/test.rb'

ap = ArgParser.new
ap.define_param('--filter')
ap.define_param('--log-path')

prm, arr = ap.parse(ARGV)

@filter = prm['--filter']
@filter = Regexp.new(@filter) if @filter
@log_path = prm['--log-path']

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
