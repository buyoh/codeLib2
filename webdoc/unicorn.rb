@dir = "[Sinatraアプリが置いてあるフォルダへのパス]"
worker_processes 1
#working_directory "/my/nice/path" # 記載しておけば他のディレクトリでこのファイルを叩けなくなる
preload_app true
timeout 8
listen "./tmp/sockets.sock", :backlog => 64
pid "./tmp/pids/unicorn.pid"
stderr_path "./log/unicorn.stderr.log"
stdout_path "./log/unicorn.stdout.log"