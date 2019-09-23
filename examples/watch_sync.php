<?php

Zookeeper::setDebugLevel(Zookeeper::LOG_LEVEL_DEBUG);
$zookeeper = new Zookeeper('localhost:2181', function() {
});
$path = '/timandes';
$zookeeper->get($path, function() use($path) {
	echo "{$path} has been updated" . PHP_EOL;
});

while (true) {
	zookeeper_dispatch();
}

