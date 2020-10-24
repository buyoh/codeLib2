#!/usr/bin/env ruby

# docの有無と項目の確認
# testされるかどうか（依存まで見に行く）

require 'pathname'
require 'json'
require_relative './lib/collector/collector.rb'
require_relative './lib/code/codeparser.rb'

def relative_path(path, base)
  Pathname.new(File.absolute_path(path)).relative_path_from(File.dirname(File.absolute_path(base))).to_s
end

@codelib_path = __dir__ + '/..'
@codelib_path = File.absolute_path(@codelib_path)

src_paths_path = Collector.src_paths(@codelib_path).map { |e| relative_path(e, @codelib_path + '/src')}

coverages = {}
src_paths_path.each do |path|
  coverages[path] = {
    tested_by: []
  }
end

Collector.test_paths(@codelib_path).each do |test_path|
  nom_test_path = relative_path(test_path, @codelib_path + '/src')
  dfs = lambda do |path|
    code = open(path, 'r') { |io| Code.read_code(io, Collector.lang_from_path(path))}
    code[:dependencies].each do |dep_path|
      nom_dep_path = relative_path(dep_path, @codelib_path + '/src')
      coverages[nom_dep_path][:tested_by] << nom_test_path if coverages[nom_dep_path]
      dfs.call(nom_dep_path)
    end
  end
  dfs.call(nom_test_path)
end

puts JSON.pretty_generate(coverages)
