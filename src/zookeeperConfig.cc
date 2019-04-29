//
// Created by zhanglei on 19-4-26.
//
#ifndef EXT_ZOOKEEPR_ZOOKEEPERCONFIG_H
#include "zookeeperConfig.h"
#endif //EXT_ZOOKEEPR_ZOOKEEPERCONFIG_H

PHPX_METHOD(zookeeperConfig, __construct)
{

}

PHPX_METHOD(zookeeperConfig, add)
{

}

PHPX_METHOD(zookeeperConfig, get)
{

}

PHPX_METHOD(zookeeperConfig, remove)
{

}

PHPX_METHOD(zookeeperConfig, set)
{

}

PHPX_METHOD(zookeeperConfig, __destruct)
{

}

//挂载实例的方法
void init_zookeeper_config(Extension *ext)
{
    Class *c = new Class("swoole\\zookeeperConfig");
    c->addMethod(PHPX_ME(zookeeperConfig, __construct));
    c->addMethod(PHPX_ME(zookeeperConfig, add));
    c->addMethod(PHPX_ME(zookeeperConfig, get));
    c->addMethod(PHPX_ME(zookeeperConfig, remove));
    c->addMethod(PHPX_ME(zookeeperConfig, set));
    c->addMethod(PHPX_ME(zookeeperConfig, __destruct));
    ext->registerClass(c);
}