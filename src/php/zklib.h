//
// Created by zhanglei on 19-4-23.
//

#pragma once

#include <stdbool.h>
#include "phpx.h"
#include "zookeeper.h"

namespace zookeeperZend {

    class zKLib{
        public:
            //把一个数组转化为acl结构体
//            static bool convert_array_to_acl(php::Array *param_array,struct ACL_vector *zookeeper_acl);
            static struct ACL_vector* convert_array_to_acl(php::Array* param_array);

            //转化stat到数组
            static void convert_stat_to_array(php::Array* destArray,struct Stat *stat);

            //转化acl到数组
            static void convert_acl_to_array(php::Array* destArray,struct ACL_vector *acl);

            //释放掉acl结构体
            static bool free_acl_struct(struct ACL_vector* acl_vector);

    };

}
