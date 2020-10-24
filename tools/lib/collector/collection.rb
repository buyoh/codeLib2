require_relative './collector'
require_relative '../util/relative_path'
require_relative '../code/codeparser'

class Collection
  attr_reader :base_path, :src_docs, :src_paths, :test_paths, :src_relations, :langs
  def initialize(base_path = '.')
    @base_path = base_path
    @codes = {}

    # avoid '/foo/bar//baz'
    @base_path = File.absolute_path(@base_path)
    @base_path = @base_path[0..-2] if @base_path[-1] == '/'

    # replace to './src/lang/folder/item.hpp'
    @src_paths = Collector.src_paths(@base_path).map { |e| relative_path(e, @base_path + '/src')}.sort
    @test_paths = Collector.test_paths(@base_path).map { |e| relative_path(e, @base_path + '/src')}.sort
    @langs = Collector.langs(@base_path)
    @src_docs = @src_paths.map do |path|
      # '/base/path/./src/lang/folder/item.hpp'
      d = open(@base_path + '/' + path, 'r') { |io| Code.read_docfile(io)}
      next nil unless d.key?(:title)

      d[:path] = path
      d
    end.compact

    # note:
    # dependencies の情報は doc から取れるので不要（パースもしていないので#include...の形でしか取れない）
    @src_relations = @src_paths.map { |path| [path, { tested_by: [] }]}.to_h

    # calculate tested_by
    @test_paths.each do |test_path|
      nom_test_path = relative_path(@base_path + '/' + test_path, @base_path + '/src')
      dfs = lambda do |path|
        cod = code(path)
        cod and cod[:dependencies].each do |dep_path|
          nom_dep_path = relative_path(@base_path + '/' + dep_path, @base_path + '/src')
          @src_relations[nom_dep_path][:tested_by] << nom_test_path if @src_relations[nom_dep_path]
          dfs.call(nom_dep_path)
        end
      end
      dfs.call(nom_test_path)
    end
  end

  def code(path)
    return @codes[path] if @codes.key?(path)

    path2 = @base_path + '/' + path
    lang = Collector.lang_from_path(path)
    return nil unless File.exist? path2
    return nil unless @langs.include? lang

    @codes[path] = open(path2, 'r') { |io| Code.read_code(io, lang)}
  end
end
