dnl $Id$

PHP_ARG_WITH(mm,for mm support,
[  --with-mm[=DIR]         Include mm support for session storage])

PHP_ARG_ENABLE(session, whether to enable session support,
[  --disable-session       Disable session support], yes)

if test "$PHP_MM" != "no"; then
  for i in /usr/local /usr $PHP_MM; do
    if test -f "$i/include/mm.h"; then
      MM_DIR=$i
    fi
  done

  if test -z "$MM_DIR" ; then
    AC_MSG_ERROR(cannot find mm library)
  fi
  
  PHP_ADD_LIBRARY_WITH_PATH(mm, $MM_DIR/lib, SESSION_SHARED_LIBADD)
  PHP_ADD_INCLUDE($MM_DIR/include)
  AC_DEFINE(HAVE_LIBMM, 1, [Whether you have libmm])
  PHP_MODULE_PTR(phpext_ps_mm_ptr)
fi

if test "$PHP_SESSION" != "no"; then
  AC_CHECK_FUNCS(pread pwrite)
  PHP_MISSING_PWRITE_DECL
  PHP_MISSING_PREAD_DECL
  PHP_EXTENSION(session,$ext_shared)
  PHP_SUBST(SESSION_SHARED_LIBADD)
  AC_DEFINE(HAVE_PHP_SESSION,1,[ ])
fi
