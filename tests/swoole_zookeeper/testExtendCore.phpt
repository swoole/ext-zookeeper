<?php

use swoole\zookeeper;

//require __DIR__ . '/../inc/skipif.inc';
//require __DIR__ . '/../inc/bootstrap.php';
class ourZooClass extends zookeeper{

    public function __construct()
    {
//        parent::__construct();
    }

    public function test()
    {
        $this->get(TEST_ZOOKEEPER_PERSISTENT_KEY);
    }
}

$class = new ourZooClass();
try{
$class->test();
}catch(Exception $e){
var_dump($class);
}
