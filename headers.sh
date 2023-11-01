set -e
. ./config.sh

mkdir -p "$SYSROOT"

for PROJECT in $SYSTEM_HEADER_PROJECTS; do
        (cd src/$PROJECT && DESTDIR="$SYSROOT" $MAKE install-headers)
done
