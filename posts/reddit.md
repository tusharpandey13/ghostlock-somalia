# Reddit

Draft, replace the URL before posting.

Title:

```text
GhostLock CVE-2026-43499 on a Redmi A7 Pro 5G: root on three boots, now Permissive on a fix under observation
```

Body:

```text
I own the device and ran this under per-session written authorization. GhostLock
(CVE-2026-43499) took a uid-2000 shell to a uid-0 kernel credential patch on
three boots, with SELinux Permissive for that boot. No boot image was flashed or
modified.

It is per-boot only. The non-obvious part was the physical load address: the
Unisoc loader places the kernel at 0x80080000 + (TOS byte << 15) on every boot,
and the same byte seeds the virtual KASLR. The tracefs leak recovers that byte,
so the physical load is derivable per boot, no brute force and no serial port.

A later dual-socket su build passed its smoke tests, then panicked in module_put
via __fput on a binder thread and rebooted. I changed the server to close only
pipe/regular fds, re-rooted on the new boot, and both the private socket and the
abstract ADB path work. No new panic in the short watch window, but the fix is
still under observation, not proven stable.

This is not a how-to and not a claim of persistent root. I am posting the
evidence trail: the tracefs KASLR leak, the per-boot P0 derivation, the QEMU
lab, the cred-only payload, and both incident records.

Writeup: <public-url>
```

Notes:

- Owned device, public CVE, per-boot, fix under observation. No serial or IP.
- Post as a research note, not a "how to root your phone" guide.
