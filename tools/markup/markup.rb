
module Markup
  def self.parse(str)
    str.gsub('<', '&lt;')
  end
end
