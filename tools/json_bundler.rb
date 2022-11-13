#!/usr/bin/env ruby

require 'optparse'

def validate_path(path)
  path = path[0..-2] if path[-1] == '/'
  File.absolute_path(path)
end

@output_path = '/null'
@minimize = false

opt = OptionParser.new
opt.on('--codelib-path path/to/codelib') { |v| } # TODO: REMOVE
opt.on('--output-path public/data') { |v| @output_path = v }
opt.on('--minimize') { |_v| @minimize = true }
opt.parse!(ARGV)

@output_path = validate_path(@output_path)

unless Dir.exist?(@output_path)
  warn '--output-path does not exist'
  exit 1
end

# TODO: REFACTORING
@codelib_path = validate_path(__dir__ + '/../')

require 'json'
require 'pathname'
require_relative './lib/collector/collection.rb'
require_relative './lib/collector/collector.rb'
require_relative './lib/collector/gitlog.rb'

collection = Collection.new(@codelib_path)
output_docs = collection.src_docs.map do |article|
  path = article[:path]
  absolte_path = File.absolute_path(@codelib_path + '/' + article[:path])
  relation = collection.src_relations[path]
  {
    title: article[:title],
    overview: article[:overview],
    code: article[:code],
    lang: Collector.lang_from_path(article[:path]),
    path: '/' + article[:path],
    require: article[:require] || '',
    references: (article[:references] || '').split("\n"),
    words: (article[:words] || '').split(','),
    verified: (article[:verified] || '').split("\n"),
    commits: GitLog.history(absolte_path, { n: 99 }, @codelib_path),
    tested_by: relation ? relation[:tested_by] : []
  }
end

# TODO: top page contents e.g. coverage
# summary = {}

if @minimize
  IO.write @output_path + '/codelib_full.json', JSON.generate(output_docs)
else
  IO.write @output_path + '/codelib_full.json', JSON.pretty_generate(output_docs)
end
