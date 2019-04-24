# PHP Unit-test

Run these tests to make certain that the swoole extension you installed can work well.

## Preparation

依赖 `zookeeper server`, 默认:

- TEST_ZOOKEEPER_HOST: 127.0.0.1
- TEST_ZOOKEEPER_PORT: 2181
- TEST_ZOOKEEPER_TIMEOUT: 1

## How to run

just run `./start.sh`

## Defaults

| Config    | Enable   |
| --------- | -------- |
| show-diff | yes      |
| show-mem  | yes      |
| show-slow | 1000(ms) |

## Log files

| suffix | intro                                         |
| ------ | --------------------------------------------- |
| diff   | show the differents between output and expect |
| out    | script output                                 |
| exp    | expect output                                 |
| log    | all above                                     |
| php    | php temp script file                          |

## Clean

Run `./clean` to remove all of the tests log files.

## Contribute the test script

Run `./new [test-script-filename]`

E.g. : `./new ./swoole-coroutine/co_sleep.phpt`