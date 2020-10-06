#!/usr/bin/env ruby

Dir.chdir __dir__
require 'fileutils'
require 'optparse'
require_relative './lib/collector/collector'
require_relative './lib/test/tester'

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

def do_job(path, lang, tid = 0)
  return true if @filter && path !~ @filter

  puts "test: #{path}"
  unless Tester.const_defined?(lang.upcase)
    puts 'not implemented language: ' + lang
    return true
  end

  tempdir = @tempdir + tid.to_s
  FileUtils.mkdir_p tempdir unless Dir.exist? tempdir
  langc = Tester.const_get(lang.upcase)
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
    Collector.src_files.each do |path|
      lang = path.split('/')[2]
      failed |= do_job(path, lang)
    end
  else
    mtx = Mutex.new
    paths = Collector.src_files.clone
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
          lang = path.split('/')[1]
          failed |= !do_job(path, lang, tid)
        end
      end
    end.each(&:join)
  end
end

# logio.close if logio

abort if failed
exit 0
