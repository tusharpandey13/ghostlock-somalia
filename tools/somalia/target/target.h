// targets/somalia-OS3.0.13.0/target.h — composed target description, android15-6.6.
//
// Committed rather than resolver-composed because the vendor kernel banner
// "6.6.118-android15-8-ge56cf6b09cca-ab15511674-4k" has no "-ab<digits>"
// segment, so effective_target_header() would emit no kernel-image include.
// This file states the build facts, then includes the kernel-image layer
// (kernel-image.h, symbol/struct offsets for this one image) and the interface
// layer (cves/targets/kmi/android15-6.6.h, geometry/layouts/slab defaults).
//
// Every definition below is written so a later layer's #ifndef default defers
// to the build's own value, matching the layering convention in
// cves/targets/README.md.
#ifndef OFFSET_H
#define OFFSET_H

#if defined(APP_PAYLOAD) && APP_PAYLOAD
#define BUILD_VARIANT_LABEL "somalia-OS3.0.13.0-app"
#else
#define BUILD_VARIANT_LABEL "somalia-OS3.0.13.0-root-umh"
#endif

#ifndef BUILD_FINGERPRINT
#define BUILD_FINGERPRINT \
  "Redmi/somalia_in/somalia:16/BP2A.250605.031.A3/OS3.0.13.0.WBWINXM:user/release-keys"
#endif

/* The kernel image these offsets describe, as its banner names it. A payload
 * is valid for an IMAGE, not for a phone; anything checking that a binary
 * belongs to the kernel it is about to run against compares this. */
/* Derive P0 from the boot's own tracefs KASLR slide at runtime. The vendor
 * loader uses 0x80080000 + (TOS byte << 15); fixed P0 is not valid per boot. */
#ifndef P0_DERIVE_FROM_SLIDE
#define P0_DERIVE_FROM_SLIDE 1
#endif

#ifndef TARGET_KERNEL_ID
#define TARGET_KERNEL_ID "6.6.118-android15-8-ge56cf6b09cca-ab15511674-4k"
#endif

/* KERNEL_VERSION(6,6,118) = (6<<16)|(6<<8)|118. Read at run time by
 * tcp_gate_check() (fops_tcp.c). */
#ifndef KERNEL_VERSION_CODE
#define KERNEL_VERSION_CODE 0x060676ULL
#endif

/* P0_KERNEL_PHYS_LOAD: SUPERSEDED 2026-10-23 by the firmware audit. The
 * vendor loader computes phys(_text)=0x80080000+(TOS random byte<<15) on every
 * normal boot; 0x80000000 is always wrong and 0x80200000 only if byte=0x30.
 * Do not use a fixed P0 for a device build without the current boot's value.
 * The current boot's value is derivable from the tracefs KASLR slide;
 * see docs/exploit/p0-derivation.md. The old comment below is retained as
 * history.
 *
 * Old: the KMI default 0x80000000 is the best default but is
 * UNVALIDATED for somalia. `PHYS_OFFSET` is the RAM base, not phys(_text). The
 * QEMU +2 MiB is a QEMU loader artifact, not head.S relocation. The stock
 * chain is lab-validated and needs this value for its pipe primitive; the
 * virtual-data/pselect re-route is blocked by HARDENED_USERCOPY. Resolve before
 * a device run. */

/* Kernel-image layer: symbol and structure offsets for this one image. */
#include "kernel-image.h"

/* Interface layer: address-space geometry, structure layouts, slab params and
 * the stack-map constants (SIGRETURN/CLONE3/PROCESS_VM/BPF/MCAST/SOCKOPT
 * WAITER_OFF) for android15-6.6. */
#include "../../../thirdparty/pixel-ksu-root/cves/targets/kmi/android15-6.6.h"

#endif /* OFFSET_H */
