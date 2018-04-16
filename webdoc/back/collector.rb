
require 'kconv'

module Document

  def self.collect_documents
    throw "current directory is not root" unless File.directory?("src")

    # -------------------
    
    def self.fileload(path)
      dic = {}
      tag = :u
      iscode = false
      open(path, 'r') do |io|
        while line = io.gets
          line = line.toutf8.chomp
          if !iscode && (line=~/^(?:#|\/\/)\s*(\S?.*)$/)
            line = $1
            if line=~/^%(\w+)$/
              tag = $1.to_sym
            else
              dic[tag] ||= ""
              dic[tag] << line << "\n"
            end
          else
            iscode = true
            dic[:code] ||= ""
            dic[:code] << line << "\n"
          end
        end
      end
      dic.each do |key, val|
        dic[key] = val.strip
      end
      dic
    end
    
    # -------------------
    
    files = []
    
    dfs = lambda do |path|
      if File.directory?(path)
        Dir.foreach(path) do |f|
          dfs.call "#{path}/#{f}" if f[0] != '.'
        end
      else
        files << path
      end
    end
    
    Dir.foreach("src") do |lang|
      next if lang[0] == '.'
      dfs.call "src/#{lang}"
    end
    
    # -------------------
    
    dic = []
    files.each do |file|
      d = fileload(file)
      d[:path] = file
      dic << d if d.key?(:title)
    end

    return dic
  end
end