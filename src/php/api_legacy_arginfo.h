/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: f37245c53cb2a4fa610bc1bb6ae53b00473e9c1b */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Swoole_ZooKeeper___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, host)
	ZEND_ARG_INFO(0, timeout)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Swoole_ZooKeeper_create, 0, 0, 2)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Swoole_ZooKeeper_addAuth, 0, 0, 2)
	ZEND_ARG_INFO(0, scheme)
	ZEND_ARG_INFO(0, cert)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Swoole_ZooKeeper_set, 0, 0, 2)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, version)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Swoole_ZooKeeper_get, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, version)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Swoole_ZooKeeper_exists, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

#define arginfo_class_Swoole_ZooKeeper_delete arginfo_class_Swoole_ZooKeeper_get

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Swoole_ZooKeeper_setAcl, 0, 0, 3)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, acl)
	ZEND_ARG_INFO(0, version)
ZEND_END_ARG_INFO()

#define arginfo_class_Swoole_ZooKeeper_getAcl arginfo_class_Swoole_ZooKeeper_exists

#define arginfo_class_Swoole_ZooKeeper_getChildren arginfo_class_Swoole_ZooKeeper_exists

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Swoole_ZooKeeper_getClientId, 0, 0, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Swoole_ZooKeeper_watch arginfo_class_Swoole_ZooKeeper_exists

#define arginfo_class_Swoole_ZooKeeper_wait arginfo_class_Swoole_ZooKeeper_getClientId

#define arginfo_class_Swoole_ZooKeeper_watchChildren arginfo_class_Swoole_ZooKeeper_exists

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Swoole_ZooKeeper_setDebugLevel, 0, 0, 1)
	ZEND_ARG_INFO(0, level)
ZEND_END_ARG_INFO()

#define arginfo_class_Swoole_ZooKeeper_getState arginfo_class_Swoole_ZooKeeper_getClientId

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Swoole_ZooKeeper_setDeterministicConnOrder, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Swoole_ZooKeeper_setLogStream, 0, 0, 1)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Swoole_ZooKeeper_setWatcher, 0, 0, 1)
	ZEND_ARG_INFO(0, watcher)
ZEND_END_ARG_INFO()


ZEND_METHOD(Swoole_ZooKeeper, __construct);
ZEND_METHOD(Swoole_ZooKeeper, create);
ZEND_METHOD(Swoole_ZooKeeper, addAuth);
ZEND_METHOD(Swoole_ZooKeeper, set);
ZEND_METHOD(Swoole_ZooKeeper, get);
ZEND_METHOD(Swoole_ZooKeeper, exists);
ZEND_METHOD(Swoole_ZooKeeper, delete);
ZEND_METHOD(Swoole_ZooKeeper, setAcl);
ZEND_METHOD(Swoole_ZooKeeper, getAcl);
ZEND_METHOD(Swoole_ZooKeeper, getChildren);
ZEND_METHOD(Swoole_ZooKeeper, getClientId);
ZEND_METHOD(Swoole_ZooKeeper, watch);
ZEND_METHOD(Swoole_ZooKeeper, wait);
ZEND_METHOD(Swoole_ZooKeeper, watchChildren);
ZEND_METHOD(Swoole_ZooKeeper, setDebugLevel);
ZEND_METHOD(Swoole_ZooKeeper, getState);
ZEND_METHOD(Swoole_ZooKeeper, setDeterministicConnOrder);
ZEND_METHOD(Swoole_ZooKeeper, setLogStream);
ZEND_METHOD(Swoole_ZooKeeper, setWatcher);


static const zend_function_entry class_Swoole_ZooKeeper_methods[] = {
	ZEND_ME(Swoole_ZooKeeper, __construct, arginfo_class_Swoole_ZooKeeper___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, create, arginfo_class_Swoole_ZooKeeper_create, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, addAuth, arginfo_class_Swoole_ZooKeeper_addAuth, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, set, arginfo_class_Swoole_ZooKeeper_set, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, get, arginfo_class_Swoole_ZooKeeper_get, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, exists, arginfo_class_Swoole_ZooKeeper_exists, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, delete, arginfo_class_Swoole_ZooKeeper_delete, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, setAcl, arginfo_class_Swoole_ZooKeeper_setAcl, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, getAcl, arginfo_class_Swoole_ZooKeeper_getAcl, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, getChildren, arginfo_class_Swoole_ZooKeeper_getChildren, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, getClientId, arginfo_class_Swoole_ZooKeeper_getClientId, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, watch, arginfo_class_Swoole_ZooKeeper_watch, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, wait, arginfo_class_Swoole_ZooKeeper_wait, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, watchChildren, arginfo_class_Swoole_ZooKeeper_watchChildren, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, setDebugLevel, arginfo_class_Swoole_ZooKeeper_setDebugLevel, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Swoole_ZooKeeper, getState, arginfo_class_Swoole_ZooKeeper_getState, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, setDeterministicConnOrder, arginfo_class_Swoole_ZooKeeper_setDeterministicConnOrder, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, setLogStream, arginfo_class_Swoole_ZooKeeper_setLogStream, ZEND_ACC_PUBLIC)
	ZEND_ME(Swoole_ZooKeeper, setWatcher, arginfo_class_Swoole_ZooKeeper_setWatcher, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
