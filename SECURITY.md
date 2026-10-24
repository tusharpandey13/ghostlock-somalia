# Security and scope

This repository documents security research on a single, owner-authorized
Redmi A7 Pro 5G (`somalia`, Unisoc T8300). GhostLock (CVE-2026-43499) is a
public Linux kernel vulnerability; this project is a device-specific port and
research record, not a new CVE disclosure.

The material is published for defensive research, reproducibility, and
education. It is not a turnkey rooting service and includes no prebuilt
payloads, firmware, keys, or device identifiers. Do not use it on devices you
do not own or have explicit authorization to test.

The public tree is deliberately curated. Raw captures, device identifiers,
ADB authorization material, vendor images, and owner-specific logs are retained
privately and are not part of this repository.

## Reporting an issue in this repository

Use GitHub's private security advisory feature for this repository, or the
contact listed on the owner's GitHub profile. Do not open a public issue for
unredacted device data or credentials.

## Upstream credit

GhostLock: NebuSec, "IonStack Part II — GhostLock", Apache-2.0.
Porting references: JingMatrix/pixel-ksu-root; JoinChang/ghostlock-oneplus;
KernelSnitch (TU Graz). See `NOTICE`.
