# Disclosure and vendor status

CVE-2026-43499 is public. The NVD record was published 2026-05-21 and the
NebuSec writeup is public. This repository does not create a new embargo and
does not claim discovery of that CVE.

## What this repository publishes

A device-specific port on a Redmi A7 Pro 5G, a per-boot P0 derivation, QEMU
parity, and two incident records. It publishes source offsets and patches, not
a turnkey payload or keys.

## Vendor contact

As of 2026-10-24, Xiaomi and Unisoc have **not** been contacted about this port.
The public CVE is already upstream; a vendor report about "I rooted my phone"
would not be a new vulnerability report.

Two observations from the read-only recon may be worth a private hardening
report, if the owner chooses to pursue it:

- `/proc/mali/memory_usage` appeared to expose live `kctx` pointers while
  `kptr_restrict=2` was set.
- `kprobe_events` was `0777` and `/dev/mali0` was `0666` on a user build.

Those are device/vendor observations, not claims of a new CVE. A report should
use the vendor security portal, include a redacted build fingerprint, and omit
serial numbers, IMEI, MAC addresses, ADB keys, and the full exploit. Ask for a
90-day coordination window for any new issue. Do not sign an NDA without
reading it.

If the vendor responds, add the date and outcome here. If not, say so; silence
is part of the record.
