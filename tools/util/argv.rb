

class ArgParser
  def initialize
    @argtype = {}
  end
  def define_param(name)
    @argtype[name] = :prm
  end
  def define_flag(name)
    @argtype[name] = :flg
  end
  def parse(given_args)
    untagged = []
    res = {}
    hold = nil
    given_args.each do |a|
      if hold
        res[hold] = a
        hold = nil
      else
        case @argtype[a]
        when :flg
          res[a] = true
        when :prm
          hold = a
        else
           untagged << a
        end
      end
    end
    [res, untagged]
  end
end