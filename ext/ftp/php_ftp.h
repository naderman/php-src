/*
   +----------------------------------------------------------------------+
   | PHP HTML Embedded Scripting Language Version 3.0                     |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-1999 PHP Development Team (See Credits file)      |
   +----------------------------------------------------------------------+
   | This program is free software; you can redistribute it and/or modify |
   | it under the terms of one of the following licenses:                 |
   |                                                                      |
   |  A) the GNU General Public License as published by the Free Software |
   |     Foundation; either version 2 of the License, or (at your option) |
   |     any later version.                                               |
   |                                                                      |
   |  B) the PHP License as published by the PHP Development Team and     |
   |     included in the distribution in the file: LICENSE                |
   |                                                                      |
   | This program is distributed in the hope that it will be useful,      |
   | but WITHOUT ANY WARRANTY; without even the implied warranty of       |
   | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        |
   | GNU General Public License for more details.                         |
   |                                                                      |
   | You should have received a copy of both licenses referred to here.   |
   | If you did not, or have any questions about PHP licensing, please    |
   | contact core@php.net.                                                |
   +----------------------------------------------------------------------+
   | Authors:                                                             |
   |          Andrew Skalski      <askalski@chek.com>                     |
   +----------------------------------------------------------------------+
 */

/* $Id$ */

#ifndef	_INCLUDED_FTP_H
#define	_INCLUDED_FTP_H

#if COMPILE_DL
#undef HAVE_FTP
#define HAVE_FTP 1
#endif

#if HAVE_FTP

extern php3_module_entry php3_ftp_module_entry;
#define php3_ftp_module_ptr &php3_ftp_module_entry

#ifdef ZEND_VERSION
PHP_MINIT_FUNCTION(ftp);
#else
int php3_minit_ftp(INIT_FUNC_ARGS);
#endif

PHP_FUNCTION(ftp_connect);
PHP_FUNCTION(ftp_login);
PHP_FUNCTION(ftp_pwd);
PHP_FUNCTION(ftp_cdup);
PHP_FUNCTION(ftp_chdir);
PHP_FUNCTION(ftp_mkdir);
PHP_FUNCTION(ftp_rmdir);
PHP_FUNCTION(ftp_nlist);
PHP_FUNCTION(ftp_rawlist);
PHP_FUNCTION(ftp_systype);
PHP_FUNCTION(ftp_get);
PHP_FUNCTION(ftp_put);
PHP_FUNCTION(ftp_quit);

#define phpext_ftp_ptr php3_ftp_module_ptr

#else
#define php3_ftp_module_ptr NULL
#endif	/* HAVE_FTP */

#endif
