#include <assert.h>
#include <iostream>
#include <string.h>

#include "phpx.h"
#include "zookeeper.h"
#include "zklib.h"

using namespace php;
using namespace std;
using namespace zookeeperZend;

zhandle_t *handle = NULL;
int connected = 0;
int expired = 0;
struct timeval startTime;

const bool debug = false;

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

static void dump_stat(const struct Stat *stat)
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

    fprintf(stderr, "\tctime = %s\tczxid=%lld\n"
            "\tmtime=%s\tmzxid=%lld\n"
            "\tversion=%x\taversion=%x\n"
            "\tephemeralOwner = %lld\n", tctimes, stat->czxid, tmtimes, stat->mzxid, stat->version, stat->aversion,
            stat->ephemeralOwner);
}

static void zk_dispatch(Object &_this, zhandle_t *zh, QueryResult &result)
{
    int fd, rc, events = ZOOKEEPER_READ;
    struct timeval tv;
    while (result.running)
    {
        rc = zookeeper_interest(zh, &fd, &events, &tv);
        if (rc)
        {
            _error: _this.set("errCode", rc);
            result.retval = false;
            return;
        }
        rc = zookeeper_process(zh, events);
        if (rc)
        {
            goto _error;
        }
    }
    if (result.error != 0)
    {
        _this.set("errCode", result.error);
    }
}

static void watch_func(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
    Object *_this = (Object *) watcherCtx;
    auto fn = _this->get("watcher");
    Args args;
    Variant key(path);
    args.append(_this->ptr());
    args.append(key);
    call(fn, args);
}

static void my_string_completion(int rc, const char *name, const void *data)
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

static void my_string_completion_free_data(int rc, const char *name, const void *data)
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
    dump_stat(stat);
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
    dump_stat(stat);
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
    if (debug)
    {
        printf("my_stat_completion rc=%d\n", rc);
        dump_stat(stat);
    }

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

/**
 * C acl信息转换为php数组信息
 * 参考https://github.com/andreiz/php-zookeeper/blob/master/php_zookeeper.c#L922
 * @param aclv
 * @param array
 */
static void php_aclv_to_array(const struct ACL_vector *aclv, Array &array)
{
    int i;
    for (i = 0; i < aclv->count; i++)
    {
        Array temp;
        temp.set("perms", aclv->data[i].perms);
        temp.set("scheme", aclv->data[i].id.scheme);
        temp.set("id", aclv->data[i].id.id);
        array.append(temp);
    }
}

/**
 * stats转转换php数组信息
 * @param stat
 * @param array
 */
static void php_stat_to_array(const struct Stat *stat, Array &array)
{
    array.clean();
    array.set("czxid", (long) stat->czxid);
    array.set("mzxid", (long) stat->mzxid);
    array.set("ctime", (long) stat->ctime);
    array.set("mtime", (long) stat->mtime);
    array.set("version", stat->version);
    array.set("cversion", stat->cversion);
    array.set("aversion", stat->aversion);
    array.set("ephemeralOwner", (long) stat->ephemeralOwner);
    array.set("dataLength", stat->dataLength);
    array.set("numChildren", stat->numChildren);
    array.set("pzxid", (long) stat->pzxid);
}

/**
 * 获取acl完成回调方法
 * @param rc
 * @param acl
 * @param stat
 * @param data
 */
void my_acl_completion(int rc, struct ACL_vector *acl, struct Stat *stat, const void *data)
{
    QueryResult *result = (QueryResult *) data;
    result->error = rc;
    if (rc == ZOK)
    {
        Array _array(result->retval);
        Array aclArr, statArr;

        php_stat_to_array(stat, statArr);
        php_aclv_to_array(acl, aclArr);

        _array.append(statArr);
        _array.append(aclArr);

        //返回两个数组，stat信息数组和，acl信息数组
        result->retval = _array;
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
    if(!zh) {
        zend_throw_exception(NULL,"Could not create zookeeper handle", 0);
        _this.oSet<zhandle_t>("handle", "zhandle_t", NULL);
        return;
    }else{
        _this.oSet<zhandle_t>("handle", "zhandle_t", zh);
    }
}

PHPX_METHOD(zookeeper, get)
{
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    QueryResult result;
    int rc = zoo_aget(zh, args[0].toCString(), 0, my_silent_data_completion, &result);
    if (rc)
    {
        retval = false;
        _this.set("errCode", rc);
    }
    else
    {
        zk_dispatch(_this, zh, result);
        retval = result.retval;
    }
}

PHPX_METHOD(zookeeper, addAuth)
{
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    QueryResult result;

    if (args.count() > 2)
    {
        return ;
    }

    int rc = zoo_add_auth(zh, args[0].toCString(), args[1].toCString(), args[1].length(), my_void_completion, &result);
    if (rc)
    {
        retval = false;
        _this.set("errCode", rc);
    }
    else
    {
        zk_dispatch(_this, zh, result);
        retval = result.retval;
    }
}

PHPX_METHOD(zookeeper, getAcl)
{
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    QueryResult result;

    int rc = zoo_aget_acl(zh, args[0].toCString(), my_acl_completion, &result);
    if (rc)
    {
        retval = false;
        _this.set("errCode", rc);
    }
    else
    {
        zk_dispatch(_this, zh, result);
        retval = result.retval;
    }
}

PHPX_METHOD(zookeeper, exists)
{
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    QueryResult result;

    int rc = zoo_aexists(zh, args[0].toCString(), 0, my_stat_completion, &result);
    if (rc)
    {
        retval = false;
        _this.set("errCode", rc);
    }
    else
    {
        zk_dispatch(_this, zh, result);
        retval = result.retval;
    }
}

PHPX_METHOD(zookeeper, create)
{
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    QueryResult result;

    long flags = args.count() >= 3 ? args[2].toInt() : 0;
    int rc = zoo_acreate(zh, args[0].toCString(), args[1].toCString(), args[1].length(), &ZOO_OPEN_ACL_UNSAFE, flags,
            my_string_completion_free_data, &result);
    if (rc)
    {
        retval = false;
        _this.set("errCode", rc);
    }
    else
    {
        zk_dispatch(_this, zh, result);
        retval = result.retval;
    }
}

PHPX_METHOD(zookeeper, set)
{
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    QueryResult result;
    long version = -1;
    if (args.count() > 2)
    {
        version = args[2].toInt();
    }

    int rc = zoo_aset(zh, args[0].toCString(), args[1].toCString(), args[1].length(), (int) version,
            my_silent_stat_completion, &result);

    if (rc)
    {
        retval = false;
        _this.set("errCode", rc);
    }
    else
    {
        zk_dispatch(_this, zh, result);
        retval = result.retval;
    }
}

PHPX_METHOD(zookeeper, delete)
{
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    QueryResult result;
    long version = -1;

    if (args.count() > 1)
    {
        version = args[2].toInt();
    }

    int rc = zoo_adelete(zh, args[0].toCString(), (int) version, my_void_completion, &result);
    if (rc)
    {
        retval = false;
        _this.set("errCode", rc);
    }
    else
    {
        zk_dispatch(_this, zh, result);
        retval = result.retval;
    }
}

PHPX_METHOD(zookeeper, getChildren)
{
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    QueryResult result;

    int rc = zoo_aget_children(zh, args[0].toCString(), 0, my_strings_completion, &result);
    if (rc)
    {
        retval = false;
        _this.set("errCode", rc);
    }
    else
    {
        zk_dispatch(_this, zh, result);
        retval = result.retval;
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

PHPX_METHOD(zookeeper, setLogStream)
{
    php_stream *stream;
    FILE *fp;
    zval *z_stream;

    if (!args.count())
    {
        _return_null: return;
    }

    if (!args[0].isResource())
    {
        error(E_WARNING, "expects parameter 1 to be resource");
        goto _return_null;
    }

    _this.set("logStream", args[0]);
    z_stream = args[0].ptr();

    stream = (php_stream *)zend_fetch_resource(Z_RES_P(z_stream), "stream", Z_RES_P(z_stream)->type);
    if (NULL == stream)
    {
        goto _return_null;
    }

    if (FAILURE == php_stream_cast(stream, PHP_STREAM_AS_STDIO, (void ** ) &fp, REPORT_ERRORS))
    {
        goto _return_null;
    }

    zoo_set_log_stream(fp);
}

PHPX_METHOD(zookeeper, setWatcher)
{
    if (args.count() == 0)
    {
        _return_null: return;
    }
    if (!args[0].isCallable())
    {
        error(E_WARNING, "expects parameter 1 to be callable");
        goto _return_null;
    }
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    _this.set("watcher", args[0]);
    zoo_set_watcher(zh, watch_func);
}

void zookeeper_dtor(zend_resource *res)
{
    zhandle_t *zh = static_cast<zhandle_t *>(res->ptr);
    if(zh != nullptr) {
        zookeeper_close(zh);
    }
}

PHPX_METHOD(zookeeper, setAcl)
{
    struct ACL_vector *zookeeper_acl;
    QueryResult result;
    long version = -1;
    //至少有一个参数
    if (args.count() < 1)
    {
        error(E_WARNING, "must be have one param");
        fail: retval = false;
        return;
    }

    //如果第一个参数不是一个字符串
    if (!(args[0].isString()))
    {
        error(E_WARNING, "first param must be string");
        goto fail;
    }


    //版本号必须是一个整数
    if (args.count() > 1 && args.count() == 2)
    {
        if (!(args[1].isArray()))
        {
            error(E_WARNING, "second param must be array");
            goto fail;
        }
    }

    if (args.count() == 3)
    {
        if (!(args[2].isInt()))
        {
            error(E_WARNING, "third param must be int");
            goto fail;
        }
        version = args[2].toInt();
    }

    Array acl_array(args[1]);
    zookeeper_acl = zKLib::convert_array_to_acl(&acl_array);
    if (!(zookeeper_acl))
    {
        error(E_WARNING, "acl array set error");
        goto fail;
    }

    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    int rc = zoo_aset_acl(zh,args[0].toCString(),version,zookeeper_acl,my_set_acl_completion,&result);
    zKLib::free_acl_struct(zookeeper_acl);
    if (rc)
    {
        retval = false;
        _this.set("errCode", rc);
    }
    else
    {
        zk_dispatch(_this, zh, result);
        retval = result.retval;
    }
}

PHPX_METHOD(zookeeper, watch)
{
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    QueryResult result;
    int rc = zoo_awget(zh, args[0].toCString(), watch_func, &_this, my_silent_data_completion, &result);
    if (rc)
    {
        retval = false;
    }
    else
    {
        zk_dispatch(_this, zh, result);
    }
}

PHPX_METHOD(zookeeper, watchChildren)
{
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    QueryResult result;

    int rc = zoo_awget_children(zh, args[0].toCString(), watch_func, &_this, my_strings_completion, &result);
    if (rc)
    {
        retval = false;
    }
    else
    {
        zk_dispatch(_this, zh, result);
    }
}

PHPX_METHOD(zookeeper, waitEvent)
{
    zhandle_t *zh = _this.oGet<zhandle_t>("handle", "zhandle_t");
    QueryResult result;

    int fd, rc, events = ZOOKEEPER_READ;
    struct timeval tv;

    while (true)
    {
        int rc = zookeeper_interest(zh, &fd, &events, &tv);
        if (rc)
        {
            _error: _this.set("errCode", rc);
            result.retval = false;
            return;
        }
        if (swoole_coroutine_socket_wait_event(fd, 512, 1) < 0)
        {
            result.retval = false;
            return;
        }
        rc = zookeeper_process(zh, events);
        if (rc)
        {
            goto _error;
        }
    }
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
        c->addProperty("logStream", Variant(), PRIVATE);
        c->addProperty("watcher", Variant(), PRIVATE);

        c->addConstant("PERM_READ", ZOO_PERM_READ);
        c->addConstant("PERM_WRITE", ZOO_PERM_WRITE);
        c->addConstant("PERM_ALL", ZOO_PERM_ALL);
        c->addConstant("PERM_ADMIN", ZOO_PERM_ADMIN);
        c->addConstant("PERM_CREATE", ZOO_PERM_CREATE);
        c->addConstant("PERM_DELETE", ZOO_PERM_DELETE);

        c->addMethod(PHPX_ME(zookeeper, __construct));
        c->addMethod(PHPX_ME(zookeeper, create));
        c->addMethod(PHPX_ME(zookeeper, addAuth));
        c->addMethod(PHPX_ME(zookeeper, set));
        c->addMethod(PHPX_ME(zookeeper, get));
        c->addMethod(PHPX_ME(zookeeper, exists));
        c->addMethod(PHPX_ME(zookeeper, delete));
        c->addMethod(PHPX_ME(zookeeper, setAcl));
        c->addMethod(PHPX_ME(zookeeper, getAcl));
        c->addMethod(PHPX_ME(zookeeper, getChildren));
        c->addMethod(PHPX_ME(zookeeper, watch));
        c->addMethod(PHPX_ME(zookeeper, watchChildren));
        c->addMethod(PHPX_ME(zookeeper, waitEvent));
        c->addMethod(PHPX_ME(zookeeper, setDebugLevel), STATIC);
        c->addMethod(PHPX_ME(zookeeper, getState));
        c->addMethod(PHPX_ME(zookeeper, getClientId));
        c->addMethod(PHPX_ME(zookeeper, setDeterministicConnOrder), PUBLIC, new ArgInfo(1));
        c->addMethod(PHPX_ME(zookeeper, setLogStream));
        c->addMethod(PHPX_ME(zookeeper, setWatcher));

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
