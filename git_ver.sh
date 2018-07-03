#!/bin/bash
rm -f SerialTool/include/version.h
git rev-list HEAD | sort > config.git-hash
LOCALVER=`wc -l config.git-hash | awk '{print $1}'`
if [ $LOCALVER \> 1 ] ; then
    VER=`git rev-list origin/master | sort | join config.git-hash - | wc -l | awk '{print $1}'`
    if [ $VER != $LOCALVER ] ; then
        VER="$VER+$(($LOCALVER-$VER))"
    fi
    VER="$VER$(git rev-list HEAD -n 1 | cut -c 1-3)"
    if git status | grep -q "modified:" ; then
        VER="${VER}M"
    fi
    GIT_VERSION=$VER
else
    GIT_VERSION=
    VER="x"
fi
rm -f config.git-hash

cat SerialTool/ver_temp.h | sed "s/\$GIT_VERSION/$GIT_VERSION/g" > SerialTool/include/version.h

echo "Generated version.h"
