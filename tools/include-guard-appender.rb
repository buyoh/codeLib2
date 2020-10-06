#!/usr/bin/env ruby

require_relative './lib/collector/collector'

# easy implementation

# @tempdir = '/tmp/codelib2'
Dir.chdir __dir__
def do_task(path)
  identifier = path.gsub(/\W/, '_').upcase + '__'
  puts path

  builder = ''
  need_update = false
  check_satisfied_guard = false
  checked_doc_begin = 0
  checked_doc_end = 0
  check_satisfied_guard_terminal = false

  io = open(path, 'r')
  while raw_line = io.gets
    line = raw_line.chomp
    if line.start_with? "#ifndef #{identifier}"
      check_satisfied_guard = true
    elsif line.downcase.start_with? '// %=begin'
      unless check_satisfied_guard
        need_update = true
        builder << "#ifndef #{identifier}\n"
        builder << "#define #{identifier}\n"
      end
      checked_doc_begin += 1
    elsif line.downcase.start_with? '// %=end'
      checked_doc_end += 1
    elsif line.start_with? "#endif  // #{identifier}"
      check_satisfied_guard_terminal = true
    end
    builder << raw_line.chomp << "\n"
  end
  io.close

  if !check_satisfied_guard && check_satisfied_guard_terminal
    puts 'ERROR: not found IFNDEF, but found ENDIF'
    return [false, :fail]
  end

  if checked_doc_begin != checked_doc_end
    puts 'ERROR: wrong block'
    return [false, :fail]
  end

  if checked_doc_begin > 0 && checked_doc_end > 0
    puts 'SKIP: nodoc'
    return [true, :skip]
  end

  unless check_satisfied_guard_terminal
    need_update = true
    builder << "#endif  // #{identifier}\n"
  end

  if need_update
    puts 'OK: updated'
    IO.write(path, builder)
    return [true, :updated]
  end

  puts 'OK: no update'
  [true, :ok]
end

counter = Hash.new(0)
failed = false
Dir.chdir('../') do
  # Dir.mkdir @tempdir unless Dir.exist? @tempdir

  Collector.src_files.each do |path|
    next if @filter && path !~ @filter

    lang = Collector.lang_from_path(path)
    next if lang != 'cpp'

    ok, s = do_task(path)
    failed |= !ok
    counter[s] += 1
    counter[:total] += 1
  end
end

p counter

abort if failed
exit 0
