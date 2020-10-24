require 'pathname'

def relative_path(path, base)
  Pathname.new(File.absolute_path(path)).relative_path_from(File.dirname(File.absolute_path(base))).to_s
end
