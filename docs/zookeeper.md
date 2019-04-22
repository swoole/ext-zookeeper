
# zookeeper安装过程

###安装环境

	ubuntu18.04

###1.下载代码包

	https://www.apache.org/dyn/closer.cgi/zookeeper/
	
###2.解压安装包并且到指定目录

	tar zxvf zookeeper-3.4.14.tar.gz 
	
	mv zookeeper-3.4.14 /home/zhanglei/soft/
	
###2.安装参考

	http://zookeeper.apache.org/doc/current/zookeeperAdmin.html#sc_maintenance
	
###3.安装java的jdk包

	sudo apt-get install openjdk-8-jdk
	
###4.查看文档 

Single Server and Developer Setup 部分：

	http://zookeeper.apache.org/doc/current/zookeeperStarted.html
	
###5.修改配置文件:

	 cp /home/zhanglei/soft/zookeeper-3.4.14/bin/../conf/zoo_sample.cfg  /home/zhanglei/soft/zookeeper-3.4.14/bin/../conf/zoo.cfg
	 
 修改dataDir
 
 	vim  /home/zhanglei/soft/zookeeper-3.4.14/bin/../conf/zoo.cfg
 	
 	修改为:
 	dataDir=/home/zhanglei/data/zookeeper

###6.启动zookeeper(切换到bin下)

	./zkServer.sh start

###7.查看zookeeper状态(切换到bin下)

	./zkServer.sh status
	
	
出现：

	ZooKeeper JMX enabled by default
	Using config: /home/zhanglei/soft/zookeeper-3.4.14/bin/../conf/zoo.cfg
	Mode: standalone
	
安装成功

使用

	lsof -i:2181

确认打开端口


	