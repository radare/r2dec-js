/* radare - LGPL - Copyright 2018 - pancake */
#if 0
gcc -o core_test.so -fPIC `pkg-config --cflags --libs r_core` core_test.c -shared
mkdir -p ~/.config/radare2/plugins
mv core_test.so ~/.config/radare2/plugins
#endif

#include <r_types.h>
#include <r_lib.h>
#include <r_cmd.h>
#include <r_core.h>
#include <r_cons.h>
#include <string.h>
#include <r_anal.h>

#undef R_API
#define R_API static
#undef R_IPI
#define R_IPI static

static void _cmd_pdd(RCore *core, const char *input) {
	switch (*input) {
	case '?':
		eprintf ("Usage: pdd [args] - core plugin for r2dec\n");
		eprintf (" pdd   - decompile current function\n");
		eprintf (" pdd?  - show this help\n");
		eprintf (" pddu  - install/upgrade r2dec via r2pm\n");
		break;
	case 'u':
		// update
		r_core_cmd0 (core, "!r2pm -ci r2dec");
		break;
	default:
		// decompile
		r_core_cmdf (core, "#!pipe r2dec --colors %s", input);
		break;
	}
}

static int r_cmd_pdd(void *user, const char *input) {
	RCore *core = (RCore *) user;
	if (!strncmp (input, "e cmd.pdc", 9)) {
		if (strchr (input, '=') && strchr (input, '?')) {
			r_cons_printf ("r2dec\n");
			return false;
		}
	}
	if (!strncmp (input, "pdd", 3)) {
		_cmd_pdd (core, input + 3);
		return true;
	}
	return false;
}

RCorePlugin r_core_plugin_test = {
	.name = "r2dec",
	.desc = "pseudo-decompiler available with the pdd command",
	.license = "Apache",
	.call = r_cmd_pdd,
};

#ifndef CORELIB
RLibStruct radare_plugin = {
	.type = R_LIB_TYPE_CORE,
	.data = &r_core_plugin_test,
	.version = R2_VERSION
};
#endif
