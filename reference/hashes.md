# Hashes and provenance

Hashes for the kernel image, firmware containers, Mali module, helper payloads,
and upstream commits. A hash identifies a build or file; it does not make the
file public. The "Availability" column says whether the artifact is a public
download, a public source commit, or a private build/evidence file.

Tags are VALIDATED, INFERRED, UNVALIDATED, BLOCKED.

## Firmware and kernel

| Artifact | SHA-256 | Availability | Tag |
|---|---|---|---|
| Vendor package `OS3.0.13.0.WBWINXM` | `17b7725823a96f7996c0d031c27933c78793b434694e1cdff5684ca3f424a104` | public download; not redistributed here | VALIDATED |
| `boot.img` (vendor package image) | `3b00662df114ee11daa315fea191b670a9c39cc9005d0f0f122d3212ac85adad` | extracted from the vendor package; not redistributed | VALIDATED |
| `vendor_boot.img` (vendor package image) | `77abf64ba1241a417170d3958331b4f37a47b314cc55535ad078ceb062f66a47` | extracted from the vendor package; not redistributed | VALIDATED |
| Kernel `Image` | `81181e8321e655acbce601faff94ab1610f9b825869a56a1c0e5bfcf9db1126c` | extracted from `boot.img`; not redistributed | VALIDATED |
| `vmlinux` | `cd396c20c0aa5a705d2c111131ff0753453a3fd689b3ecdf36c89175f38274e2` | private analysis artifact | VALIDATED |
| `/proc/config.gz` | `e8b254e98e11f4ffcaef3e29b07fe32870af0988e573c4cccf88b01bf38fa2b0` | private device capture | VALIDATED |
| SELinux policy blob | `8f16da7b21a25245e65449844a8154dd694f7b1046b0cb4e4e21c3441de8caa1` | private device capture | VALIDATED |
| `super.img` | `18e51bfed4e782e26e6f548095919d665aa7f6957dd5157357ff7eedeb6182f3` | extracted from the vendor package; not redistributed | VALIDATED |

The kernel in `boot.img` is byte-identical to the extracted `Image`. That
equality is what makes the offline symbol table applicable to the phone image.
[VALIDATED]

The build fingerprint is a string, not a hash:

```text
Redmi/somalia_in/somalia:16/BP2A.250605.031.A3/OS3.0.13.0.WBWINXM:user/release-keys
```

The kernel banner is:

```text
6.6.118-android15-8-ge56cf6b09cca-ab15511674-4k
```

## Mali module

| Artifact | Value | Availability | Tag |
|---|---|---|---|
| `mali_kbase.ko` SHA-256 | `70a7326260f978c3470737db0cb0b71ab74911050e7f69a253744229048aef1c` | extracted from the vendor package; not redistributed | VALIDATED |
| Build ID (SHA-1 note) | `3d572a38eae1f2e0f67de1e5c909cf1e13c498ca` | public value from the shipped module | VALIDATED |
| DDK version | `r54p1-12eac0 (UK version 11.46)` | public value | VALIDATED |
| `srcversion` | `1395BEBB8675C9F007549F5` | public value | VALIDATED |

The module file is 2,026,064 bytes. The loaded copy is 1,159,168 bytes. Both
facts are recorded because `/proc/modules` reports the loaded size, not the file
size. [VALIDATED]

## GhostLock target and helper builds

These are private builds produced for this one device. They are listed so the
evidence can be checked, not distributed as payloads.

| Artifact | SHA-256 | Availability | Tag |
|---|---|---|---|
| Stock chain `.so` | `4037e15d92714cfc71c9f90718713b3ea7b9e11c58a74cd5b1dc98af707c33c7` | private build | VALIDATED |
| `cve-2026-43499-root` helper | `767f85ad5108e02efe7a77fc957ec0a22cca03e64a524b1228f79b2c55339956` | private build | VALIDATED |
| Cred-only `.so`, P0 `0x80000000` | `5804dc353e086263b2fd2c85f4d07647a2e3677c7e6fd6138c2ad2560214f4f0` | private build | VALIDATED |
| Cred-only `.so`, P0 `0x80200000` | `5b8b141fde2b1daef332a59f579af03d05737761b76fbf843fc3bf872ece2c79` | private build | VALIDATED |
| Cred-only static binary, P0 `0x80000000` | `ea6f315d38fd245409acc8070574d30974c53e1bd55a3075e15aa5e6d36a8959` | private build | VALIDATED |
| Cred-only `.so` plus root socket | `cdab57f08f720c9b1886ecaf662449b1a4d436aa3b9a173e55984bb591d5d33e` | private build | VALIDATED |
| `root.sh` runtime-P0 launcher | `3930be8e41d79b701dd015a5a81e0d5de4c5f24a2a007b11806587144f778b9f` | private build | VALIDATED |
| First non-socket root build | `8b673adf3ce3748dfbf1a25ae347d51ae0af3d37bfc0088cc89aa1ec5e817313` | private build, historical | VALIDATED |

The two fixed-P0 cred-only builds are historical. They used wrong physical
targets (`0x80000000` and `0x80200000`) and are the reason fixed P0 was dropped.
The later builds derive P0 from the boot's own slide. [VALIDATED]

The target header that records the offsets is at
[`tools/somalia/target/kernel-image.h`](../tools/somalia/target/kernel-image.h).
The launcher is [`tools/somalia/root.sh`](../tools/somalia/root.sh).

## Upstream commits

| Project | Commit | License / availability | Tag |
|---|---|---|---|
| `JingMatrix/pixel-ksu-root` | `65762e75248d99c07eaf4e518e2b928e279411bd` | public | VALIDATED |
| `JoinChang/ghostlock-oneplus` | `9b2a7ba61cc2afe0f5652af2f317b62eec68356a` | public | VALIDATED |
| `nxp-imx/linux-imx` r54 Mali source | `be78e49cb4339fd38c9a40019df49b72fbb8bcb7` | public | VALIDATED |
| `nxp-imx/linux-imx` r56 Mali source | `6e694eda40aabd6671fca82b5416dbd76beeebb9` | public | VALIDATED |
| `Xilinx/malig78ae-kbase` r54p1 source | `b310d9654874e9fd47dd118b79312a77194e29dc` | public | VALIDATED |

The public upstream trees are used for analysis and offset derivation. No
prebuilt exploit binary, vendor firmware, or device identifier is redistributed
in this repository.

## What is not hashed here

- The device serial, IMEI, MAC address, Wi-Fi SSID, IP address, account ID, and
  ADB keys.
- Vendor signing keys and the AVB public key material.
- Full partition images, EROFS trees, APEX files, and debloated vendor APKs.

Those are either identifiers, secrets, or artifacts too large to redistribute.

## Related pages

- [Offsets](offsets.md)
- [Glossary](glossary.md)
- [Device overview](../docs/device/overview.md)
- [AVB](../docs/device/avb.md)
- [Mali](../docs/device/mali.md)
