module DBSolver
  # requrested_pathsが参照するrequireを全て集めたdocと依存順を返す．
  # requrested_paths: パスの配列
  # { path_sequence: path_sequence, docs: docs }
  # path_sequence: 依存順．この配列の順にコードを連結すると，正しく動作する
  def self.solve_paths(sqldb, requested_paths)
    docs = {}
    gathered_paths = [] #
    n_requiring = {} # key: path, value: 参照しているdocのpathの配列
    required = {} # key: path, value: 参照されている個数

    travel = lambda do |path|
      warn path
      next if n_requiring[path]

      doc = sqldb.find_db_by_path(path)
      next unless doc

      docs[path] = doc
      n_requiring[path] = 0

      next unless doc[:require]

      doc[:require].split.each do |pa_raw|
        pa = pa_raw.strip
        n_requiring[path] += 1
        required[pa] ||= []
        required[pa] << path
        travel.call(pa)
      end
    end

    requested_paths.each do |path|
      travel.call(path)
    end

    path_sequence = []
    queue = n_requiring.select { |_k, v| v == 0 }.keys
    warn n_requiring.inspect

    until queue.empty?
      top = queue.shift
      path_sequence << top
      next unless required[top]

      required[top].each do |re|
        n_requiring[re] -= 1
        queue << re if n_requiring[re] == 0
      end
    end

    { path_sequence: path_sequence, docs: docs }
  end

  # solve_pathsの結果をこの関数に渡すと，コードを依存順にjoinしたものを返す．
  def self.generate_merged_code(path_sequence, docs)
    path_sequence.map { |pa| docs[pa][:code] } * "\n"
  end

  # idに対応するpathを返す．
  def self.id_to_path(sqldb, id)
    doc = sqldb.find_db_by_index(id)
    doc ? doc[:path] : nil
  end
end
