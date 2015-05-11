#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h" /* for zend_alter_ini_entry */
#include "Zend/zend_interfaces.h" /* for zend_call_method_with_* */

#include "ext/standard/php_smart_str.h"

#include "php_xm.h"
#include "xm_loader.h"

zend_class_entry *xm_loader_ce;

ZEND_BEGIN_ARG_INFO_EX(xm_loader_autoloader_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, class_name)
ZEND_END_ARG_INFO()


ZEND_METHOD(xm_loader, __construct){
    //php_printf("Xm_Loader::__construct<br>");  
}

int xm_loader_register(zval *loader TSRMLS_DC) {

    zval *autoload, *method, *function, *ret = NULL;
    zval **params[1] = {&autoload};

    MAKE_STD_ZVAL(autoload);
    array_init(autoload);

    MAKE_STD_ZVAL(method);
    ZVAL_STRING(method, XM_AUTOLOAD_FUNC_NAME, 1);

    zend_hash_next_index_insert(Z_ARRVAL_P(autoload), &loader, sizeof(zval *), NULL);
    zend_hash_next_index_insert(Z_ARRVAL_P(autoload), &method, sizeof(zval *), NULL);

    MAKE_STD_ZVAL(function);
    ZVAL_STRING(function, XM_SPL_AUTOLOAD_REGISTER_NAME, 0);
    
    do {
        zend_fcall_info fci = {
            sizeof(fci),
            EG(function_table),
            function,
            NULL,
            &ret,
            1,
            (zval ***)params,
            NULL,
            1
        };
        if (zend_call_function(&fci, NULL TSRMLS_CC) == FAILURE) {
            if (ret) {
                zval_ptr_dtor(&ret);
            }
            efree(function);
            zval_ptr_dtor(&autoload);
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to register autoload function %s", XM_AUTOLOAD_FUNC_NAME);
            return 0;
        } else {
            //php_printf("xm_loader_register<br>");
        }
        
        if (ret) {
            zval_ptr_dtor(&ret);
        }
        efree(function);
        zval_ptr_dtor(&autoload);
        zval_ptr_dtor(&method);
    } while (0);
    return 1; 
}

int xm_loader_import(char *path, int len, int use_path TSRMLS_DC) {
    zend_file_handle file_handle;
    zend_op_array   *op_array;

    file_handle.filename = path;
    file_handle.free_filename = 0;
    file_handle.type = ZEND_HANDLE_FILENAME;
    file_handle.opened_path = NULL;
    file_handle.handle.fp = NULL;
    
    op_array = zend_compile_file(&file_handle, ZEND_INCLUDE TSRMLS_CC);

    if (op_array && file_handle.handle.stream.handle) {
        int dummy = 1;

        if (!file_handle.opened_path) {
            file_handle.opened_path = path;
        }
        zend_hash_add(&EG(included_files), file_handle.opened_path, strlen(file_handle.opened_path)+1, (void *)&dummy, sizeof(int), NULL);
    }
    zend_destroy_file_handle(&file_handle TSRMLS_CC);

    if (op_array) {

#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION > 2)) || (PHP_MAJOR_VERSION > 5)
        if (!EG(active_symbol_table)) {
            zend_rebuild_symbol_table(TSRMLS_C);
        }
#endif
        zend_execute(op_array TSRMLS_CC);

        destroy_op_array(op_array TSRMLS_CC);
        efree(op_array);
        return 1;
    }
    return 0;
}

int xm_internal_autoload(char *class_name, uint class_name_len, char *directory TSRMLS_DC) {
    int status;
    

    smart_str buf = {0};

    //smart_str_appendl(&buf, "/var/www/baidu/", 15);
    
    smart_str_appendl(&buf, directory, strlen(directory));

    smart_str_appendl(&buf, class_name, class_name_len);

    smart_str_appendc(&buf, '.');

    smart_str_appendl(&buf, "php", 3);

    smart_str_0(&buf);

    //php_printf("<br>Autoloader Success::%s<br>", buf.c);

    status = xm_loader_import(buf.c, buf.len, 0 TSRMLS_CC);
    
    smart_str_free(&buf);
    
    if (!status)
        return 0;
    return 1;    
}

zval *xm_loader_instance(zval *this_ptr TSRMLS_DC) {
    zval *instance;
    if (this_ptr) {
        instance = this_ptr;
    } else {
        MAKE_STD_ZVAL(instance);
        object_init_ex(instance, xm_loader_ce);
    }
    
    if (!xm_loader_register(instance TSRMLS_CC)) {
        return NULL;
    }
    
    return instance;     
}

ZEND_METHOD(xm_loader , autoload){
    char *class_name;
    uint class_name_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &class_name, &class_name_len) == FAILURE) {
        return;
    } 

    php_printf("%s<br>", class_name);
   
    char * directory = "/var/www/baidu/libs/";
    xm_internal_autoload(class_name, class_name_len, directory);
    
    RETURN_FALSE;
}

static zend_function_entry xm_loader_methods[]={
    ZEND_ME(xm_loader,    __construct,     NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    ZEND_ME(xm_loader,    autoload,         xm_loader_autoloader_arginfo,      ZEND_ACC_PUBLIC)
    {NULL,NULL,NULL}
};

XM_STARTUP_FUNCTION(loader)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "xm_loader", xm_loader_methods);
    xm_loader_ce = zend_register_internal_class(&ce TSRMLS_CC);
    return SUCCESS;
}
