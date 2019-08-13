
require 'kconv'

module Document

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

  def self.collect_documents
    throw "current directory is not root" unless File.directory?("src")

    files = Dir.glob("src/**/*").select{|file| File.file?(file) }.to_a
    
    dic = []
    files.each do |file|
      d = fileload(file)
      d[:path] = file
      dic << d if d.key?(:title)
    end

    return dic
  end
end