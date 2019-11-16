module Test
  class CPP
    CC = 'g++'
    OPTIONS = '-I ./ -lm -Wall'

    def initialize(testpath, tempdir)
      @testpath = testpath
      @tempdir = tempdir
    end

    def compile
      system "#{CC} #{OPTIONS} -o #{@tempdir}/a.out #{@testpath} -std=c++17"
    end

    def test_compilable
      return {'default' => false} unless system("#{CC} #{OPTIONS} -c -o #{@tempdir}/a.o #{@testpath}")

      opt = ['-O2', '-O3']
      std = ['c++11', 'c++14', 'c++17']
      result = {'default' => true}
      opt.each do |o|
        std.each do |s|
          args = "#{o} #{s}"
          result[args] = system("#{CC} #{OPTIONS} -Wno-unused-variable -Wno-comment -c -o #{@tempdir}/a.o #{@testpath} #{o} -std=#{s}")
        end
      end
      result
    end

    def execute
      res = nil
      Dir.chdir(@tempdir) do
        res = system "./a.out"
      end
      res
    end
  end
end
