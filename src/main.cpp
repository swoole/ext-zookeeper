#include <assert.h>
#include <iostream>
#include <string.h>

#include "phpx.h"
#include "zookeeper.h"

using namespace php;
using namespace std;

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
    QueryResult *result = (QueryResult *) data;
    result->error = rc;
    if (rc == ZOK)
    {
        Array array;
        if (strings)
        {
            int i;
            for (i = 0; i < strings->count; i++)
            {
                array.append(strings->data[i]);
            }
        }

        result->retval = array;
    }
    else
    {
        result->retval = false;
    }
    result->running = false;
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


/**
 * C acl信息转换为php数组信息
 * 参考https://github.com/andreiz/php-zookeeper/blob/master/php_zookeeper.c#L922
 * @param aclv
 * @param array
 */
static void php_aclv_to_array(const struct ACL_vector *aclv, Array *array)
{
    Array *temp = new Array();
    int i;
    for (i = 0; i < aclv->count; i++) {
        temp->clean();
        temp->set("perms",aclv->data[i].perms);
        temp->set("scheme",aclv->data[i].id.scheme);
        temp->set("id",aclv->data[i].id.id);
        array->append(*temp);
    }
}


/**
 * stats转转换php数组信息
 * @param stat
 * @param array
 */
static void php_stat_to_array(const struct Stat *stat, Array *array)
{
    array->clean();
    array->set("czxid",stat->czxid);
    array->set("mzxid",stat->mzxid);
    array->set("ctime",stat->ctime);
    array->set("mtime",stat->mtime);
    array->set("version",stat->version);
    array->set("cversion",stat->cversion);
    array->set("aversion",stat->aversion);
    array->set("ephemeralOwner",stat->ephemeralOwner);
    array->set("dataLength",stat->dataLength);
    array->set("numChildren",stat->numChildren);
    array->set("pzxid",stat->pzxid);
}

/**
 * 获取acl完成回调方法
 * @param rc
 * @param acl
 * @param stat
 * @param data
 */
void my_acl_completion(int rc, struct ACL_vector *acl,struct Stat *stat, const void *data){
    QueryResult *result = (QueryResult *) data;
    result->error = rc;
    if (rc == ZOK)
    {
        Array _array(result->retval);
        Array *aclArr = new Array(),*statArr = new Array();

        php_stat_to_array(stat,statArr);
        php_aclv_to_array(acl,aclArr);

        _array.append(*statArr);
        _array.append(*aclArr);

        //返回两个数组，stat信息数组和，acl信息数组
        result->retval=_array;
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
    //获取对象中链接的指针，通过zookeeper_init创建，由phpx的oSet方法存储
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    int fd, rc;
    //初始换QueryResult结构体，
    QueryResult result;
    result.running = true;//并且设置running为true

    //rc：zookeeper.h中ZOO_ERRORS
    rc = zookeeper_interest(zh, &fd, &events, &tv);
    if (rc)
    {
        _error: _this.set("errCode", rc);
        retval = false;
        return;
    }
    //1.zh为链接操作句柄
    //2.传入需要获取节点路径
    //3.如果非 0，则在服务器端设置监视，当节点发生变化时客户端会得到通知。
    //4.回调方法,当操作结束后，会调用my_silent_data_completion方法，并且会传入result
    rc = zoo_aget(zh, args[0].toCString(), 0, my_silent_data_completion, &result);
    if (rc)
    {
        goto _error;
    }

    //当执行到这一步，如果已经完成了会运行my_silent_data_completion，则result.running=false
    //如果到这一步，获取操作还没完成，还没调用过my_silent_data_completion，则result.running为true，循环直到
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

    //执行my_silent_data_completion后，在my_silent_data_completion方法中会将结果value设置在result.retval中
    retval = result.retval;
    if (result.error != 0)
    {
        _this.set("errCode", result.error);
    }
}

PHPX_METHOD(zookeeper, addAuth)
{
    struct timeval tv;
    int events;
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    int fd, rc;
    QueryResult result;
    result.running = true;

    if (args.count() > 1)
    {
        return ;
    }

    rc = zookeeper_interest(zh, &fd, &events, &tv);
    if (rc)
    {
        _error: _this.set("errCode", rc);
        retval = false;
        return;
    }
    rc = zoo_add_auth(zh, args[0].toCString(), args[1].toCString(),args[1].length(), my_void_completion, &result);
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

PHPX_METHOD(zookeeper, getAcl)
{
    struct timeval tv;
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    //初始换QueryResult结构体，
    QueryResult result;
    result.running = true;//并且设置running为true
    int events = ZOOKEEPER_READ;
    int fd, rc;
    //rc：zookeeper.h中ZOO_ERRORS
    rc = zookeeper_interest(zh, &fd, &events, &tv);
    if (rc)
    {
        _error: _this.set("errCode", rc);
        retval = false;
        return;
    }
    rc = zoo_aget_acl(zh, args[0].toCString(),my_acl_completion,&result);

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

    //执行my_silent_data_completion后，在my_acl_completion方法中会将结果value设置在result.retval中
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

PHPX_METHOD(zookeeper, getChildren)
{
    struct timeval tv;
    int events;
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
    rc = zoo_aget_children(zh, args[0].toCString(), 0, my_strings_completion, &result);

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

PHPX_METHOD(zookeeper, getState)
{
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    retval = zoo_state(zh);
}

PHPX_METHOD(zookeeper, getClientId)
{
    const clientid_t *cid;
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    cid = zoo_client_id(zh);
    Array rv = Array();
    rv.append((long)cid->client_id);
    rv.append((char *)cid->passwd);
    retval = rv;
}

PHPX_METHOD(zookeeper, setDeterministicConnOrder)
{
    bool value = args[0].toBool();
    zoo_deterministic_conn_order(value);
}

void zookeeper_dtor(zend_resource *res)
{
    zhandle_t *zh = static_cast<zhandle_t *>(res->ptr);
    zookeeper_close(zh);
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
        c->addMethod(PHPX_ME(zookeeper, __construct));
        c->addMethod(PHPX_ME(zookeeper, create));
        c->addMethod(PHPX_ME(zookeeper, addAuth));
        c->addMethod(PHPX_ME(zookeeper, set));
        c->addMethod(PHPX_ME(zookeeper, get));
        c->addMethod(PHPX_ME(zookeeper, exists));
        c->addMethod(PHPX_ME(zookeeper, delete));
        c->addMethod(PHPX_ME(zookeeper, getAcl));
        c->addMethod(PHPX_ME(zookeeper, getChildren));
        c->addMethod(PHPX_ME(zookeeper, setDebugLevel), STATIC);
        c->addMethod(PHPX_ME(zookeeper, getState));
        c->addMethod(PHPX_ME(zookeeper, getClientId));
        c->addMethod(PHPX_ME(zookeeper, setDeterministicConnOrder), PUBLIC, new ArgInfo(1));
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
