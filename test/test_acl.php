<?php
use swoole\zookeeper;

go(function () {
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper("127.0.0.1:2181", 2.5);
    $a = $zk->setAcl("/test34",[
        [
            "perms"=>zookeeper::PERM_READ,
            "id"=>[
                "id"=>"anyone",
                "scheme"=>"world"
            ]
        ],
        [
            "perms"=>zookeeper::PERM_READ,
            "id"=>[
                "id"=>"anyone",
                "scheme"=>"world"
            ]
        ]
    ]);
    var_dump($a);
    var_dump($zk->errCode);
    var_dump($zk->getAcl("/test34"));
});