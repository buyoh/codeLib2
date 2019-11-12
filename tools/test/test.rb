module Test
  class CPP
    def initialize(testpath, tempdir)
      @testpath = testpath
      @tempdir = tempdir
    end
    def compile # TODO: rename build
      system "g++ -I ./ -o #{@tempdir}/a.out #{@testpath} -std=c++17 -lm"
    end
    def check_compilable
      opt = ['', '-O3']
      std = ['c++11', 'c++14', 'c++17']
      {'c++17' => true}
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
