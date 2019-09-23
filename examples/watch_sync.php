<?php
// Zookeeper::setDebugLevel(Zookeeper::LOG_LEVEL_DEBUG);
$zookeeper = new Zookeeper('localhost:2181', function () {}, 5000);
$path = '/test';
$zookeeper->get($path, function () use ($path) {
    echo "{$path} has been updated" . PHP_EOL;
});

while (true) {
    zookeeper_dispatch();
}
