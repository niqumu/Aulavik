set -e
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -serial stdio -cdrom aulavik.iso
