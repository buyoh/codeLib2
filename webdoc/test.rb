
require 'sqlite3'

`rm codes.db` if File.exist?('codes.db')

sqldb = SQLite3::Database.new('codes.db')

sqldb.execute(<<-ENDSQL)
create table articles (
  id integer primary key autoincrement,
  path text unique,
  title text,
  keyword text
);
ENDSQL
sqldb.execute(<<-ENDSQL)
create table descriptors (
  articleId integer,
  keyStr text,
  valueStr text
);
ENDSQL

p sqldb.execute("insert into articles (path, title, keyword) values('foo/bar','title1','keyword1')")

p sqldb.execute("select id from articles where path='foo/bar'")
p sqldb.execute("select id from articles where path='hoge/hige'")
