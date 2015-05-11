#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h" /* for zend_alter_ini_entry */
#include "Zend/zend_interfaces.h" /* for zend_call_method_with_* */

#include "php_xm.h"
#include "xm_application.h"
#include "xm_loader.h"
#include "xm_request.h"
#include "xm_controller.h"
#include "xm_router.h"

zend_class_entry *xm_application_ce;

ZEND_METHOD(xm_application, __construct){
    
    //autoload class register    
    xm_loader_instance(NULL TSRMLS_CC);
}

ZEND_METHOD(xm_application , run){
    
    zval * request, *controller, *action, *uri, *retval;
    
    MAKE_STD_ZVAL(uri);
    MAKE_STD_ZVAL(controller);
    MAKE_STD_ZVAL(action);
    MAKE_STD_ZVAL(retval);
    
    request = xm_request_instance(NULL TSRMLS_CC);
    uri = xm_request_query(TRACK_VARS_SERVER, ZEND_STRL("REQUEST_URI") TSRMLS_CC);
    xm_router_pathinfo(request, Z_STRVAL_P(uri), Z_STRLEN_P(uri)); 
    controller = zend_read_property(xm_request_ce, request, ZEND_STRL(XM_REQUEST_PROPERTY_NAME_CONTROLLER), 1 TSRMLS_CC); 
    action     = zend_read_property(xm_request_ce, request, ZEND_STRL(XM_REQUEST_PROPERTY_NAME_ACTION),     1 TSRMLS_CC); 

    int class_len = 0;
    char *class = NULL;
    class_len = spprintf(&class, 0, "%s%s",  Z_STRVAL_P(controller), "Controller");
    class[0] = toupper(class[0]);
    
    char * directory = "/var/www/baidu/app/controller/";
    xm_internal_autoload(class, class_len, directory);

    char *class_lowercase = NULL;
    class_lowercase = zend_str_tolower_dup(class, class_len); 

    zend_class_entry **ce   = NULL;
    if(zend_hash_find(EG(class_table), class_lowercase, class_len + 1, (void **) &ce) != SUCCESS) {
        zend_error(E_ERROR, "Can't get class!");
    }
    zval *icontroller;
    object_init_ex(icontroller, *ce);

    call_user_function(NULL, &icontroller, action, retval, 0, NULL TSRMLS_CC);
   
    zval_ptr_dtor(&controller);
    zval_ptr_dtor(&action);
    zval_ptr_dtor(&uri);
}

ZEND_METHOD(xm_application , bootstrap){
    
    zend_class_entry    **ce;  
    zval  *self = getThis();    
    zval            *bootstrap;
    HashTable       *methods;

    //Load bootstrap class.
    char * directory = "/var/www/baidu/libs/";
    xm_internal_autoload(XM_DEFAULT_BOOTSTRAP, strlen(XM_DEFAULT_BOOTSTRAP), directory);

    if (zend_hash_find(EG(class_table), XM_DEFAULT_BOOTSTRAP_LOWER, XM_DEFAULT_BOOTSTRAP_LEN, (void **) &ce) != SUCCESS) {
        zend_error(E_ERROR,"Can't get class bootstrap!");
    }
    
    MAKE_STD_ZVAL(bootstrap);
    object_init_ex(bootstrap, *ce);

    methods = &((*ce)->function_table);
    for(zend_hash_internal_pointer_reset(methods);zend_hash_has_more_elements(methods) == SUCCESS;zend_hash_move_forward(methods)) {
        char *func;
        uint len;
        ulong idx;

        zend_hash_get_current_key_ex(methods, &func, &len, &idx, 0, NULL);
        if (strncasecmp(func, XM_BOOTSTRAP_INITFUNC_PREFIX, sizeof(XM_BOOTSTRAP_INITFUNC_PREFIX)-1)) {
            continue;
        }

        zend_call_method(&bootstrap, *ce, NULL, func, len - 1, NULL, 0, NULL, NULL TSRMLS_CC);
         
        if (EG(exception)) {
            zval_ptr_dtor(&bootstrap);
            RETURN_FALSE;
        }
    }

    zval_ptr_dtor(&bootstrap);
    
    RETVAL_ZVAL(self, 1, 0);
}


static zend_function_entry xm_application_methods[]={
    ZEND_ME(xm_application,    bootstrap,       NULL,   ZEND_ACC_PUBLIC)
    ZEND_ME(xm_application,    run,             NULL,   ZEND_ACC_PUBLIC)
    ZEND_ME(xm_application,    __construct,     NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    {NULL,NULL,NULL}
};

XM_STARTUP_FUNCTION(application)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "xm_application", xm_application_methods);
    xm_application_ce = zend_register_internal_class(&ce TSRMLS_CC);
    
    return SUCCESS;
}
