#!/bin/sh

if test -d .git ; then
if type git >/dev/null 2>&1 ; then
git describe --tags --match 'v[0-9]*' 2>/dev/null \
| sed -e 's/^v//' -e 's/-/-android-/'
else
sed 's/$/-android/' < VERSION
fi
else
cat VERSION
fi
