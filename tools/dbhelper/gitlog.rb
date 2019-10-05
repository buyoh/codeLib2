require "#{__dir__}/collector.rb"

module GitLog

  def self.collect_all_nocache(n = 4)
    # 深さを指定してコミットとファイルを取得
    files = Document.src_files
    db = files.reduce(Hash.new){|s,f| s[f] = nil; s}
    remain = files.size

    skip = 0
    while remain > 0
      none = true
      IO.popen("git log -n #{n} --skip=#{skip} --name-only --oneline --pretty=format:'%h;%cd;%s' --date=short", 'r') do |io|
        commit = nil
        while line = io.gets
          line.chomp!
          if line == ''
            commit = nil
            next
          else
            none = false
          end
          if commit.nil?
            # commit line
            commit = line.split(';')
          else
            # filename
            filename = line
            next unless filename =~ /^src\/.+\//
            remain -= 1 if db[filename].nil?
            db[filename] ||= commit
          end
        end
        io.close
      end
      break if none
      skip += n
    end
    db
  end
  
  def self.history(path) # date yyyy-mm-dd
    # TODO: bound date
    # `git log $path --oneline`.split("\n")
  end
end
