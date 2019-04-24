//
// Created by zhanglei on 19-4-23.
//

#ifndef EXT_ZOOKEEPR_ZK_ZEND_H
#define EXT_ZOOKEEPR_ZK_ZEND_H

#endif //EXT_ZOOKEEPR_ZK_ZEND_H

namespace zookeeperZend {

    class zKLib{
        public:
            //把一个数组转化为acl结构体
            static struct ACL_vector * convert_array_to_acl(php::Array *param_array);

            //转化stat到数组
            static void convert_stat_to_array(php::Array* destArray,struct Stat *stat);

            //转化acl到数组
            static void convert_acl_to_array(php::Array* destArray,struct ACL_vector *acl);

            //释放掉
            static bool free_acl_struct(struct ACL_vector* acl_vector);
    };

}
