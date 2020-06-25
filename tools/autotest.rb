Dir.chdir __dir__
require 'fileutils'
require 'optparse'
require_relative './dbhelper/collector.rb'
require_relative './test/test.rb'

@filter = nil
@jobs = 1
optparser = OptionParser.new
optparser.on('--filter regexp'){|e| @filter = Regexp.new(e) }
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

def do_job(path, workerid = 0)
  return true if @filter && !(path =~ @filter)
  puts "test: #{path}"

  lang = path.split('/')[1]
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
    return true
  else
    puts 'failed!!'
    return false
  end
end

failed = false
Dir.chdir('../') do

  if @jobs == 1
    Document.test_files.each do |path|
      failed |= !(do_job(path))
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
        while path = pop_paths.call()
          failed |= !(do_job(path, tid))
        end
      end
    end.each{|t| t.join}
  end
end

abort if failed
exit 0
