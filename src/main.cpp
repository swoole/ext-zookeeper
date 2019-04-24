#include <assert.h>
#include <iostream>
#include <string.h>

#include "phpx.h"
#include "zookeeper.h"
#ifndef EXT_ZOOKEEPR_ZK_ZEND_H
#include "zk_zend.h"
#endif
using namespace php;
using namespace std;
using namespace zookeeperZend;

zhandle_t *handle = NULL;
int connected = 0;
int expired = 0;
struct timeval startTime;

struct QueryResult
{
    QueryResult() :
            retval(nullptr)
    {
        running = true;
        error = 0;
    }

    bool running;
    int error;
    Variant retval;
};

void dumpStat(const struct Stat *stat)
{
    char tctimes[40];
    char tmtimes[40];
    time_t tctime;
    time_t tmtime;

    if (!stat)
    {
        fprintf(stderr, "null\n");
        return;
    }
    tctime = stat->ctime / 1000;
    tmtime = stat->mtime / 1000;

    ctime_r(&tmtime, tmtimes);
    ctime_r(&tctime, tctimes);

    fprintf(stderr, "\tctime = %s\tczxid=%ld\n"
            "\tmtime=%s\tmzxid=%ld\n"
            "\tversion=%x\taversion=%x\n"
            "\tephemeralOwner = %ld\n", tctimes, stat->czxid, tmtimes, stat->mzxid, stat->version, stat->aversion,
            stat->ephemeralOwner);
}

void my_string_completion(int rc, const char *name, const void *data)
{
    QueryResult *result = (QueryResult *) data;
    result->error = rc;
    if (rc == ZOK)
    {
        if (name == nullptr)
        {
            result->retval = true;
        }
        else
        {
            result->retval = Variant(name);
        }
    }
    else
    {
        result->retval = false;
    }
    result->running = false;
}

void my_string_completion_free_data(int rc, const char *name, const void *data)
{
    my_string_completion(rc, name, data);
}

void my_data_completion(int rc, const char *value, int value_len, const struct Stat *stat, const void *data)
{
    struct timeval tv;
    int sec;
    int usec;
    gettimeofday(&tv, 0);
    sec = tv.tv_sec - startTime.tv_sec;
    usec = tv.tv_usec - startTime.tv_usec;
    fprintf(stderr, "time = %d msec\n", sec * 1000 + usec / 1000);
    fprintf(stderr, "%s: rc = %d\n", (char*) data, rc);
    if (value)
    {
        fprintf(stderr, " value_len = %d\n", value_len);
    }
    fprintf(stderr, "\nStat:\n");
    dumpStat(stat);
    free((void*) data);
}

void my_silent_data_completion(int rc, const char *value, int value_len, const struct Stat *stat, const void *data)
{
    QueryResult *result = (QueryResult *) data;
    result->error = rc;
    if (rc == ZOK)
    {
        result->retval = Variant(value, value_len);
    }
    else
    {
        result->retval = false;
    }
    result->running = false;
}

void my_strings_completion(int rc, const struct String_vector *strings, const void *data)
{
    struct timeval tv;
    int sec;
    int usec;
    int i;

    gettimeofday(&tv, 0);
    sec = tv.tv_sec - startTime.tv_sec;
    usec = tv.tv_usec - startTime.tv_usec;
    fprintf(stderr, "time = %d msec\n", sec * 1000 + usec / 1000);
    fprintf(stderr, "%s: rc = %d\n", (char*) data, rc);
    if (strings)
    {
        for (i = 0; i < strings->count; i++)
        {
            fprintf(stderr, "\t%s\n", strings->data[i]);
        }
    }
    free((void*) data);
    gettimeofday(&tv, 0);
    sec = tv.tv_sec - startTime.tv_sec;
    usec = tv.tv_usec - startTime.tv_usec;
    fprintf(stderr, "time = %d msec\n", sec * 1000 + usec / 1000);
}

void my_strings_stat_completion(int rc, const struct String_vector *strings, const struct Stat *stat, const void *data)
{
    my_strings_completion(rc, strings, data);
    dumpStat(stat);
}

void my_void_completion(int rc, const void *data)
{
    QueryResult *result = (QueryResult *) data;
    result->error = rc;
    if (rc == ZOK)
    {
        result->retval = true;
    }
    else
    {
        result->retval = false;
    }
    result->running = false;
}

void my_stat_completion(int rc, const struct Stat *stat, const void *data)
{
    fprintf(stderr, "%s: rc = %d Stat:\n", (char*) data, rc);
    dumpStat(stat);

    QueryResult *result = (QueryResult *) data;
    result->error = rc;
    if (rc == ZOK)
    {
        result->retval = true;
    }
    else
    {
        result->retval = false;
    }
    result->running = false;
}

void my_silent_stat_completion(int rc, const struct Stat *stat, const void *data)
{
    QueryResult *result = (QueryResult *) data;
    result->error = rc;
    if (rc == ZOK)
    {
        result->retval = true;
    }
    else
    {
        result->retval = false;
    }
    result->running = false;
}

void my_acl_stat_completion(int rc, struct ACL_vector *acl,struct Stat *stat, const void *data)
{
    QueryResult *result = (QueryResult *) data;
    result->error = rc;
    if (rc == ZOK)
    {
        Array res;
        //把acl转化到数组
        zKLib::convert_acl_to_array(&res,acl);
        //把stat转化到数组
        zKLib::convert_stat_to_array(&res,stat);
        result->retval = res;
    }
    else
    {
        result->retval = false;
    }
    result->running = false;
}

void my_set_acl_completion(int rc, const void *data)
{
    QueryResult *result = (QueryResult *) data;
    result->error = rc;
    if (rc == ZOK)
    {
        result->retval = true;
    }
    else
    {
        result->retval = false;
    }
    result->running = false;
}

PHPX_METHOD(zookeeper, __construct)
{
    auto host = args[0];
    double recv_timeout = args[1].toInt();
    int recv_timeout_ms = recv_timeout * 1000;
    zoo_deterministic_conn_order(1);
    zhandle_t *zh = zookeeper_init(host.toCString(), nullptr, recv_timeout_ms, 0, NULL, 0);
    _this.oSet<zhandle_t>("handle", "zhandle_t", zh);
}

PHPX_METHOD(zookeeper, get)
{
    struct timeval tv;
    int events = ZOOKEEPER_READ;
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    int fd, rc;
    QueryResult result;
    result.running = true;

    rc = zookeeper_interest(zh, &fd, &events, &tv);
    if (rc)
    {
        _error: _this.set("errCode", rc);
        retval = false;
        return;
    }
    rc = zoo_aget(zh, args[0].toCString(), 0, my_silent_data_completion, &result);
    if (rc)
    {
        goto _error;
    }

    while (result.running)
    {
        rc = zookeeper_interest(zh, &fd, &events, &tv);
        if (rc)
        {
            goto _error;
        }
        rc = zookeeper_process(zh, events);
        if (rc)
        {
            goto _error;
        }
    }

    retval = result.retval;
    if (result.error != 0)
    {
        _this.set("errCode", result.error);
    }
}

PHPX_METHOD(zookeeper, exists)
{
    struct timeval tv;
    int events = ZOOKEEPER_READ;
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    int fd, rc;
    QueryResult result;
    result.running = true;

    rc = zookeeper_interest(zh, &fd, &events, &tv);
    if (rc)
    {
        _error: _this.set("errCode", rc);
        retval = false;
        return;
    }
    rc = zoo_aexists(zh, args[0].toCString(), 0, my_stat_completion, &result);
    if (rc)
    {
        goto _error;
    }

    while (result.running)
    {
        rc = zookeeper_interest(zh, &fd, &events, &tv);
        if (rc)
        {
            goto _error;
        }
        rc = zookeeper_process(zh, events);
        if (rc)
        {
            goto _error;
        }
    }

    retval = result.retval;
    if (result.error != 0)
    {
        _this.set("errCode", result.error);
    }
}

PHPX_METHOD(zookeeper, create)
{
    struct timeval tv;
    int events = ZOOKEEPER_READ;
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    int fd, rc;
    QueryResult result;

    rc = zookeeper_interest(zh, &fd, &events, &tv);
    if (rc)
    {
        _error: _this.set("errCode", rc);
        retval = false;
        return;
    }

    long flags = args.count() >= 3 ? args[2].toInt() : 0;
    rc = zoo_acreate(zh, args[0].toCString(), args[1].toCString(), args[1].length(), &ZOO_OPEN_ACL_UNSAFE, flags,
            my_string_completion_free_data, &result);
    if (rc)
    {
        goto _error;
    }

    while (result.running)
    {
        rc = zookeeper_interest(zh, &fd, &events, &tv);
        if (rc)
        {
            goto _error;
        }
        rc = zookeeper_process(zh, events);
        if (rc)
        {
            goto _error;
        }
    }

    retval = result.retval;
    if (result.error != 0)
    {
        _this.set("errCode", result.error);
    }
}

PHPX_METHOD(zookeeper, set)
{
    struct timeval tv;
    int events;
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    int fd, rc;
    QueryResult result;
    long version = -1;

    if (args.count() > 2)
    {
        version = args[2].toInt();
    }

    rc = zookeeper_interest(zh, &fd, &events, &tv);
    if (rc)
    {
        _error: _this.set("errCode", rc);
        retval = false;
        return;
    }
    rc = zoo_aset(zh, args[0].toCString(), args[1].toCString(), args[1].length(), (int) version,
            my_silent_stat_completion, &result);

    while (result.running)
    {
        rc = zookeeper_interest(zh, &fd, &events, &tv);
        if (rc)
        {
            goto _error;
        }
        rc = zookeeper_process(zh, events);
        if (rc)
        {
            goto _error;
        }
    }

    retval = result.retval;
    if (result.error != 0)
    {
        _this.set("errCode", result.error);
    }
}

PHPX_METHOD(zookeeper, delete)
{
    struct timeval tv;
    int events;
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    int fd, rc;
    QueryResult result;
    long version = -1;

    if (args.count() > 1)
    {
        version = args[2].toInt();
    }

    rc = zookeeper_interest(zh, &fd, &events, &tv);
    if (rc)
    {
        _error: _this.set("errCode", rc);
        retval = false;
        return;
    }
    rc = zoo_adelete(zh, args[0].toCString(), (int) version, my_void_completion, &result);

    while (result.running)
    {
        rc = zookeeper_interest(zh, &fd, &events, &tv);
        if (rc)
        {
            goto _error;
        }
        rc = zookeeper_process(zh, events);
        if (rc)
        {
            goto _error;
        }
    }

    retval = result.retval;
    if (result.error != 0)
    {
        _this.set("errCode", result.error);
    }
}

PHPX_METHOD(zookeeper, setDebugLevel)
{
    long level = args[0].toInt();
    zoo_set_debug_level((ZooLogLevel) level);
}

void zookeeper_dtor(zend_resource *res)
{
    zhandle_t *zh = static_cast<zhandle_t *>(res->ptr);
    zookeeper_close(zh);
}

PHPX_METHOD(zookeeper, getAcl)
{
    int events;
    struct timeval tv;
    QueryResult result;
    result.running = true;
    int fd, rc;
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");

    //等待连接完成
    rc = zookeeper_interest(zh, &fd, &events, &tv);

    //参数检查
    if(args.count() != 1)
    {
        error(E_WARNING,"You can only pass one parameter");
        retval = false;
        return;
    }

    if(!args[0].isString())
    {
        error(E_WARNING,"Path must be string");
        retval = false;
        return;
    }

    if (rc)
    {
        _error: _this.set("errCode", rc);
        retval = false;
        return;
    }
    rc = zoo_aget_acl(zh,args[0].toCString(),my_acl_stat_completion, &result);
    while (result.running)
    {
        rc = zookeeper_interest(zh, &fd, &events, &tv);
        if (rc)
        {
            goto _error;
        }
        rc = zookeeper_process(zh, events);
        if (rc)
        {
            goto _error;
        }
    }

    retval = result.retval;
    if (result.error != 0)
    {
        _this.set("errCode", result.error);
    }

    return;
}

PHPX_METHOD(zookeeper, setAcl)
{
    struct ACL_vector *zookeeper_acl;
    QueryResult result;
    long version = -1;
    int events;
    int fd, rc;
    struct timeval tv;
    //至少有一个参数
    if(args.count() < 1)
    {
        error(E_WARNING, "must be have one param");
        fail:
            retval = false;
            return;
    }

    //如果第一个参数不是一个字符串
    if(!(args[0].isString()))
    {
        error(E_WARNING, "first param must be string");
        goto fail;
    }


    //版本号必须是一个整数
    if(args.count() > 1 && args.count() == 2)
    {
        if(!(args[1].isArray()))
        {
            error(E_WARNING, "second param must be array");
            goto fail;
        }
    }

    if(args.count() == 3)
    {
        if(!(args[2].isInt())) {
            error(E_WARNING, "third param must be int");
            goto fail;
        }

        version = args[2].toInt();
    }


    Array acl_array(args[1]);
    zookeeper_acl = zKLib::convert_array_to_acl(&acl_array);
    if(!(zookeeper_acl))
    {
        error(E_WARNING, "acl array set error");
        goto fail;
    }

    int  a=1;

    //初始化结构体
    zhandle_t *zh = _this.oGet<zhandle_t>("handle","zhandle_t");
    rc = zookeeper_interest(zh, &fd, &events, &tv);
    if (rc)
    {
        _error: _this.set("errCode", rc);
        retval = false;
        return;
    }
    rc = zoo_aset_acl(zh,args[0].toCString(),version,zookeeper_acl,my_set_acl_completion,&result);

    zKLib::free_acl_struct(zookeeper_acl);

    while(result.running)
    {
        rc = zookeeper_interest(zh, &fd, &events, &tv);
        if (rc)
        {
            goto _error;
        }
        rc = zookeeper_process(zh, events);
        if (rc)
        {
            goto _error;
        }
    }

    retval = result.retval;
    if (result.error != 0)
    {
        _this.set("errCode", result.error);
    }

    return;

}

PHPX_EXTENSION()
{
    Extension *ext = new Extension("swoole_zookeeper", "0.0.1");

    ext->onStart = [ext]() noexcept
    {
        ext->registerResource("zhandle_t", zookeeper_dtor);

        char version[64];
        sprintf(version, "%d.%d.%d", ZOO_MAJOR_VERSION, ZOO_MINOR_VERSION, ZOO_PATCH_VERSION);
        ext->registerConstant("ZOO_VERSION", version);
        ext->registerConstant("ZOO_EPHEMERAL", ZOO_EPHEMERAL);
        ext->registerConstant("ZOO_SEQUENCE", ZOO_SEQUENCE);

        Class *c = new Class("swoole\\zookeeper");
        c->addProperty("errCode", 0);
        c->addConstant("PERM_READ",ZOO_PERM_READ);
        c->addConstant("PERM_WRITE",ZOO_PERM_WRITE);
        c->addConstant("PERM_ALL",ZOO_PERM_ALL);
        c->addConstant("PERM_ADMIN",ZOO_PERM_ADMIN);
        c->addConstant("PERM_CREATE",ZOO_PERM_CREATE);
        c->addConstant("PERM_DELETE",ZOO_PERM_DELETE);
        c->addMethod(PHPX_ME(zookeeper, __construct));
        c->addMethod(PHPX_ME(zookeeper, create));
        c->addMethod(PHPX_ME(zookeeper, set));
        c->addMethod(PHPX_ME(zookeeper, get));
        c->addMethod(PHPX_ME(zookeeper, getAcl));
        c->addMethod(PHPX_ME(zookeeper, exists));
        c->addMethod(PHPX_ME(zookeeper, delete));
        c->addMethod(PHPX_ME(zookeeper, setAcl));
        c->addMethod(PHPX_ME(zookeeper, setDebugLevel), STATIC);
        ext->registerClass(c);
    };

    ext->require("swoole");

    ext->info(
    { "zookeeper support", "enabled" },
    {
    { "version", ext->version },
    { "date", "2018-08-31" }, });
    return ext;
}
