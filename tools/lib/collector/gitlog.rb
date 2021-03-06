require 'time'
require_relative './collector'

module GitLog
  # 事前知識なしでfilesのすべてのlatest commitの情報を取得する。
  # files: Collector.src_paths
  # @result: filepath => {sha, date, message}
  def self.collect_all_latest_nocache(files, per_skip = 4, base_path = nil)
    arg_basepath = base_path ? "-C #{base_path}" : ''
    db = files.each_with_object({}) { |f, s| s[f] = nil; }
    remain = files.size

    skip = 0
    while remain > 0
      none = true
      IO.popen("git log #{arg_basepath} -n #{per_skip} --skip=#{skip}" + \
        " --name-only --oneline --pretty=format:'%H;%cd;%s'", 'r') do |io|
        commit = nil
        while line = io.gets
          line.chomp!
          if line.empty?
            commit = nil
            next
          end
          none = false
          if commit.nil?
            # commit line
            s, d, m = line.split(';')
            commit = { sha: s, date: Time.parse(d), message: m }
          else
            # filename line
            filename = line
            next unless filename =~ %r{^src/.+/}

            if db[filename].nil?
              remain -= 1
              db[filename] = commit
            end
          end
        end
        io.close
      end
      break if none

      skip += per_skip
    end
    db
  end

  # path のファイルに対して、全てのコミットログを取得する。
  # @return: [{sha, date, message}, ...]
  def self.history(path, options, base_path = nil)
    arg_basepath = base_path ? "-C #{base_path}" : ''
    `git #{arg_basepath} log --oneline --pretty=format:'%H;%cd;%s' #{format_options(options)} #{path}` \
      .split("\n") \
      .reject(&:empty?).map { |l| s, d, m = l.split(';'); { sha: s, date: Time.parse(d), message: m } }
  end

  def self.format_options(hash, ignore = {})
    str = ''
    str << "-n #{hash[:n]} " if hash[:n] && !ignore[:n]
    str << "--after #{date_format(hash[:after])} " if hash[:after] && !ignore[:after]
    str
  end
  private_class_method :format_options

  def self.date_format(date) # date yyyy-mm-dd
    format('%04d-%02d-%02d', date.year, date.month, date.day)
  end
  private_class_method :date_format
end
