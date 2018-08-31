#include "phpx.h"
#include "zookeeper.h"
#include <iostream>

using namespace php;
using namespace std;

PHPX_FUNCTION(zookeeper_hello)
{
    echo("argc=%d\n", args.count());
    retval = "hello world\n";
}

PHPX_METHOD(zookeeper, __construct)
{
    auto host = args[0];
    long recv_timeout = args[1].toInt();
    echo("init\n");
    zhandle_t *zk = zookeeper_init(host.toCString(), NULL, recv_timeout, 0, NULL, 0);
    char buf[1024];
    int len = sizeof(buf);
    struct Stat stat;
    echo("get\n");
    zoo_get(zk, "/test", 0, buf, &len, &stat);
    echo("end\n");
}

PHPX_EXTENSION()
{
    Extension *ext = new Extension("zookeeper", "0.0.1");

    ext->onStart = [ext]() noexcept {
        ext->registerConstant("QUEUE_VERSION", 1001);
        Class *c = new Class("zookeeper");
        c->addMethod(PHPX_ME(zookeeper, __construct));
        ext->registerClass(c);
    };

    //extension->onShutdown = [extension]() noexcept {
    //};

    //extension->onBeforeRequest = [extension]() noexcept {
    //    cout << extension->name << "beforeRequest" << endl;
    //};

    //extension->onAfterRequest = [extension]() noexcept {
    //    cout << extension->name << "afterRequest" << endl;
    //};

    ext->require("swoole");

    ext->info({"zookeeper support", "enabled"},
                    {
                        {"version", ext->version},
                        {"date", "2018-08-31"},
                    });
    ext->registerFunction(PHPX_FN(zookeeper_hello));

    return ext;
}
