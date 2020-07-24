# frozen_string_literal: true

require 'kconv'

module Code
  module_function

  def fileload(path)
    dic = {}
    tag = nil
    doc_type = nil
    uncomment = false

    do_block = lambda do |line|
      if line =~ /^BEGIN (\w+)$/
        s = Regexp.last_match(1).downcase.to_sym
        # abort unless doc_type.nil?  # 失敗を返す手段を用意していない
        doc_type = s
      elsif line =~ /^END (\w+)$/
        s = Regexp.last_match(1).downcase.to_sym
        # abort unless doc_type == s  # 失敗を返す手段を用意していない
        doc_type = nil
      end
    end

    do_doc = lambda do |line_com, line_raw|
      if line_com && line_com =~ /^%(\w+)$/
        tag = Regexp.last_match(1).to_sym
        next
      end
      if line_com && line_com == '```'
        uncomment = !uncomment
        next
      end
      if tag
        dic[tag] ||= ''
        dic[tag] << (uncomment ? line_raw : line_com) << "\n"
      end
    end

    do_code = lambda do |_line_com, line_raw|
      dic[:code] ||= ''
      dic[:code] << line_raw << "\n"
    end

    io = open(path, 'r')
    while line_raw = io.gets
      line_raw = line_raw.toutf8.chomp
      line_com = comment_line(line_raw)
      if line_com || uncomment
        if line_com && line_com =~ /^%=(.+)$/
          do_block.call Regexp.last_match(1)
          next
        end
      end

      if doc_type == :doc
        do_doc.call line_com, line_raw
      elsif doc_type == :code
        do_code.call line_com, line_raw
      end
    end
    io.close

    dic.each do |key, val|
      dic[key] = val.strip
    end
    dic
  end
end

class << Code
  private

  def comment_line(line)
    return nil unless line =~ %r{^(?:#|//)\s*(\S?.*)$}

    Regexp.last_match(1)
  end
end
