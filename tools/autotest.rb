require 'fileutils'
require 'optparse'
require_relative './collector/collector'
require_relative './test/test'

@filter = nil
@jobs = 1
optparser = OptionParser.new
optparser.on('--filter regexp') { |e| @filter = Regexp.new(e) }
optparser.on('-j', '--jobs number') do |e|
  e = e.to_i
  abort 'invalid arguments(jobs)' if e < 1
  @jobs = e
end
# optparser.on('--log-path path'){|e| }
# optparser.on('--verbose'){|f| }
# optparser.on('--quiet'){|f| }
optparser.parse!(ARGV)

@tempdir = '/tmp/codelib2'

Dir.chdir __dir__
def do_job(path, lang, workerid = 0)
  return true if @filter && path !~ @filter

  puts "test: #{path}"

  langc = Test.const_get(lang.upcase)
  unless langc
    puts 'not implemented language: ' + lang
    return true
  end

  tempdir_worker = @tempdir + '__' + workerid.to_s
  FileUtils.mkdir_p tempdir_worker unless Dir.exist? tempdir_worker
  tester = langc.new(path, tempdir_worker)

  if tester.compile && tester.execute(nochdir: true)
    puts 'ok'
    true
  else
    puts 'failed!!'
    false
  end
end

failed = false
Dir.chdir('../') do
  if @jobs == 1
    Document.test_files.each do |path|
      lang = path.split('/')[2] # /test/cpp/hoge...
      failed |= !do_job(path, lang)
    end
  else
    mtx = Mutex.new
    paths = Document.test_files.clone
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
          lang = path.split('/')[2]
          failed |= !do_job(path, lang, tid)
        end
      end
    end.each(&:join)
  end
end

abort if failed
exit 0
