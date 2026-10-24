# QEMU validation notes

The lab used the exact vendor kernel `Image` and a source-parity GKI 6.6.118
build. The key lessons are in `docs/lab/qemu.md`:

- fix the initramfs `/dev/ashmem` umask before judging an open failure;
- validate CFI with genuine kernel-text fops slots;
- use a forced first-miss hook to exercise the post-miss path without relying
  on a lucky phone miss;
- reject the process-wide SIGALRM refresh watchdog; use one pselect attempt per
  process and let the outer loop re-fork.

The raw lab tree is not published (large kernel source/build outputs).
