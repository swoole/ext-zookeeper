//
// Created by zhanglei on 19-4-23.
//

#include <stdbool.h>
#include <phpx.h>
#include "zookeeper.h"
#ifndef EXT_ZOOKEEPR_ZK_ZEND_H
#include "zk_zend.h"

#endif
using namespace php;
using namespace zookeeperZend;

//将路径的状态赋值到一个数组中
/**
 *
 * @param destArray
 * @param stat
 */
void zKLib::convert_stat_to_array(Array* destArray,struct Stat *stat)
{
    Array statArray;
    destArray->set("stat",statArray);
    statArray.set("czxid",stat->czxid);
    statArray.set("mzxid",stat->mzxid);
    statArray.set("ctime",stat->ctime);
    statArray.set("mtime",stat->mtime);
    statArray.set("version",stat->version);
    statArray.set("cversion",stat->cversion);
    statArray.set("aversion",stat->aversion);
    statArray.set("ephemeralOwner",stat->ephemeralOwner);
    statArray.set("dataLength",stat->dataLength);
    statArray.set("numChildren",stat->numChildren);
    statArray.set("pzxid",stat->pzxid);
}

//将acl赋值到数组
/**
 *
 * @param destArray
 * @param acl
 */
void zKLib::convert_acl_to_array(Array* destArray,struct ACL_vector *acl)
{
    Array aclArray;
    destArray->set("acl",aclArray);
    for(int i=0;i<acl->count;i++)
    {
        Array aclSaveArray;
        Array idArray;
        struct ACL *acl_unit = acl->data+i;
        aclSaveArray.set("perms",acl_unit->perms);
        idArray.set("id",acl_unit->id.id);
        idArray.set("scheme",acl_unit->id.scheme);
        aclSaveArray.set("id",idArray);
        aclArray.set(i,aclSaveArray);
    }
}

//把一个数组转化为acl结构体
struct ACL_vector * zKLib::convert_array_to_acl(Array* param_array)
{
    struct ACL_vector* acl_vector;
    struct ACL *create_acl;
    struct ACL *acl_collect;
    //初始化结构体
    Array zk_array(*param_array);
    acl_vector = (struct ACL_vector*)emalloc(sizeof(struct ACL_vector));
    bzero(acl_vector,sizeof(struct ACL_vector));
    acl_vector->count = (int32_t)zk_array.count();

    acl_collect = (struct ACL *)emalloc(acl_vector->count * sizeof(struct ACL));

    for(int i=0;i<acl_vector->count;i++) {
        create_acl = (struct ACL *) emalloc(sizeof(struct ACL));
        bzero(create_acl, sizeof(struct ACL));
        Array deal_array(zk_array[0]);
        Variant perms = deal_array.get("perms");

        if (perms == nullptr) {
            return nullptr;
        }
        if (!perms.isInt() && !perms.isString()) {
            return nullptr;
        }

        create_acl->perms = (int32_t) perms.toInt();
        Variant acl_id = deal_array.get("acl_id");

        if (acl_id == nullptr) {
            return nullptr;
        }

        if (!acl_id.isArray()) {
            return nullptr;
        }

        Array acl_array(acl_id);
        Variant scheme = acl_array.get("scheme");

        if (scheme == nullptr) {
            return nullptr;
        }
        if (!scheme.isString()) {
            return nullptr;
        }

        std::string std_scheme = scheme.toString();
        char *scheme_buf = (char *) emalloc(std_scheme.length() + 1);
        bzero(scheme_buf, strlen(scheme_buf) + 1);
        strcpy(scheme_buf, std_scheme.c_str());
        create_acl->id.scheme = scheme_buf;
        Variant acl_struct_id = acl_array.get("id");

        if (scheme == nullptr) {
            return nullptr;
        }
        if (!scheme.isString()) {
            return nullptr;
        }

        std::string std_acl_struct_id = acl_struct_id.toString();
        char *acl_struct_id_buf = (char *) emalloc(std_acl_struct_id.length() + 1);
        bzero(acl_struct_id_buf, (std_acl_struct_id.length()) + 1);
        strcpy(acl_struct_id_buf, std_acl_struct_id.c_str());
        create_acl->id.id = acl_struct_id_buf;

        acl_collect[i] = *create_acl;
    }
    acl_vector->data = acl_collect;
    return acl_vector;
}

bool zKLib::free_acl_struct(struct ACL_vector* acl_vector)
{
    if(acl_vector)
    {
        int count = acl_vector->count;
        for(int i=0;i<count;i++)
        {
            if(acl_vector->data)
            {
                efree((acl_vector->data+i)->id.id);
                efree((acl_vector->data+i)->id.scheme);
                efree((acl_vector->data+i));
            }
        }
        efree(acl_vector->data);
        efree(acl_vector);
        return true;
    }else{
        return  false;
    }
}