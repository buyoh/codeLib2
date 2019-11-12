Dir.chdir __dir__
require './util/argv.rb'
require './dbhelper/collector.rb'

ap = ArgParser.new
ap.define_param('--filter')

prm, arr = ap.parse(ARGV)

@filter = prm['--filter']
@filter = Regexp.new(@filter) if @filter

@tempdir = '/tmp'


require './test/test.rb'



failed = false
Dir.chdir('../') do
  Dir.mkdir @tempdir unless Dir.exist? @tempdir

  Document.test_files.each do |path|
    next if @filter && !(path =~ @filter)
    puts "test: #{path}"
    lang = path.split('/')[1]
    tester = Test.const_get(lang.upcase).new(path, @tempdir)
    if tester.compile && tester.execute
      puts "ok"
    else
      puts "failed!!"
    end
  end
end

abort if failed
exit 0
