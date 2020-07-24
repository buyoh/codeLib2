Dir.chdir __dir__
require 'fileutils'
require 'optparse'
require_relative './dbhelper/collector.rb'

require_relative './test/test.rb'

@filter = nil
@jobs = 1
optparser = OptionParser.new
optparser.on('--filter regexp') { |e| @filter = Regexp.new(e) }
optparser.on('-j', '--jobs number') do |e|
  e = e.to_i
  abort 'invalid arguments(jobs)' if e < 1
  @jobs = e
end
# optparser.on('--log-path path'){|e| @log_path = e }
optparser.parse!(ARGV)

@tempdir = '/tmp/codelib2'

# logio = @log_path ? open(@log_path, 'w') : nil

def do_job(path, tid = 0)
  return true if @filter && path !~ @filter

  puts "test: #{path}"

  lang = path.split('/')[1]
  unless Test.const_defined?(lang.upcase)
    puts 'not implemented language: ' + lang
    return true
  end

  tempdir = @tempdir + tid.to_s
  FileUtils.mkdir_p tempdir unless Dir.exist? tempdir
  langc = Test.const_get(lang.upcase)
  tester = langc.new(path, tempdir)

  result = tester.test_compilable
  if result['default']
    if result.all? { |_k, v| v }
      puts 'ok'
    else
      puts 'partially ok'
    end
    true
  else
    puts 'failed!!'
    false
  end
end

failed = false
Dir.chdir('../') do
  if @jobs == 1
    Document.src_files.each do |path|
      failed |= do_job(path)
    end
  else
    mtx = Mutex.new
    paths = Document.src_files.clone
    pop_paths = lambda do
      res = nil
      mtx.synchronize do
        res = paths.shift unless paths.empty?
      end
      next res
    end

    @jobs.times.map do |tid|
      Thread.new(tid) do |tid|
        while path = pop_paths.call
          failed |= !do_job(path, tid)
        end
      end
    end.each(&:join)
  end
end

# logio.close if logio

abort if failed
exit 0
