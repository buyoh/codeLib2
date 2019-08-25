require './code/codeparser.rb'
require './markup/markup.rb'

module Document

  def self.collect_documents
    raise RuntimeError.new("current directory is not root") unless File.directory?("src")

    files = Dir.glob("src/**/*").select{|file| File.file?(file) }.to_a
    
    dic = []
    files.each do |path|
      d = Code.fileload(path)
      if d.key?(:title)
        d.keys.each do |k|
          d[k] = Markup.parse(d[k])
        end
        d[:path] = path
        dic << d
      end
    end

    return dic
  end
end
