/*
   +----------------------------------------------------------------------+
   | PHP version 4.0                                                      |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997, 1998, 1999, 2000 The PHP Group                   |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.02 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available at through the world-wide-web at                           |
   | http://www.php.net/license/2_02.txt.                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Brian Schaffner <brian@tool.net>                            |
   |          Shane Caraveo <shane@caraveo.com>                           |
   |          Zeev Suraski <zeev@zend.com>                                |
   +----------------------------------------------------------------------+
 */

#include "php.h"
#include "dl.h"
#include "php_globals.h"
#include "ext/standard/info.h"

#ifdef HAVE_LIBDL
#include <stdlib.h>
#include <stdio.h>

#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#ifdef PHP_WIN32
#include "win32/param.h"
#include "win32/winutil.h"
#else
#include <sys/param.h>
#endif

#endif

/* {{{ proto int dl(string extension_filename)
   Load a PHP extension at runtime */
PHP_FUNCTION(dl)
{
	pval **file;
	PLS_FETCH();

	/* obtain arguments */
	if (ARG_COUNT(ht) != 1 || zend_get_parameters_ex(1, &file) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	convert_to_string_ex(file);

	if (!PG(enable_dl)) {
		php_error(E_ERROR, "Dynamically loaded extentions aren't enabled.");
	} else if (PG(safe_mode)) {
		php_error(E_ERROR, "Dynamically loaded extensions aren't allowed when running in SAFE MODE.");
	} else {
		php_dl(*file,MODULE_TEMPORARY,return_value);
	}
}

/* }}} */


#ifdef HAVE_LIBDL

#ifdef ZTS
#define USING_ZTS 1
#else
#define USING_ZTS 0
#endif

void php_dl(pval *file,int type,pval *return_value)
{
	void *handle;
	char *libpath;
	zend_module_entry *module_entry,*tmp;
	zend_module_entry *(*get_module)(void);
	PLS_FETCH();
	ELS_FETCH();

	if (PG(extension_dir) && PG(extension_dir)[0]){
		int extension_dir_len = strlen(PG(extension_dir));

		libpath = emalloc(extension_dir_len+file->value.str.len+2);

		sprintf(libpath,"%s/%s",PG(extension_dir),file->value.str.val);
	} else {
		libpath = estrndup(file->value.str.val, file->value.str.len);
	}

	/* load dynamic symbol */
	handle = DL_LOAD(libpath);
	if (!handle) {
		int error_type;

		if (type==MODULE_TEMPORARY) {
			error_type = E_ERROR;
		} else {
			error_type = E_CORE_ERROR;
		}
#ifdef PHP_WIN32
		php_error(error_type,"Unable to load dynamic library '%s'<br>\n%s",libpath,php_win_err());
#else
		php_error(error_type,"Unable to load dynamic library '%s' - %s",libpath,dlerror());
#endif

		efree(libpath);

		RETURN_FALSE;
	}

	efree(libpath);

	
	get_module = (zend_module_entry *(*)(void)) DL_FETCH_SYMBOL(handle, "get_module");

	/*
	 * some OS prepend _ to symbol names while their dynamic linker
	 * does not do that automatically. Thus we check manually for
	 * _get_module.
	 */

	if (!get_module)
		get_module = (zend_module_entry *(*)(void)) DL_FETCH_SYMBOL(handle, "_get_module");

	if (!get_module) {
		DL_UNLOAD(handle);
		php_error(E_CORE_WARNING,"Invalid library (maybe not a PHP library) '%s' ",file->value.str.val);
		RETURN_FALSE;
	}
	module_entry = get_module();
	if ((module_entry->zend_debug != ZEND_DEBUG) || (module_entry->zts != USING_ZTS)
		|| (module_entry->zend_api != ZEND_MODULE_API_NO)) {
		php_error(E_CORE_WARNING,
					"%s: Unable to initialize module\n"
					"Module compiled with debug=%d, thread-safety=%d module API=%d\n"
					"PHP compiled with debug=%d, thread-safety=%d module API=%d\n"
					"These options need to match\n",
					module_entry->name, module_entry->zend_debug, module_entry->zts, module_entry->zend_api,
					ZEND_DEBUG, USING_ZTS, ZEND_MODULE_API_NO);
		DL_UNLOAD(handle);
		RETURN_FALSE;
	}
	module_entry->type = type;
	module_entry->module_number = zend_next_free_module();
	if (module_entry->module_startup_func) {
		if (module_entry->module_startup_func(type, module_entry->module_number ELS_CC)==FAILURE) {
			php_error(E_CORE_WARNING,"%s:  Unable to initialize module",module_entry->name);
			DL_UNLOAD(handle);
			RETURN_FALSE;
		}
	}
	zend_register_module(module_entry);

	if ((type == MODULE_TEMPORARY) && module_entry->request_startup_func) {
		if (module_entry->request_startup_func(type, module_entry->module_number ELS_CC)) {
			php_error(E_CORE_WARNING,"%s:  Unable to initialize module",module_entry->name);
			DL_UNLOAD(handle);
			RETURN_FALSE;
		}
	}
	
	/* update the .request_started property... */
	if (zend_hash_find(&module_registry,module_entry->name,strlen(module_entry->name)+1,(void **) &tmp)==FAILURE) {
		php_error(E_ERROR,"%s:  Loaded module got lost",module_entry->name);
		RETURN_FALSE;
	}
	tmp->handle = handle;
	
	RETURN_TRUE;
}


PHP_MINFO_FUNCTION(dl)
{
        php_info_print_table_row(2, "Dynamic Library Support", "enabled");
}

#else

void php_dl(pval *file,int type,pval *return_value)
{
	php_error(E_WARNING,"Cannot dynamically load %s - dynamic modules are not supported",file->value.str.val);
	RETURN_FALSE;
}

PHP_MINFO_FUNCTION(dl)
{
	PUTS("Dynamic Library support not available<br>.\n");
}

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
