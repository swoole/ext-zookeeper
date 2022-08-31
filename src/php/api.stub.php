<?php
/**
 * @generate-legacy-arginfo
 * @generate-function-entries
 */
namespace Swoole {

    class ZooKeeper
    {
        public function __construct(string $host, float $timeout)
        {
        }

        public function create(string $path, string $value, int $flags = 0) : bool
        {
        }

        public function addAuth(string $scheme, string $cert): bool
        {
        }

        public function set(string $path, string $value, int $version = -1): bool
        {
        }

        public function get(string $path, int $version = -1): mixed
        {
        }

        public function exists(string $path): bool
        {
        }

        public function delete(string $path, int $version = -1): bool
        {
        }

        public function setAcl(string $path, array $acl, int $version): bool
        {
        }

        public function getAcl(string $path): mixed
        {
        }

        public function getChildren(string $path): mixed
        {
        }
        
        public function getClientId(): array
        {
        }

        public function watch(string $path): bool
        {
        }
        
        public function wait(): bool
        {
        }

        public function watchChildren(string $path): mixed
        {
        }

        static public function setDebugLevel(int $level): bool
        {
        }

        public function getState(): int
        {
        }

        public function setDeterministicConnOrder(bool $value): void
        {
        }

        public function setLogStream(\resource $stream): void
        {
        }

        public function setWatcher(callable $watcher): void
        {
        }
    }
}
