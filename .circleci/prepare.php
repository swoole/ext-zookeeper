#!/usr/bin/env php
<?php

function d($name, $url)
{
    $root = dirname(__DIR__);
    echo "{$root}/data/{$name}", '_____';
    if (!file_exists("{$root}/data/{$name}")) {
        echo 'fetch:', $name, PHP_EOL;
        file_get_contents($url, "{$root}/data/{$name}");
    } else {
        echo 'ign:', $name, PHP_EOL;
    }
}


$data = [
    'phpx' => 'https://github.com/swoole/phpx/releases/download/v0.1.1/phpx',
    'swoole.zip' => 'https://github.com/swoole/swoole-src/archive/v4.3.3.zip',
    'phpx.zip' => 'https://github.com/swoole/phpx/archive/v0.1.1.zip',
];

foreach($data as $name => $url) {
    d($name, $url);
}


$root = dirname(__DIR__);

echo "handle phpx-src", PHP_EOL;
echo `unzip {$root}/data/phpx.zip -d {$root}/data/phpx-src && cd {$root}/data/phpx-src/phpx-0.1.1 && cmake . && sudo make install`;

echo "handle swoole-src",  PHP_EOL;
echo `unzip {$root}/data/swoole.zip -d {$root}/data/swoole-src && cd {$root}/data/swoole-src/swoole-src-4.3.3 && phpize && ./configure && make && sudo make install`;
