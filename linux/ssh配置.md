### ssh配置文件的路径
* ~/.ssh/config

### 如何开启连接复用
* 效果：第二次登录同一台机器不需要输入密码
```shell
Host *
    ControlMaster auto
    ControlPath /tmp/ssh_connection_%h_%p_%r.sock
```

### 一份简单的ssh配置示例
```shell
Host *
    ServerAliveInterval 80
    ServerAliveCountMax 6
    ForwardAgent yes
    CheckHostIP no
    Compression yes

    ControlMaster auto
    ControlPath /tmp/ssh_connection_%h_%p_%r.sock
    User weirong.cwr

Host dev
    Hostname 10.10.10.10
    User weirong.cwr

Host login
    Hostname login1.mywork.org
    User weirong.cwr
```
* 有几个需要解释一下的点
    * ServerAliveInterval 80 表示每隔80秒给ssh server发一次心跳，保持链接，不被踢下来
    * ServerAliveCountMax 6 尝试6次心跳失败就不再重试
    * ForwardAgent yes ssh证书登录做代理转发的，配置FowardAgent后，进需要在自己的笔记本上保存私钥（比如id_rsa/id_dsa文件），其他任何机器都不需要放私钥
    * User 配置项是为了防止每次都写用户名，对于本机用户和服务器用户不一致的情况非常适用
    * Host dev 配置项适用于开发机ip记不住，配置了就有别名了

### 在服务器上进行连接复用（tmux）
* tmux在正常服务器上仅仅需要安装就行，但是在跳板机上因为普通用户没有权限，所以需要手动从源码安装编译
* tmux编译的唯一依赖就是libevent，所以在编译tmux之前需要先编译libevent(libevent可以在sourceforge上获取，tmux的包可以在github上获取。由于版本之间可能会出一些依赖，这两个包放在本工程的repo下面）
```shell
#常规套路编译
./configure --prefix=/home/weirong.cwr/libevent
make
make install
```
* 关于怎么编译这些屎，configire -h 总是能给出当前命令的帮助文档。
```shell
./configure --prefix=/home/weirong.cwr/tmux2 --enable-static CFLAGS=-I/home/weirong.cwr/libevent/include LDFLAGS=-L/home/weirong.cwr/libevent/lib
make && make install
#如果configure不成功就使用下面的那个
#./configure --prefix=/home/weirong.cwr/tmux2 --enable-static LIBEVENT_CFLAGS=-I/home/weirong.cwr/libevent/include LIBEVENT_LIBS=-L/home/weirong.cwr/libevent/lib
```
* 安装过程中一些典型报错的解决
    * configure: error: "curses not found" ---> yum install ncurses-devel
* 当然了，如果有同版本的os编译出来的二进制我才懒得去编译。
* tmux配置文件目录~/.tmux.conf
```shell
#目前在用的配置
# Statusbar properties.
set -g display-time 3000
set -g status-right "%m/%d %a %H:%M"
set -g status-bg default
set -g status-fg yellow
set -g set-titles on
set -g bell-action none
set -g history-limit 5000
set -g base-index 1
setw -g automatic-rename off
set -g status-utf8 on
setw -g utf8 on
set-window-option -g window-status-current-attr bright,reverse
set-window-option -g window-status-current-bg black
set-window-option -g window-status-current-fg green
set-window-option -g automatic-rename off
set-window-option -g utf8 on

# Bind function keys.
bind -n F2 prev
bind -n F3 next
bind-key -n C-left prev
bind-key -n C-right next
## switch pane
bind-key -n M-left select-pane -L
bind-key -n M-right select-pane -R
bind-key -n M-up select-pane -U
bind-key -n M-down select-pane -D
bind-key -n C-\ split-window
bind-key -n M-\ split-window -h

bind-key -n C-F12 lock

set -g history-limit 10000
set -g terminal-overrides 'xterm*:smcup@:rmcup@'
setw -g mode-keys vi

#Pane之间切换的快捷键
bind h select-pane -L
bind j select-pane -D
bind k select-pane -U
bind l select-pane -R

#Pane大小调整快捷键
bind < resize-pane -L 5
bind > resize-pane -R 5
bind _ resize-pane -D 5
bind + resize-pane -U 5

#窗口切分快捷键
bind \ split-window -h
bind - split-window -v
#set -g prefix C-x
### KEY map
##F1: prev window
#F2: next window
# Ctrl+\: split window V
# Alt+\: split window H
# Ctrl+F12: lock session
# Ctrl+Z+Left: resize pane left
# Ctrl+Z+Right: resize pane right
# Ctrl+Z+Up: resize pane up
# Ctrl+Z+Down: resize pane down
# Ctrl+Z then Left: switch to left pane
# Ctrl+Z then Right: switch to right pane
# Ctrl+Z then Up: switch to up pane
# Ctrl+Z then Down: switch to down pane
# Ctrl+Z then x: kill current pane or window
```
