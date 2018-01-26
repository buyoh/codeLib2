
require 'json'

unless File.directory? ("src")
    puts "current directory is not root"
    abort
end

# -------------------

def fileload(path)
    dic = {}
    tag = :u
    iscode = false
    open(path, 'r') do |io|
        while line = io.gets
            line.force_encoding "UTF-8"
            if !iscode && (line=~/^(?:#|\/\/)\s*/)
                line = line[$~.end(0)..-1]
                if line.size >= 1 && line[0] == '%'
                    case line.scan(/^\%\S+/)[0]
                    when '%title'
                        tag = :title
                        next
                    when '%overview'
                        tag = :overview
                        next
                    when '%usage'
                        tag = :usage
                        next
                    when '%verified'
                        tag = :verified
                        next
                    when '%references'
                        tag = :references
                        next
                    end
                end
                dic[tag] ||= ""
                dic[tag] << line
            else
                iscode = true
                dic[:code] ||= ""
                dic[:code] << line
            end
        end
    end
    dic
end

# -------------------

files = []

dfs = lambda do |path|
    if File.directory?(path)
        Dir.foreach(path) do |f|
            dfs.call "#{path}/#{f}" if f[0] != "."
        end
    else
        files <<  path
    end
end

Dir.foreach("src") do |lang|
    next if lang[0] == "."
    dfs.call "src/#{lang}"
end

# -------------------

# require 'pp'

dic = []
files.each do |file|
    d = fileload(file)
    dic << d if d.key?(:title)
end

puts JSON.generate(dic)
