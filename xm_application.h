#ifndef XM_APPLICATION_H
#define XM_APPLICATION_H

#define XM_DEFAULT_BOOTSTRAP            "Bootstrap"
#define XM_DEFAULT_BOOTSTRAP_LOWER      "bootstrap"
#define XM_DEFAULT_BOOTSTRAP_LEN        10
#define XM_BOOTSTRAP_INITFUNC_PREFIX    "_init"


extern zend_class_entry *xm_application_ce;

XM_STARTUP_FUNCTION(application);
#endif
