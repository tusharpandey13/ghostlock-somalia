# Reproduce the device-specific parts

The full exploit source is upstream. This repository publishes the target
offsets, the accepted pselect patch, the P0 helper, and the lab method. It does
not ship a prebuilt payload.

## Inputs

- Upstream GhostLock tree: `JingMatrix/pixel-ksu-root`, commit
  `65762e75248d99c07eaf4e518e2b928e279411bd`.
- Device target: `tools/somalia/target/target.h` and `kernel-image.h`.
- Accepted route patch: `tools/somalia/patch/pselect-attempts1.patch`.
- P0 helper: `scripts/ghostlock_p0_from_slide.py`.
- Kernel image hash: `81181e8321e655acbce601faff94ab1610f9b825869a56a1c0e5bfcf9db1126c`.
- Toolchain used here: NDK r27c for the Bionic payload; zig 0.17.0 for the
  static lab binary.

## Host build sketch

```sh
# point the target header at the somalia files
# apply the accepted patch to fops.c
# build the upstream payload with LIB_KMI=66
# build the helper that loads the payload
```

The exact upstream build script is in the upstream repository. The public
target header and patch are the device-specific deltas. Do not substitute a
fixed P0; build or patch it from the current boot's leak.

## QEMU check

Use the vendor `Image` extracted from `boot.img`. A known-good invocation is:

```sh
qemu-system-aarch64 -M virt -cpu max -smp 4 -m 2048 -accel hvf \
  -kernel Image -initrd initramfs.cpio.gz \
  -append 'console=ttyAMA0 rdinit=/init' -nographic -no-reboot
```

The lab init must run `umask(0)` before `mknod("/dev/ashmem", ...)`; otherwise
the open fails with EACCES and looks like an exploit failure. The VM used
`P0=0x40200000` only as a local QEMU value. That value is not the phone's P0.

A forced first miss can be produced with a lab-only hook in `try_cfi_stage()`,
gated on a test variable and `cfi_attempts == 1`. It must not be shipped in a
phone build.

## Phone check

1. Boot normally.
2. Run the tracefs leak; derive P0 from the leaked `_text`.
3. Build or patch the payload with that P0.
4. Run one bounded attempt. Stop on root, panic, or timeout.
5. Do not reuse P0. Do not reboot to test a wireless path.

Expected root markers and the limits of what they mean are in
[evidence/root-runs.md](../../evidence/root-runs.md) and
[cred-patch](../exploit/cred-patch.md).
