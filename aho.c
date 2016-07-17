/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_aho.h"

ZEND_DECLARE_MODULE_GLOBALS(aho)

static int le_aho;

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("aho.enabled", "On",  PHP_INI_ALL, OnUpdateBool, enabled, zend_aho_globals, aho_globals)
    STD_PHP_INI_ENTRY("aho.dict",    NULL, PHP_INI_ALL, OnUpdateStringUnempty, dict, zend_aho_globals, aho_globals)
PHP_INI_END()
/* }}} */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string aho_hello(string msg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(aho_hello)
{
	char *msg = NULL;
	size_t msg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &msg, &msg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "hello %.78s and enabled is %d and dict is %s.", msg, AHO_G(enabled), AHO_G(dict));

	RETURN_STR(strg);
}
/* }}} */

/* {{{ php_aho_init_globals
 */
static void php_aho_init_globals(zend_aho_globals *aho_globals)
{
	aho_globals->enabled = 1;
	aho_globals->dict = NULL;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(aho)
{
	AHO_G(trie) = ac_trie_create();
	REGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(aho)
{
	ac_trie_release(AHO_G(trie));
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(aho)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "aho support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ aho_functions[]
 *
 * Every user visible function must have an entry in aho_functions[].
 */
const zend_function_entry aho_functions[] = {
	PHP_FE(aho_hello, NULL)
	PHP_FE_END	/* Must be the last line in aho_functions[] */
};
/* }}} */

/* {{{ aho_module_entry
 */
zend_module_entry aho_module_entry = {
	STANDARD_MODULE_HEADER,
	"aho",
	aho_functions,
	PHP_MINIT(aho),
	PHP_MSHUTDOWN(aho),
	NULL,
	NULL,
	PHP_MINFO(aho),
	PHP_AHO_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_AHO
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(aho)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
