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

安装中可能遇到的问题

1. 安装完成后，执行 *php test/test.php*  报找不到 swoole\zookeeper 类

执行 *php -i | grep php.ini* 找到配置文件的位置，在 php.ini 中动态扩展配置区加入 **extension=swoole_zookeeper.so**