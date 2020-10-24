#!/usr/bin/env ruby

# docの有無と項目の確認
# testされるかどうか（依存まで見に行く）

require 'pathname'
require 'json'
require_relative './lib/collector/collection.rb'
require_relative './lib/collector/collector.rb'
require_relative './lib/code/codeparser.rb'

def relative_path(path, base)
  Pathname.new(File.absolute_path(path)).relative_path_from(File.dirname(File.absolute_path(base))).to_s
end

@codelib_path = __dir__ + '/..'
@codelib_path = File.absolute_path(@codelib_path)

collection = Collection.new(@codelib_path)

puts JSON.pretty_generate(collection.src_relations)
