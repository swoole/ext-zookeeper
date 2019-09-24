# phpx 安装过程

### 安装环境

	ubuntu18.04

### 1.下载安装包

	https://github.com/swoole/phpx

### 2.安装composer

```shell
#下载composer可执行文件
php -r "copy('https://getcomposer.org/installer', 'composer-setup.php');"
#执行安装文件
php composer-setup.php
#删除安装文件
php -r "unlink('composer-setup.php');"
#移动 composer.phar 可以进行全局调用
mv composer.phar /usr/local/bin/composer
```

### 3.注意

最好把 `phpize` 和 `php-config` 都丢到 `/usr/bin` 里防止找不到

### 4.运行phpx下的shell

首先进入到 `phpx` 的目录，执行 `shell`

```shell
./build.sh
```

cmake解析

```
cmake .
```

编译

```
make -j 4
```

安装

```
sudo make install
```

导入动态库

```
sudo ldconfig
```

将 `bin` 目录下的二进制文件 `copy` 到 `/usr/bin` 下面

```
sudo cp bin/phpx /usr/bin/phpx
```

运行 `phpx`

```shell
zhanglei@zhanglei-OptiPlex-9020:~/ourc/phpx-master$ phpx
	
	PHP-X Console Tool 0.0.1
	
	Usage:
	  command [options] [arguments]
	
	Options:
	  -h, --help            Display this help message
	  -q, --quiet           Do not output any message
	  -V, --version         Display this application version
	      --ansi            Force ANSI output
	      --no-ansi         Disable ANSI output
	  -n, --no-interaction  Do not ask any interactive question
	  -v|vv|vvv, --verbose  Increase the verbosity of messages: 1 for normal output, 2 for more verbose output and 3 for debug
	
	Available commands:
	  build    Build phpx project
	  clean    Clean object files
	  create   Create a phpx project
	  help     Displays help for a command
	  install  Install phpx project
	  list     Lists commands
	  run      Run phpx project
	zhanglei@zhanglei-OptiPlex-9020:~/ourc/phpx-master$ 
```