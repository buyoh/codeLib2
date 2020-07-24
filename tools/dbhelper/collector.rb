# frozen_string_literal: true

require_relative '../code/codeparser'
require_relative '../markup/markup'

module Document
  def self.langs
    raise 'current directory is not root' unless File.directory?('src')

    Dir.glob('./src/*').select { |path| File.directory? path }.map { |path| File.basename(path) }
  end

  def self.src_files
    langs.map { |lang| Dir.glob("src/#{lang}/**/*").select { |file| File.file?(file) } }.flatten
  end

  def self.test_files
    langs.map { |lang| Dir.glob("test/#{lang}/**/*").select { |file| File.file?(file) } }.flatten
  end

  def self.collect_documents
    dic = []
    src_files.each do |path|
      d = Code.fileload(path)
      next unless d.key?(:title)

      d.keys.each do |k|
        d[k] = Markup.parse(d[k])
      end
      d[:path] = path
      dic << d
    end

    dic
  end
end
