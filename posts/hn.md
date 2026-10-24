# Hacker News

Draft, replace the URL before posting. No "Show HN" prefix.

Title:

```text
GhostLock on a Redmi A7 Pro 5G: root three times, then a su-build panic
```

First comment:

```text
Author here. The device is a Redmi A7 Pro 5G (somalia, Unisoc T8300, Android 16,
kernel 6.6.118). GhostLock CVE-2026-43499 gave a uid-0 cred patch from uid 2000
on three boots, without touching the boot image. The non-obvious part was
deriving the per-boot physical load from the tracefs KASLR leak: the loader
places the kernel at 0x80080000 + (TOS byte << 15), the same byte seeds the
virtual KASLR slide, so the leak yields the physical load at runtime. That
removes the host-side rebuild on each boot.

The phone is rooted and Permissive now, on a fix that is under observation. A
dual-socket/private-Termux su build passed its smoke tests, then panicked in
module_put via __fput and rebooted. I changed the server to close only
pipe/regular fds (not char/ashmem/socket), re-rooted on the new boot, and no new
panic appeared in the short watch window. I am not claiming it is stable yet.

I wrote up both incidents, including the pstore signature and the suspected
inherited-fd/forged-fops teardown path. The bug itself is upstream: NebuSec's
IonStack part II covers the root cause. Happy to answer questions about the
leak, the P0 formula, or the QEMU lab.

Writeup: <public-url>
```

Notes:

- Owned device, public CVE, per-boot, fix under observation. No serial or IP.
- The only external reference named is NebuSec's writeup, which is verified.
