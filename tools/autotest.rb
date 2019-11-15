Dir.chdir __dir__
require './util/argv.rb'
require './dbhelper/collector.rb'

require './test/test.rb'

ap = ArgParser.new
ap.define_param('--filter')

prm, arr = ap.parse(ARGV)

@filter = prm['--filter']
@filter = Regexp.new(@filter) if @filter

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
