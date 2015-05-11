/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_XM_H
#define PHP_XM_H

extern zend_module_entry xm_module_entry;
#define phpext_xm_ptr &xm_module_entry

#define PHP_XM_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_XM_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_XM_API __attribute__ ((visibility("default")))
#else
#	define PHP_XM_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define XM_STARTUP_FUNCTION(module)    ZEND_MINIT_FUNCTION(xm_##module)
#define XM_STARTUP(module)             ZEND_MODULE_STARTUP_N(xm_##module)(INIT_FUNC_ARGS_PASSTHRU)

PHP_MINIT_FUNCTION(xm);
PHP_MSHUTDOWN_FUNCTION(xm);
PHP_RINIT_FUNCTION(xm);
PHP_RSHUTDOWN_FUNCTION(xm);
PHP_MINFO_FUNCTION(xm);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(xm)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(xm)
*/

/* In every utility function you add that needs to use variables 
   in php_xm_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as XM_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define XM_G(v) TSRMG(xm_globals_id, zend_xm_globals *, v)
#else
#define XM_G(v) (xm_globals.v)
#endif

#endif	/* PHP_XM_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
