#!/usr/bin/env php
<?php

$root = dirname(__DIR__);

function getResource($name, $url)
{
    global $root;
    echo "{$root}/data/{$name}", '_____';
    if (!file_exists("{$root}/data/{$name}")) {
        echo 'fetch:', $name, PHP_EOL;
        file_put_contents("{$root}/data/{$name}", file_get_contents($url));
    } else {
        echo 'ign:', $name, PHP_EOL;
    }
}

$data = [
    [
        "name" => "swoole-src",
        "version" => "4.3.3",
        "cmd" => "phpize && ./configure && make && sudo make install",
        "url" => []
    ],
    [
        "name" => 'phpx',
        "version" => "0.1.1",
        "cmd" => "cmake . && sudo make install",
        "url" => [
            "phpx.php" => "https://github.com/swoole/phpx/releases/download/v0.1.1/phpx",
        ],
    ],
];

foreach ($data as $value) {
    foreach($value['url'] as $name => $url) {
        getResource($name, $url);
    }
    getResource("{$value['name']}.zip", "https://github.com/swoole/{$value['name']}/archive/v{$value['version']}.zip");
    echo `unzip {$root}/data/{$value['name']}.zip -d {$root}/data/{$value['name']} && cd {$root}/data/{$value['name']}/{$value['name']}-{$value['version']} && {$value['cmd']}`;
}