module Test
  class CPP
    CC = 'g++'
    OPTIONS = '-I ./ -lm -Wall -Werror -Wno-error=comment -Wno-comment'

    def initialize(testpath, tempdir)
      @testpath = testpath
      @tempdir = tempdir
    end

    def compile
      system "#{CC} #{OPTIONS} -o #{@tempdir}/a.out #{@testpath} -std=c++17"
    end

    def test_compilable(easy = false)
      return {'default' => false} unless system("#{CC} #{OPTIONS} -fsyntax-only #{@testpath}")

      opt = ['-O2', '-O3']
      std = ['c++11', 'c++14', 'c++17']
      result = {'default' => true}
      return result if easy
      opt.each do |o|
        std.each do |s|
          args = "#{o} #{s}"
          result[args] = system("#{CC} #{OPTIONS} -Wno-unused-variable -Wno-comment -fsyntax-only #{@testpath} #{o} -std=#{s}")
        end
      end
      result
    end
    

    def execute(nochdir: false)
      res = nil
      if nochdir
        p "#{@tempdir}/a.out"
        res = system "#{@tempdir}/a.out"
        p $?
      else
        Dir.chdir(@tempdir) do
          res = system "./a.out"
        end
      end
      res
    end
  end
end
