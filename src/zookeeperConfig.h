//
// Created by zhanglei on 19-4-26.
//
#include <stdbool.h>
#include <phpx.h>
#include "zookeeper.h"

#ifndef EXT_ZOOKEEPR_ZOOKEEPERCONFIG_H
#define EXT_ZOOKEEPR_ZOOKEEPERCONFIG_H

#endif //EXT_ZOOKEEPR_ZOOKEEPERCONFIG_H

using namespace php;
using namespace std;

PHPX_METHOD(zookeeperConfig, __construct);

PHPX_METHOD(zookeeperConfig, add);

PHPX_METHOD(zookeeperConfig, get);

PHPX_METHOD(zookeeperConfig, remove);

PHPX_METHOD(zookeeperConfig, set);

PHPX_METHOD(zookeeperConfig, __destruct);

void init_zookeeper_config(Extension *ext);