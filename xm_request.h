#ifndef XM_REQUEST_H
#define XM_REQUEST_H


#define XM_REQUEST_PROPERTY_NAME_CONTROLLER    "controller"
#define XM_REQUEST_PROPERTY_NAME_ACTION        "action"
#define XM_REQUEST_PROPERTY_NAME_PARAMS        "params"

extern zend_class_entry *xm_request_ce;
zval * xm_request_instance(zval *this_ptr TSRMLS_DC);
zval * xm_request_query(uint type, char * name, uint len TSRMLS_DC);
int xm_request_set_params_multi(zval *request, zval *values TSRMLS_DC);

XM_STARTUP_FUNCTION(request);
#endif
