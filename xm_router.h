#ifndef XM_ROUTER_H
#define XM_ROUTER_H


#define XM_ROUTER_URL_DELIMIETER    "/"

extern zend_class_entry *xm_router_ce;
zval * xm_router_instance(zval *this_ptr TSRMLS_DC);
zval * yaf_router_parse_parameters(char *uri TSRMLS_DC);
int xm_router_pathinfo(zval *request, char *req_uri, int req_uri_len TSRMLS_DC);

XM_STARTUP_FUNCTION(router);
#endif
