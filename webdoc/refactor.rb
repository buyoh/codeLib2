require './dbhelper/dbhelper'
require './dbhelper/collector'
require './dbhelper/dbsolver'

@inplace = $*.include?('--inplace')



sqldb = DBHelper.new


success = true

Dir.chdir('../') do

  sqldb.all_index_db.each do |item|
    path = item[:path]
    li = path.split('/')

    code = 0
    next unless li[0] == 'src'
    case li[1]
    when 'javascript'
      if @inplace
        code = system("eslint #{path} --fix")
      else
        code = system("eslint #{path}")
      end
    when 'cpp'
      if @inplace
        code = system("clang-format -i -style=Chromium #{path}")
      else
        diff = `bash -c "diff <( clang-format -style=Chromium #{path} ) #{path}"`
        unless diff=~/^\s*$/
          code = 1
          puts "clang-format assertion: #{path}"
          puts "diff..."
          puts diff
          puts ''
        end
      end
    end

    success &= code == 0
  end

end

abort "refactor check failed" unless success
exit 0