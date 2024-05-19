require 'pathname'
require_relative '../code/codeparser'

module Collector
  def self.langs(basepath = '.')
    raise 'current directory is not root' unless File.directory?("#{basepath}/src")

    Dir.glob("#{basepath}/src/*").select { |path| File.directory? path }.map { |path| File.basename(path) }
  end

  def self.src_paths(basepath = '.')
    langs(basepath).map { |lang| Dir.glob("#{basepath}/src/#{lang}/**/*").select { |file| File.file?(file) } }.flatten
  end

  def self.src_files(basepath = '.')  # TODO: rename src_paths
    src_paths(basepath)
  end

  def self.test_paths(basepath = '.')
    langs(basepath).map { |lang| Dir.glob("#{basepath}/test/#{lang}/**/*").select { |file| File.file?(file) } }.flatten
  end

  def self.collect_documents(basepath = '.') # TODO: 引数はsrc_files
    dic = []
    src_files(basepath).each do |path|
      d = open(path, 'r') { |io| Code.read_docfile(io) }
      next unless d.key?(:title)

      d[:path] = path
      dic << d
    end

    dic
  end

  # ./src/lang/foo/bar.ext の形式にする
  def self.normalize_path(path, basepath)
    base = File.dirname(File.absolute_path(basepath))
    Pathname.new(File.absolute_path(path)).relative_path_from(base).to_s
  end

  def self.lang_from_path(path)
    path.split('/').reject { |h| h[0] == '.' || h == 'src' || h == 'test' }[0]
  end
end
