dnl $Id$
dnl config.m4 for extension aho

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(aho, for aho support,
dnl Make sure that the comment is aligned:
dnl [  --with-aho             Include aho support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(aho, whether to enable aho support,
dnl Make sure that the comment is aligned:
dnl [  --enable-aho           Enable aho support])

if test "$PHP_AHO" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-aho -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/aho.h"  # you most likely want to change this
  dnl if test -r $PHP_AHO/$SEARCH_FOR; then # path given as parameter
  dnl   AHO_DIR=$PHP_AHO
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for aho files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       AHO_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$AHO_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the aho distribution])
  dnl fi

  dnl # --with-aho -> add include path
  dnl PHP_ADD_INCLUDE($AHO_DIR/include)

  dnl # --with-aho -> check for lib and symbol presence
  dnl LIBNAME=aho # you may want to change this
  dnl LIBSYMBOL=aho # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $AHO_DIR/$PHP_LIBDIR, AHO_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_AHOLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong aho lib version or lib not found])
  dnl ],[
  dnl   -L$AHO_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(AHO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(aho, aho.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
