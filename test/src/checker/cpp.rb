
module Tester
  def self.cpp(importcode, targetcode, testcode)
    basecode = IO.read("./test/src/base/base.cpp")
    code = ""
    l, _, basecode = basecode.partition("//#import")
    code << l << importcode
    l, _, basecode = basecode.partition("//#target")
    code << l << targetcode
    l, _, basecode = basecode.partition("//#test")
    code << l << testcode << basecode
    Dir.chdir(@tempdir) do
      IO.write("test.cpp", code)
      next false unless system("g++ -std=c++14 -lm -o test.out test.cpp")
      next false unless system("./test.out")
      true
    end
  end
end