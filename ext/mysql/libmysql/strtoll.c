/* Copyright Abandoned 1996 TCX DataKonsult AB & Monty Program KB & Detron HB 
This file is public domain and comes with NO WARRANTY of any kind */

/* This is defines strtoll() if neaded */

#include <global.h>
#include <m_string.h>
#if !defined(HAVE_STRTOULL) && defined(HAVE_LONG_LONG)
#define MYSQL_LONGLONG
#include "strto.c"
#endif
