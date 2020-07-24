# frozen_string_literal: true

module Markup
  def self.parse(str)
    str.gsub('<', '&lt;')
  end
end
