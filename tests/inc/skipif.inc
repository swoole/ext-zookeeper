<?php

require __DIR__ . '/bootstrap.php';

define('SWOOLE_COLOR_RED', 1);
define('SWOOLE_COLOR_GREEN', 2);
define('SWOOLE_COLOR_YELLOW', 3);
define('SWOOLE_COLOR_BLUE', 4);
define('SWOOLE_COLOR_MAGENTA', 5);
define('SWOOLE_COLOR_CYAN', 6);
define('SWOOLE_COLOR_WHITE', 7);

function swoole_color(string $content, int $color): string
{
    return "\033[3{$color}m{$content}\033[0m";
}

function skip(string $reason, bool $is_skip = true, int $color = SWOOLE_COLOR_YELLOW)
{
    if ($is_skip) {
        exit('skip ' . swoole_color($reason, $color));
    }
}

// TODO 这个地方判断其实有点问题， 因为 zookeeper 库会从开始就输出日志
function check_can_connect_zookeeper_server()
{
    static $zookeeperStarted = false;
    
    go(function () use (&$zookeeperStarted) {
        if ($zookeeperStarted === false) {
            \swoole\zookeeper::setDebugLevel(1);
            $zk = new \swoole\zookeeper(TEST_ZOOKEEPER_FULL_URL, TEST_ZOOKEEPER_TIMEOUT);
            $fp = fopen("/tmp/check_zookeeper.log", "wb+");
            $zk->setLogStream($fp);
            $ret = $zk->create('/check_zookeeper_server_is_started', 1, ZOO_EPHEMERAL);
            if ($ret === '/check_zookeeper_server_is_started') {
                $zookeeperStarted = true;
            }
        }
    });
    swoole_event_wait();

    return $zookeeperStarted;
}


skip('skip swoole extension is required', !extension_loaded("swoole"));

skip('skip swoole_zookeeper extension is required', !extension_loaded("swoole_zookeeper"));

skip('skip zookeeper server is required', !check_can_connect_zookeeper_server());

