# Affected build and claim boundary

This is a single-device result. It says nothing about other builds, other
regions, or other `somalia` units.

| Item | Value |
|---|---|
| Device | Redmi A7 Pro 5G, `somalia`, model 26020RN1AI |
| SoC | Unisoc T8300 / UMS9632 |
| Android | 16, `BP2A.250605.031.A3` |
| Build | `OS3.0.13.0.WBWINXM` |
| Security patch | 2026-08-01 |
| Kernel | `6.6.118-android15-8-ge56cf6b09cca-ab15511674-4k` |
| Slot | `_b`; slot A reported unbootable |
| CVE | CVE-2026-43499 (GhostLock), public |
| Local access needed | ADB shell / uid 2000, or an equivalent local shell |
| Remote attack | none demonstrated |
| Vendor patch status | not confirmed |
| Result here | per-boot uid-0 credential patch; no persistence |

The practical access requirement matters. This is not a remote exploit and not a
lock-screen bypass. It starts from an already-authorized local shell. A device
with a different build, kernel, compiler schedule, or bootloader may need
different offsets and may not have the same P0 behavior.

The CVE is public and already patched in upstream Linux. This repository is a
port and device study, not a new vulnerability report. See
[disclosure](../methodology/disclosure.md).
