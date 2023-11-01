set -e
. ./headers.sh

for PROJECT in $PROJECTS; do
        (cd src/$PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done
