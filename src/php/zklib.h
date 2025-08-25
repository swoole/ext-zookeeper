//
// Created by zhanglei on 19-4-23.
//

#pragma once

#include <stdbool.h>
#include "phpx.h"
#include "zookeeper.h"

namespace zookeeper {
struct ACL_vector *convert_array_to_acl(php::Array *param_array);
void convert_stat_to_array(php::Array *destArray, struct Stat *stat);
void convert_acl_to_array(php::Array *destArray, struct ACL_vector *acl);
bool free_acl_struct(struct ACL_vector *acl_vector);
}  // namespace zookeeper
