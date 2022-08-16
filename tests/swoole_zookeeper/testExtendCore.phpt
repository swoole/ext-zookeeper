--TEST--
swoole_zookeeper: test new instance
--SKIPIF--
<?php
require __DIR__ . '/../inc/skipif.inc';
?>
--FILE--
<?php
declare(strict_types=1);

include_once __DIR__ . '/../inc/bootstrap.php';
use swoole\zookeeper;

//require __DIR__ . '/../inc/skipif.inc';
//require __DIR__ . '/../inc/bootstrap.php';
class ourZooClass extends zookeeper{

    private $status=1;

    const STOP = 0;

    const RUN = 1;

    public function __construct()
    {
        parent::__construct(TEST_ZOOKEEPER_FULL_URL, TEST_ZOOKEEPER_TIMEOUT);
    }

    public function run()
    {
        test(function(){
            try{
                if (!$this->exists(TEST_ZOOKEEPER_PERSISTENT_KEY))
                {
                    $this->create(TEST_ZOOKEEPER_PERSISTENT_KEY, "swoole");
                }
                $this->get(TEST_ZOOKEEPER_PERSISTENT_KEY);
            } catch(Exception $e){
                var_dump($this);
            }
        });
    }
}

zookeeper::setDebugLevel(1);
$class = new ourZooClass();
$class->run();
?>
--EXPECT--
