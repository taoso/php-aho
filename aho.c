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
  | Author: php@lvht.net                                                 |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>
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
/* {{{ proto string aho_match(string subject)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(aho_match)
{
	zend_string *subject;
	zval *matches = NULL;

#define FAST_ZPP 1
#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|z/", &subject, &matches) == FAILURE) {
		RETURN_FALSE;
	}
#else
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(subject)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_EX(matches, 0, 1)
	ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);
	printf("foo\n");
#endif

	AC_TEXT_t chunk;
	chunk.astring = ZSTR_VAL(subject);
	chunk.length = ZSTR_LEN(subject);
	ac_trie_settext(AHO_G(trie), &chunk, 0);

	if (matches) {
		array_init(matches);
	}

	AC_MATCH_t match;
	zval *patterns;
	zval tmp;
	int count = 0;
	while ((match = ac_trie_findnext(AHO_G(trie))).size) {
		count += match.size;
		if (!matches) {
			continue;
		}
		patterns = (zval *)safe_emalloc(match.size, sizeof(zval), 0);
		for (int i = 0; i < match.size; i++) {
			array_init_size(&patterns[i], 3);
			AC_PATTERN_t *pp = &match.patterns[i];

			ZVAL_LONG(&tmp, match.position - pp->ptext.length);
			zend_hash_next_index_insert_new(Z_ARRVAL(patterns[i]), &tmp);

			ZVAL_STRINGL(&tmp, pp->ptext.astring, pp->ptext.length);
			zend_hash_next_index_insert_new(Z_ARRVAL(patterns[i]), &tmp);

			if (pp->id.u.stringy) {
				ZVAL_STRINGL(&tmp, pp->id.u.stringy, strlen(pp->id.u.stringy));
			} else {
				ZVAL_NULL(&tmp);
			}

			zend_hash_next_index_insert_new(Z_ARRVAL(patterns[i]), &tmp);

			zend_hash_next_index_insert_new(Z_ARRVAL(*matches), &patterns[i]);
		}
	}
	RETURN_LONG(count);
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
	char *line = NULL;
	size_t linecap = 0;
	ssize_t linelen;

	REGISTER_INI_ENTRIES();

	FILE *fp = fopen(AHO_G(dict), "r");
	if (!fp) {
		return FAILURE;
	}

	AHO_G(trie) = ac_trie_create();

	char *delim = NULL;
	AC_PATTERN_t patt;
	patt.rtext.astring = NULL;
	patt.rtext.length = 0;
	char *astring = NULL;
	char *tstring = "";

	while ((linelen = getline(&line, &linecap, fp)) > 0) {
		if (line[linelen - 1] == '\n') line[--linelen] = '\0';
		if (line[linelen - 1] == '\r') line[--linelen] = '\0';
		delim = strrchr(line, '`');

		if (delim) {
			*delim = '\0';

			astring = line;
			tstring = ++delim;
		} else {
			astring = line;
		}

		patt.ptext.astring = line;
		patt.ptext.length = strlen(line);
		patt.id.u.stringy = tstring;
		patt.id.type = AC_PATTID_TYPE_STRING;

		ac_trie_add(AHO_G(trie), &patt, 1);
		tstring = "";
	}

	ac_trie_finalize(AHO_G(trie));

	fclose(fp);
	free(line);

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

ZEND_BEGIN_ARG_INFO_EX(arginfo_aho_match, 0, 0, 1)
    ZEND_ARG_INFO(0, subject)
    ZEND_ARG_INFO(1, matches) /* array */
ZEND_END_ARG_INFO()

/* {{{ aho_functions[]
 *
 * Every user visible function must have an entry in aho_functions[].
 */
const zend_function_entry aho_functions[] = {
	PHP_FE(aho_match, arginfo_aho_match)
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
