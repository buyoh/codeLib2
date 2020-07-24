require 'fileutils'
require 'sqlite3'

class DBWrapperSQLite3
  def initialize
    Dir.mkdir('tmp') unless Dir.exist?('tmp')
    FileUtils.touch('tmp/codes.db') unless File.exist?('tmp/codes.db')
    @sqldb = SQLite3::Database.new('tmp/codes.db')
  end

  def dump_table_info
    count_a = @sqldb.execute("select count(*) from sqlite_master where type='table' and name='articles';")[0][0]
    count_d = @sqldb.execute("select count(*) from sqlite_master where type='table' and name='descriptors';")[0][0]
    count_g = @sqldb.execute("select count(*) from sqlite_master where type='table' and name='gitcommits';")[0][0]

    if count_a > 0
      puts 'table articles exists.'
      puts 'table_info:'
      @sqldb.execute('pragma table_info(articles);').sort.each do |item|
        p item
      end
    end
    if count_d > 0
      puts 'table descriptors exists.'
      puts 'table_info:'
      @sqldb.execute('pragma table_info(descriptors);').sort.each do |item|
        p item
      end
    end
    if count_g > 0
      puts 'table gitcommits exists.'
      puts 'table_info:'
      @sqldb.execute('pragma table_info(gitcommits);').sort.each do |item|
        p item
      end
    end
  end

  def create_db # reform_db
    count_a = @sqldb.execute("select count(*) from sqlite_master where type='table' and name='articles';")[0][0]
    count_d = @sqldb.execute("select count(*) from sqlite_master where type='table' and name='descriptors';")[0][0]
    count_g = @sqldb.execute("select count(*) from sqlite_master where type='table' and name='gitcommits';")[0][0]

    if count_a > 0 && @sqldb.execute('pragma table_info(articles);').sort != [\
      [0, 'id', 'integer', 0, nil, 1], \
      [1, 'path', 'text', 0, nil, 0], \
      [2, 'title', 'text', 0, nil, 0], \
      [3, 'keyword', 'text', 0, nil, 0], \
      [4, 'commitId', 'integer', 0, nil, 0]
    ]
      @sqldb.execute('drop table articles')
      count_a = 0
    end
    if count_d > 0 && @sqldb.execute('pragma table_info(descriptors);').sort != [\
      [0, 'articleId', 'integer', 0, nil, 0], \
      [1, 'keyStr', 'text', 0, nil, 0], \
      [2, 'valueStr', 'text', 0, nil, 0]
    ]
      @sqldb.execute('drop table descriptors')
      count_d = 0
    end
    if count_g > 0 && @sqldb.execute('pragma table_info(gitcommits);').sort != [\
      [0, 'id', 'integer', 0, nil, 1], \
      [1, 'sha', 'text', 0, nil, 0], \
      [2, 'date', 'integer', 0, nil, 0], \
      [3, 'message', 'text', 0, nil, 0]
    ]
      @sqldb.execute('drop table gitcommits')
      count_d = 0
    end

    @sqldb.execute(<<-ENDSQL) if count_a == 0
      create table articles (
        id integer primary key autoincrement,
        path text unique,
        title text,
        keyword text,
        commitId integer
      );
    ENDSQL
    @sqldb.execute(<<-ENDSQL) if count_d == 0
    create table descriptors (
      articleId integer,
      keyStr text,
      valueStr text
    );
    ENDSQL
    @sqldb.execute(<<-ENDSQL) if count_g == 0 # dateと言いながら中身はTimeの整数値だぞ！気をつけろ！
    create table gitcommits (
      id integer primary key autoincrement,
      sha text unique,
      date integer,
      message text
    );
    ENDSQL
  end

  # 検索に一致した {id, path, title, words} の配列を返す
  # memo: %が*，_が?に該当．部分判定は '%query%'
  def search_articles(path, title, keyword, union = 'or')
    where_query = []
    where_value = []
    if path;    where_query << 'path like ?';    where_value << path;    end
    if title;   where_query << 'title like ?';   where_value << title;   end
    if keyword; where_query << 'keyword like ?'; where_value << keyword; end
    return [] if where_value.empty?

    @sqldb.execute("select id, path, title, keyword, commitId from articles where #{where_query * " #{union} "};", where_value)
  end

  def all_articles
    @sqldb.execute('select id, path, title, keyword, commitId from articles;')
  end

  def all_articles_joined_commit
    @sqldb.execute('select articles.id, articles.path, articles.title, articles.keyword, gitcommits.date, gitcommits.sha from articles left outer join gitcommits on articles.commitId = gitcommits.id;')
  end

  def get_article(id)
    @sqldb.execute('select path, title, keyword, commitId from articles where id=?;', id)[0]
  end

  def get_article_path(path)
    @sqldb.execute('select id, path, title, keyword, commitId from articles where path like ?;', '%' + path)[0]
  end

  def getid_article(path)
    @sqldb.execute('select id from articles where path=?;', path)[0][0]
  end

  def insert_article(path, title, keyword, commitId)
    @sqldb.execute('insert into articles(path, title, keyword, commitId) values(?,?,?,?);', [path, title, keyword, commitId])
  end

  def update_article(id, path, title, keyword, commitId)
    @sqldb.execute('update articles set path=?, title=?, keyword=?, commitId=? where id=?;', [path, title, keyword, commitId, id])
  end

  def delete_article(id)
    @sqldb.execute('delete from articles where id=?', id)
  end

  def select_descriptors(article_id)
    @sqldb.execute('select keyStr,valueStr from descriptors where articleId=?;', article_id)
  end

  def get_descriptor_value(article_id, key)
    @sqldb.execute('select keyStr from descriptors where articleId=? and keyStr=?;', article_id, key)[0]
  end

  def insert_descriptor(article_id, key, val)
    @sqldb.execute('insert into descriptors(articleId, keyStr, valueStr) values(?,?,?);', [article_id, key, val])
  end

  def update_descriptor(article_id, key, val)
    @sqldb.execute('update descriptors set valueStr=? where articleId=? and keyStr=?;', [val, article_id, key])
  end

  def delete_descriptors_by_article_id(article_id)
    @sqldb.execute('delete from descriptors where articleId=?', article_id)
  end

  def all_gitcommits
    @sqldb.execute('select id, sha, date, message from gitcommits;')
  end

  def get_gitcommit(id)
    @sqldb.execute('select id, sha, date, message from gitcommits where id=?;', id)[0]
  end

  def get_gitcommit_sha(sha)
    @sqldb.execute('select id, sha, date, message from gitcommits where sha=?;', sha)[0]
  end

  def insert_gitcommit(sha, date, message)
    @sqldb.execute('insert into gitcommits(sha, date, message) values(?,?,?);', [sha, date, message])
  end

  # TODO: impl me
  # def delete_unnecessary_gitcommit
  # end
end
