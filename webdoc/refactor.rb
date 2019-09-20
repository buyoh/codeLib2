Dir.chdir __dir__
require './dbhelper/collector'
require './code/codeparser'

@inplace = $*.include?('--inplace')


success = true

Dir.chdir('../') do

  Document.src_files.each do |path|
    info = Code.fileload(path)
    li = path.split('/')

    tags = (info[:tags] || '').split(',').map(&:strip)

    next if tags.include?('nolint')

    code = 0
    case li[1]
    when 'javascript'
      if @inplace
        code = system("eslint #{path} --fix")
      else
        code = system("eslint #{path}")
      end
    when 'cpp'
      if @inplace
        code = system("clang-format -i -style=file #{path}")
      else
        diff = `bash -c "diff <( clang-format -style=file #{path} ) #{path}"`
        unless diff=~/^\s*$/
          code = 1
          puts "clang-format assertion: #{path}"
          puts "diff..."
          puts diff
          puts ''
        end
      end
    end
    puts "failed: #{path}" unless code
    success &= code
  end

end

abort "refactor check failed" unless success
exit 0
