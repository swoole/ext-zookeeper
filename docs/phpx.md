#phpx 安装过程

###安装环境

	ubuntu18.04
	
###1.下载安装包

	https://github.com/swoole/phpx
	
###2.安装composer


	wget https://pecl.php.net/get/phar-2.0.0.tgz
	
	mv composer.phar composer
	
	chmod +x composer
	
	sudo mv composer /usr/local/bin
	
###3.注意

最好把phpize 和 php-config都丢到/usr/bin里防止找不到
	
###4.运行phpx下的shell

首先进入到phpx的目录

执行shell

	./build.sh

cmake解析
	
	cmake .
	
编译

	make -j 4
	
安装
	
	sudo make install
	
导入动态库
	
	sudo ldconfig
	
将bin目录下的二进制文件copy入/usr/bin下面

	sudo cp bin/phpx /usr/bin/phpx
	
运行phpx

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


	