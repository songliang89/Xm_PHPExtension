#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h" /* for zend_alter_ini_entry */
#include "Zend/zend_interfaces.h" /* for zend_call_method_with_* */

#include "php_xm.h"
#include "xm_controller.h"


zend_class_entry *xm_controller_ce;

ZEND_METHOD(xm_controller, __construct){
    php_printf("Xm_Controller_Abstract::__construct");
}

static zend_function_entry xm_controller_methods[]={
    ZEND_ME(xm_controller,    __construct,     NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    {NULL,NULL,NULL}
};

XM_STARTUP_FUNCTION(controller)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "xm_controller_abstract", xm_controller_methods);
    xm_controller_ce = zend_register_internal_class(&ce TSRMLS_CC);
    return SUCCESS;
}
