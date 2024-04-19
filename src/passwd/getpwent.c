#include "pwf.h"
#include "android.h"

static FILE *f;
static char *line;
static struct passwd pw;
static size_t size;

void setpwent()
{
	if (f) fclose(f);
	f = 0;
}

weak_alias(setpwent, endpwent);

struct passwd *getpwent()
{
	struct passwd *res;
	if (!f) f = fopen("/system/etc/passwd", "rbe");
	if (!f) return 0;
	__getpwent_a(f, &pw, &line, &size, &res);
	return res;
}

struct passwd *getpwuid(uid_t uid)
{
#if defined(__ANDROID__)
	struct passwd* res;
	__getpw_a(0, uid, &pw, &line, &size, &res);
	if (res == NULL) return NULL;
	__android_setup_pwd(res);
	return res;
#else
	struct passwd *res;
	__getpw_a(0, uid, &pw, &line, &size, &res);
	return res;
#endif
}

struct passwd *getpwnam(const char *name)
{
#if defined(__ANDROID__)
	struct passwd* res;
	__getpw_a(name, 0, &pw, &line, &size, &res);
	if (res == NULL) return NULL;
	__android_setup_pwd(res);
	return res;
#else
	struct passwd *res;
	__getpw_a(name, 0, &pw, &line, &size, &res);
	return res;
#endif
}

