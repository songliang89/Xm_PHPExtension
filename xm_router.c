#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h" /* for zend_alter_ini_entry */
#include "Zend/zend_interfaces.h" /* for zend_call_method_with_* */

#include "php_xm.h"

#include "xm_request.h"
#include "xm_router.h"

zend_class_entry *xm_router_ce;

zval * xm_router_parse_parameters(char *uri TSRMLS_DC) {
	char *key, *ptrptr, *tmp, *value;
	zval *params, *val;
	uint key_len;

	MAKE_STD_ZVAL(params);
	array_init(params);

	tmp = estrdup(uri);
	key = php_strtok_r(tmp, XM_ROUTER_URL_DELIMIETER, &ptrptr);
	while (key) {
		key_len = strlen(key);
		if (key_len) {
			MAKE_STD_ZVAL(val);
			value = php_strtok_r(NULL, XM_ROUTER_URL_DELIMIETER, &ptrptr);
			if (value && strlen(value)) {
				ZVAL_STRING(val, value, 1);
			} else {
				ZVAL_NULL(val);
			}
            php_printf("%s<br>", key);
            php_printf("%s<br>", Z_STRVAL_P(val));
            zend_hash_update(Z_ARRVAL_P(params), key, key_len + 1, (void **)&val, sizeof(zval *), NULL);
		}

		key = php_strtok_r(NULL, XM_ROUTER_URL_DELIMIETER, &ptrptr);
	}

	efree(tmp);
	return params;
}

int xm_router_pathinfo(zval *request, char *req_uri, int req_uri_len TSRMLS_DC){
	zval *params;
	char *module = NULL, *controller = NULL, *action = NULL, *rest = NULL;

	do {
#define strip_slashs(p) while (*p == ' ' || *p == '/') { ++p; }
		char *s, *p;
		char *uri;

		if (req_uri_len == 0
				|| (req_uri_len == 1 && *req_uri == '/')) {
			break;
		}

		uri = req_uri;
		s = p = uri;

		if (req_uri_len) {
			char *q = req_uri + req_uri_len - 1;
			while (q > req_uri && (*q == ' ' || *q == '/')) {
				*q-- = '\0';
			}
		}

		strip_slashs(p);

		if ((s = strstr(p, "/")) != NULL) {
			controller = estrndup(p, s - p);
			p  = s + 1;
		}

		strip_slashs(p);
		if ((s = strstr(p, "/")) != NULL) {
			action = estrndup(p, s - p);
			p  = s + 1;
		}

		strip_slashs(p);
		if (*p != '\0') {
			do {
				if (!controller) {
					controller = estrdup(p);
					break;
				}

				if (!action) {
					action = estrdup(p);
					break;
				}

				rest = estrdup(p);
			} while (0);
		}
	} while (0);

    zend_update_property_string(xm_request_ce, request, ZEND_STRL(XM_REQUEST_PROPERTY_NAME_CONTROLLER), controller TSRMLS_CC);
    zend_update_property_string(xm_request_ce, request, ZEND_STRL(XM_REQUEST_PROPERTY_NAME_ACTION), action TSRMLS_CC);

    if (rest) {
        params = xm_router_parse_parameters(rest TSRMLS_CC);
        (void)xm_request_set_params_multi(request, params TSRMLS_CC);
        zval_ptr_dtor(&params);
        efree(rest);
    }   
	efree(controller);
	efree(action);

	return 1;
}

zval *xm_route_instance(zval *this_ptr TSRMLS_DC) {
    zval *instance;
    if (this_ptr) {
        instance = this_ptr;
    } else {
        MAKE_STD_ZVAL(instance);
        object_init_ex(instance, xm_router_ce);
    }
    
    return instance;     
}


ZEND_METHOD(xm_router, __construct){
    php_printf("Xm_Router_Abstract::__construct");
}

static zend_function_entry xm_router_methods[]={
    ZEND_ME(xm_router,    __construct,     NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    {NULL,NULL,NULL}
};

XM_STARTUP_FUNCTION(router)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Xm_Router", xm_router_methods);
    xm_router_ce = zend_register_internal_class(&ce TSRMLS_CC);
    
    return SUCCESS;
}
