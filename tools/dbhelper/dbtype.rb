# frozen_string_literal: true

# TODO: replace...

# rdoc
# Article {id: Integer, path: String, title: String, words: String, commit_id: Integer}
Article = Struct.new('Article', :id, :path, :title, :words, :commit_id, keyword_init: true)

# rdoc
# Commit {id: Integer, sha: String, date: Date}
Commit = Struct.new('Commit', :id, :sha, :date, :message, keyword_init: true)

# rdoc
# Descriptor {key: String, value: String}
Descriptor = Struct.new('Descriptor', :key, :value, keyword_init: true)

# rdoc
# Descriptors {article_id: Integer, values: Array<Descriptor>}
Descriptor s = Struct.new('Descriptors', :article_id, :values, keyword_init: true)

# unnecessary... :(

# def create_struct(props_name)
#   props = props_name.clone
#   Class.new() do
#     define_method(:initialize) do |**args|
#       props.each do |pr|
#         instance_variable_set '@' + pr, args[pr.to_sym]
#       end
#     end
#     attr_accessor *props
#   end
# end
# Descriptor = create_struct(['key', 'value'])
