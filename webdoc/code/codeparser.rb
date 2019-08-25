require 'kconv'

module Code

  def self.fileload(path)
    dic = {}
    tag = nil
    iscode = false
    open(path, 'r') do |io|
      while line = io.gets
        line = line.toutf8.chomp
        if !iscode && (line=~/^(?:#|\/\/)\s*(\S?.*)$/)
          line = $1
          if line=~/^%(\w+)$/
            tag = $1.to_sym
          elsif tag
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
  
end
