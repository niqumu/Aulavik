set -e
. ./build.sh

mkdir -p ./isodir/boot/grub

\cp ./sysroot/boot/aulavik.kernel ./isodir/boot/aulavik.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "aulavik" {
    multiboot /boot/aulavik.kernel
}
EOF
grub-mkrescue -o aulavik.iso ./isodir