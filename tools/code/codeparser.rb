require 'kconv'

module Code

  def self.fileload(path)
    dic = {}
    tag = nil
    iscode = false
    uncomment = false
    open(path, 'r') do |io|
      while line_raw = io.gets
        line_raw = line_raw.toutf8.chomp
        if !iscode && (line_raw =~ /^(?:#|\/\/)\s*(\S?.*)$/ || uncomment)
          line = $1 if !uncomment
          if !uncomment && line =~ /^%(\w+)$/
            tag = $1.to_sym
          elsif line_raw =~ /^\W*\s```$/
            uncomment = !uncomment
            next
          elsif tag
            dic[tag] ||= ""
            dic[tag] << (uncomment ? line_raw : line) << "\n"
          end
        else
          iscode = true
          dic[:code] ||= ""
          dic[:code] << line_raw << "\n"
        end
      end
    end
    dic.each do |key, val|
      dic[key] = val.strip
    end
    dic
  end
  
end
