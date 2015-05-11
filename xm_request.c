#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h" /* for zend_alter_ini_entry */
#include "Zend/zend_interfaces.h" /* for zend_call_method_with_* */

#include "standard/php_string.h" /* for php_basename */
#include "Zend/zend_exceptions.h" /* for zend_exception_get_default */

#include "php_xm.h"
#include "xm_request.h"

zend_class_entry *xm_request_ce;

ZEND_BEGIN_ARG_INFO_EX(xm_request_void_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_METHOD(xm_request, __construct){
    php_printf("Xm_Request::__construct<br>");
}

zval *xm_request_instance(zval *this_ptr TSRMLS_DC) {
    zval *instance;
    if (this_ptr) {
        instance = this_ptr;
    } else {
        MAKE_STD_ZVAL(instance);
        object_init_ex(instance, xm_request_ce);
    }
    
    return instance;     
}

int xm_request_set_params_multi(zval *request, zval *values TSRMLS_DC) {
	
    zval *params = zend_read_property(xm_request_ce, request, ZEND_STRL(XM_REQUEST_PROPERTY_NAME_PARAMS), 1 TSRMLS_CC);
	
    if (values && Z_TYPE_P(values) == IS_ARRAY) {
        //zend_hash_copy(Z_ARRVAL_P(params), Z_ARRVAL_P(values), (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));
		return 1;
	}
	return 0;
}

zval * xm_request_query(uint type, char * name, uint len TSRMLS_DC) {  
    zval    **carrier, **ret;  
  
#if (PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION < 4)
	zend_bool 	jit_initialization = (PG(auto_globals_jit) && !PG(register_globals) && !PG(register_long_arrays));
#else
	zend_bool 	jit_initialization = PG(auto_globals_jit);
#endif
    
switch (type) {  
        case TRACK_VARS_POST:  
        case TRACK_VARS_GET:  
        case TRACK_VARS_FILES:  
        case TRACK_VARS_COOKIE:  
            carrier = &PG(http_globals)[type];  
            break;  
        case TRACK_VARS_ENV:  
            if (jit_initialization) {  
                zend_is_auto_global(ZEND_STRL("_ENV") TSRMLS_CC);  
            }  
            carrier = &PG(http_globals)[type];  
            break;  
        case TRACK_VARS_SERVER:  
            if (jit_initialization) {  
                zend_is_auto_global(ZEND_STRL("_SERVER") TSRMLS_CC);  
            }  
            carrier = &PG(http_globals)[type];  
            break;  
        case TRACK_VARS_REQUEST:  
            if (jit_initialization) {  
                zend_is_auto_global(ZEND_STRL("_REQUEST") TSRMLS_CC);  
            }  
            (void)zend_hash_find(&EG(symbol_table), ZEND_STRS("_REQUEST"), (void **)&carrier);  
            break;  
        default:  
            break;  
    }  
  
    if (!carrier || !(*carrier)) {  
        zval *empty;  
        MAKE_STD_ZVAL(empty);  
        ZVAL_NULL(empty);  
        return empty;  
    }  
  
    if (!len) {  
        Z_ADDREF_P(*carrier);  
        return *carrier;  
    }  
  
    if (zend_hash_find(Z_ARRVAL_PP(carrier), name, len + 1, (void **)&ret) == FAILURE) {  
        zval *empty;  
        MAKE_STD_ZVAL(empty);  
        ZVAL_NULL(empty);  
        return empty;  
    }  
  
    Z_ADDREF_P(*ret);  
    return *ret;  
}  

PHP_METHOD(xm_request, getParams) {
	zval *params = zend_read_property(xm_request_ce, getThis(), ZEND_STRL(XM_REQUEST_PROPERTY_NAME_PARAMS), 1 TSRMLS_CC);
	RETURN_ZVAL(params, 1, 0);
}

static zend_function_entry xm_request_methods[]={
    ZEND_ME(xm_request,    __construct,     NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    ZEND_ME(xm_request,    getParams, 		xm_request_void_arginfo, 		ZEND_ACC_PUBLIC)

    {NULL,NULL,NULL}
};

XM_STARTUP_FUNCTION(request)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "xm_request", xm_request_methods);
    xm_request_ce = zend_register_internal_class(&ce TSRMLS_CC);
    
    zend_declare_property_null(xm_request_ce, ZEND_STRL(XM_REQUEST_PROPERTY_NAME_CONTROLLER), ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_null(xm_request_ce, ZEND_STRL(XM_REQUEST_PROPERTY_NAME_ACTION),     ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_null(xm_request_ce, ZEND_STRL(XM_REQUEST_PROPERTY_NAME_PARAMS),  	ZEND_ACC_PROTECTED TSRMLS_CC);

    return SUCCESS;
}
