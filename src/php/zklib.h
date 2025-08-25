#pragma once

#include "phpx.h"
#include "zookeeper.h"

namespace zookeeper {
ACL_vector *convert_array_to_acl(php::Array *param_array);
void convert_stat_to_array(php::Array *destArray, const Stat *stat);
void convert_acl_to_array(php::Array *destArray, ACL_vector *acl);
void free_acl_struct(ACL_vector *acl_vector);
}  // namespace zookeeper
