dnl $Id$
dnl config.m4 for extension xm

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(xm, for xm support,
dnl Make sure that the comment is aligned:
dnl [  --with-xm             Include xm support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(xm, whether to enable xm support,
Make sure that the comment is aligned:
[  --enable-xm           Enable xm support])

if test "$PHP_XM" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-xm -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/xm.h"  # you most likely want to change this
  dnl if test -r $PHP_XM/$SEARCH_FOR; then # path given as parameter
  dnl   XM_DIR=$PHP_XM
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for xm files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       XM_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$XM_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the xm distribution])
  dnl fi

  dnl # --with-xm -> add include path
  dnl PHP_ADD_INCLUDE($XM_DIR/include)

  dnl # --with-xm -> check for lib and symbol presence
  dnl LIBNAME=xm # you may want to change this
  dnl LIBSYMBOL=xm # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $XM_DIR/$PHP_LIBDIR, XM_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_XMLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong xm lib version or lib not found])
  dnl ],[
  dnl   -L$XM_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(XM_SHARED_LIBADD)

  PHP_NEW_EXTENSION(xm, xm.c xm_application.c xm_loader.c xm_request.c xm_controller.c xm_router.c, $ext_shared)
fi
