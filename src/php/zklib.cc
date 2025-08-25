#include "zklib.h"
#include "phpx.h"

using namespace php;

namespace zookeeper {

/**
 * 将路径的状态赋值到一个数组中
 * @param destArray
 * @param stat
 */
void convert_stat_to_array(Array *destArray, struct Stat *stat) {
    Array statArray;
    destArray->set("stat", statArray);
    statArray.set("czxid", (long) stat->czxid);
    statArray.set("mzxid", (long) stat->mzxid);
    statArray.set("ctime", (long) stat->ctime);
    statArray.set("mtime", (long) stat->mtime);
    statArray.set("version", stat->version);
    statArray.set("cversion", stat->cversion);
    statArray.set("aversion", stat->aversion);
    statArray.set("ephemeralOwner", (long) stat->ephemeralOwner);
    statArray.set("dataLength", stat->dataLength);
    statArray.set("numChildren", stat->numChildren);
    statArray.set("pzxid", (long) stat->pzxid);
}

/**
 * 将acl赋值到数组
 * @param destArray
 * @param acl
 */
void convert_acl_to_array(Array *destArray, struct ACL_vector *acl) {
    Array aclArray;
    destArray->set("acl", aclArray);
    for (int i = 0; i < acl->count; i++) {
        Array aclSaveArray;
        Array idArray;
        struct ACL *acl_unit = acl->data + i;
        aclSaveArray.set("perms", acl_unit->perms);
        idArray.set("id", acl_unit->id.id);
        idArray.set("scheme", acl_unit->id.scheme);
        aclSaveArray.set("id", idArray);
        aclArray.set(i, aclSaveArray);
    }
}

//// 把一个数组转化为acl结构体,由于接口对外需要严格检查数据结构
bool convert_array_to_acl(Array *param_array, struct ACL_vector *zookeeper_acl) {
    Variant tmp_unit_check;
    //初始化结构体
    zookeeper_acl->count = (int32_t) param_array->count();

    struct ACL zookeeper_acl_unit[zookeeper_acl->count];

    for (int i = 0; i < zookeeper_acl->count; i++) {
        struct Id acl_unit;
        tmp_unit_check = param_array->get(i);
        //检查索引防止用户填写错误
        if (tmp_unit_check == nullptr) {
            error(E_WARNING, "setAcl:get index error");
            return false;
        }
        Array acl_unit_array(tmp_unit_check);

        //检查索引防止用户填写错误
        if (tmp_unit_check == nullptr) {
            error(E_WARNING, "setAcl:get id error");
            return false;
        }

        tmp_unit_check = acl_unit_array.get("id");
        Array acl_unit_array_id(tmp_unit_check);
        zookeeper_acl_unit[i].perms = (int32_t) acl_unit_array.get("perms").toInt();
        auto id = acl_unit_array_id.get("id").toStdString();
        acl_unit.id = estrndup(id.c_str(), id.length());
        auto scheme = acl_unit_array_id.get("scheme").toStdString();
        acl_unit.scheme = estrndup(scheme.c_str(), scheme.length());
        zookeeper_acl_unit[i].id = acl_unit;
    }
    zookeeper_acl->data = zookeeper_acl_unit;
    return zookeeper_acl;
}

//把一个数组转化为acl结构体
struct ACL_vector *convert_array_to_acl(Array *param_array) {
    struct ACL_vector *acl_vector;
    struct ACL *create_acl;
    struct ACL *acl_collect;
    //初始化结构体
    Array zk_array(*param_array);
    acl_vector = (struct ACL_vector *) emalloc(sizeof(struct ACL_vector));

    //初始化这个结构体
    bzero(acl_vector, sizeof(struct ACL_vector));

    //设置这个架构体里acl的数量
    acl_vector->count = (int32_t) zk_array.count();

    //申请一块连续的内存空间
    acl_collect = (struct ACL *) emalloc(acl_vector->count * sizeof(struct ACL));
    if (acl_collect == nullptr) {
        return nullptr;
    }

    for (int i = 0; i < acl_vector->count; i++) {
        struct ACL create_acl {};

        /*====================写入perms======================*/
        Array deal_array(zk_array[0]);
        Variant perms = deal_array.get("perms");
        if (perms == nullptr) {
            return nullptr;
        }
        if (!perms.isInt() && !perms.isString()) {
            return nullptr;
        }
        create_acl.perms = (int32_t) perms.toInt();
        Variant acl_id = deal_array.get("id");
        if (acl_id == nullptr) {
            return nullptr;
        }
        if (!acl_id.isArray()) {
            return nullptr;
        }

        /*====================构造Id结构体======================*/
        Array acl_array(acl_id);
        Variant scheme = acl_array.get("scheme");
        if (scheme == nullptr) {
            return nullptr;
        }
        if (!scheme.isString()) {
            return nullptr;
        }
        std::string std_scheme = scheme.toStdString();
        create_acl.id.scheme = estrndup(std_scheme.c_str(), std_scheme.length());
        Variant acl_struct_id = acl_array.get("id");
        if (scheme == nullptr) {
            return nullptr;
        }
        if (!scheme.isString()) {
            return nullptr;
        }
        std::string std_acl_struct_id = acl_struct_id.toStdString();
        create_acl.id.id = estrndup(std_acl_struct_id.c_str(), std_acl_struct_id.length());

        //将结构体装入ACL
        acl_collect[i] = create_acl;
    }
    acl_vector->data = acl_collect;
    return acl_vector;
}

//释放掉acl结构体
bool free_acl_struct(struct ACL_vector *acl_vector) {
    if (acl_vector && acl_vector->data) {
        for (int i = 0; i < acl_vector->count; i++) {
            efree(const_cast<char *>((acl_vector->data + i)->id.id));
            efree(const_cast<char *>((acl_vector->data + i)->id.scheme));
            efree((acl_vector->data + i));
        }
        efree(acl_vector->data);
        efree(acl_vector);
        return true;
    } else {
        return false;
    }
}
}  // namespace zookeeper
