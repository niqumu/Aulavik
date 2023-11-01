set -e
. ./config.sh

for PROJECT in $PROJECTS; do
        (cd src/$PROJECT && $MAKE clean)
done

rm -rf sysroot
rm -rf isodir
rm -rf aulavik.iso
