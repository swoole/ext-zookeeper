<?php
use swoole\zookeeper;

go(function () {
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper("127.0.0.1:2181", 2.5);


    $path = "/test2345";
    $zk->create($path, 'rango');

    $re = $zk->addAuth("digest","zookeeper:zookeeper");
    var_dump($re);

    $r = $zk->get($path);
    var_dump($r);


    $a = $zk->setAcl($path,[
        [
            "perms"=>zookeeper::PERM_READ,
            "id"=>[
                "id"=>"zookeeper:zookeeper",
                "scheme"=>"auth"
            ]
        ]
    ]);
    var_dump($a);
            var_dump($zk->errCode);

    //var_dump($zk->getAcl("/test34"));
});