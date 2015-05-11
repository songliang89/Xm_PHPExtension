#ifndef XM_LOADER_H
#define XM_LOADER_H


#define XM_SPL_AUTOLOAD_REGISTER_NAME      "spl_autoload_register"
#define XM_AUTOLOAD_FUNC_NAME              "autoload"


extern zend_class_entry *xm_loader_ce;

zval * xm_loader_instance(zval *this_ptr TSRMLS_DC);
int xm_loader_register(zval *loader TSRMLS_DC);

XM_STARTUP_FUNCTION(loader);
#endif
