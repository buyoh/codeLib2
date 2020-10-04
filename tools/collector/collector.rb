require_relative '../code/codeparser'

module Document
  def self.langs(basepath = '.')
    raise 'current directory is not root' unless File.directory?("#{basepath}/src")

    Dir.glob("#{basepath}/src/*").select { |path| File.directory? path }.map { |path| File.basename(path) }
  end

  def self.src_files(basepath = '.')
    langs(basepath).map { |lang| Dir.glob("#{basepath}/src/#{lang}/**/*").select { |file| File.file?(file) } }.flatten
  end

  def self.test_files(basepath = '.')
    langs(basepath).map { |lang| Dir.glob("#{basepath}/test/#{lang}/**/*").select { |file| File.file?(file) } }.flatten
  end

  def self.collect_documents(basepath = '.')
    dic = []
    src_files(basepath).each do |path|
      d = Code.fileload(path)
      next unless d.key?(:title)

      d[:path] = path
      dic << d
    end

    dic
  end
end
