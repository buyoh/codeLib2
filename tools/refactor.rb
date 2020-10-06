#!/usr/bin/env ruby

require_relative './lib/collector/collector'
require_relative './lib/code/codeparser'

@inplace = ARGV.include?('--inplace')

success = true

Dir.chdir(__dir__ + '/../') do
  Collector.src_files.each do |path|
    info = Code.fileload(path)
    lang = Collector.lang_from_path(path)

    tags = (info[:tags] || '').split(',').map(&:strip)

    next if tags.include?('nolint')

    code = 0
    case lang
    when 'javascript'
      code = if @inplace
               system("eslint #{path} --fix")
             else
               system("eslint #{path}")
             end
    when 'cpp'
      if @inplace
        code = system("clang-format -i -style=file #{path}")
      else
        diff = `bash -c "diff <( clang-format -style=file #{path} ) #{path}"`
        unless diff =~ /^\s*$/
          code = 1
          puts "clang-format assertion: #{path}"
          puts 'diff...'
          puts diff
          puts ''
        end
      end
    end
    puts "failed: #{path}" unless code
    success &= code
  end
end

abort 'refactor check failed' unless success
exit 0
