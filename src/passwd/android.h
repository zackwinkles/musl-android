#include <features.h>

//int access (const char *, int);

hidden static void
__android_setup_pwd (struct passwd * pw) {
	if (access ("/system/bin/bash", 1))
		pw->pw_shell = "/system/bin/bash";
	else
		pw->pw_shell = "/system/bin/sh";

	pw->pw_dir = "/data/data/com.termux/files/home";
	pw->pw_passwd = "*";
	pw->pw_gecos = ""; /* Avoid NULL field. */
