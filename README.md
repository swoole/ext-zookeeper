# ext-zookeeper
依赖 `Swoole`和`PHP-X`。

安装 Swoole
----
```shell
pecl install swoole
```

安装 PHP-X
-----
参考 [PHP-X 安装](https://github.com/swoole/phpx)

Build
-----
```shell
phpx build -v -d
phpx install
```

安装过程中可能遇到的问题
-----

1. 安装完成后，执行 *php test/test.php*  报找不到 swoole\zookeeper 类

	执行 *php -i | grep php.ini* 找到配置文件的位置，在 php.ini 中动态扩展配置区加入 **extension=swoole_zookeeper.so**

2. 安装完成后，执行 *php test/test.php*  报找不到动态库 swoole_zookeeper.so
	
	目前尚未找到原因

3. 出现这种错误 Unable to load dynamic library '/usr/lib64/php/modules/cpp_ext.so' - libphpx.so: cannot open shared object file: No such file or directory in Unknown on line 0

	说明 PHP-X 没有安装成功，请重新安装 PHP-X

4. 执行 *phpx build -v -d*，出现 fatal error: swoole/include/sock_hook.h No such file or directory
	
	请重新安装编译安装 swoole