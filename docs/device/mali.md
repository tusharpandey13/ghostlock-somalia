# Mali

The GPU is a Mali-G57 MC2 driven by an out-of-tree Unisoc `mali_kbase` module.
It is a large, world-accessible attack surface on this build. The module was
studied as one exploit path, but the root result in this repository comes from
GhostLock, not from a Mali write primitive.

Tags are VALIDATED, INFERRED, UNVALIDATED, BLOCKED.

## Hardware

| Fact | Value | Tag |
|---|---|---|
| GPU | Mali-G57 MC2, r0p1 | VALIDATED |
| GPU ID | `0x09000901` | VALIDATED |
| Devfreq node | `23100000.gpu` | VALIDATED |
| Frequencies | 950, 760, 512, 384 MHz | VALIDATED |
| Governor | `simple_ondemand` | VALIDATED |
| Coherency | none; DT `system-coherency = <0x1f>` (`COHERENCY_NONE`) | VALIDATED |

## Module identity

| Fact | Value | Tag |
|---|---|---|
| Module name | `mali_kbase_r54p1` | VALIDATED |
| DDK / UK version | `r54p1-12eac0 (UK version 11.46)` | VALIDATED |
| File | `vendor_dlkm/lib/modules/mali_kbase.ko` | VALIDATED |
| File size | 2,026,064 bytes | VALIDATED |
| File SHA-256 | `70a7326260f978c3470737db0cb0b71ab74911050e7f69a253744229048aef1c` | VALIDATED |
| Loaded size | 1,159,168 bytes | VALIDATED |
| Build ID | `3d572a38eae1f2e0f67de1e5c909cf1e13c498ca` | VALIDATED |
| Vermagic | `6.6.118 SMP preempt mod_unload modversions aarch64` | VALIDATED |
| Dependencies | `unisoc_mm_emem,sprd_sip_svc,trusty` | VALIDATED |
| Taint | out-of-tree, shown as `(O)` in `/proc/modules` | VALIDATED |
| Frontend | job manager (JM), not CSF | VALIDATED |

The file is not stripped. It has 11,729 symbol-table entries, including 1,562
functions, but no DWARF and no `.BTF` section. Function names and offsets are
available offline; type layouts need the matching r54p1 source. [VALIDATED]

The module base is assigned at load time and is not stable across boots. No live
base is published here. Offsets below are relative to the module base and
are stable for this exact `.ko`. [VALIDATED]

## Selected function offsets

These are file/module offsets, not runtime addresses. [VALIDATED]

| Symbol | Offset |
|---|---|
| `kbase_jd_submit` | `0x378d0` |
| `kbase_mem_free` | `0x2118` |
| `kbase_api_mem_alloc` | `0x2126c` |
| `kbase_api_mem_jit_init` | `0x2136c` |
| `kbase_api_mem_alias` | `0x213f0` |
| `kbase_api_mem_import` | `0x2154c` |
| `kbase_soft_event_update` | `0x10d28` |

The uAPI constants match the module: `MEM_ALLOC 0xc0208005`, `MEM_FREE
0x40088007`, `MEM_SYNC 0x4020800f`, `MEM_ALIAS 0xc0208015`, `MEM_IMPORT
0xc0188016`, `SOFT_EVENT_UPDATE 0x4010801c`, and `JOB_SUBMIT 0x40108002`.
[VALIDATED]

## Mitigations

| Mitigation | State | Evidence | Tag |
|---|---|---|---|
| KCFI | on | 111 type IDs and 222 `brk #0x8228` traps in the module | VALIDATED |
| PAC | compiled, not active | `paciasp`/`autiasp` present; HWCAP `paca`/`pacg` absent | VALIDATED |
| BTI | off | `ARM64_BTI_KERNEL` unset, zero `bti` instructions | VALIDATED |
| MTE | absent | HWCAP `mte` absent, zero tag instructions | VALIDATED |
| Shadow call stack | kernel on, module off | kernel `x18` references; zero in the module | VALIDATED |
| KASAN | compiled, cannot arm | `KASAN_HW_TAGS=y` but no MTE | VALIDATED config, INFERRED runtime |
| KFENCE | sampled | `KFENCE_SAMPLE_INTERVAL=500`, 63 objects | VALIDATED |
| KASLR | on | virtual slide random per boot | VALIDATED |
| Module region | partly randomised | `RANDOMIZE_MODULE_REGION_FULL` unset | VALIDATED |

The practical result: KCFI constrains indirect calls, but PAC, BTI, and MTE do
not protect this module. That is a property of the firmware, not a claim that
the GPU is exploitable from an app.

## Access surface

| Interface | State | Tag |
|---|---|---|
| `/dev/mali0` | `c 10:273`, mode `0666`, label `u:object_r:gpu_device:s0` | VALIDATED |
| `shell` to `/dev/mali0` | allowed; `shell` is in `appdomain` on this build | VALIDATED |
| App domains to `/dev/mali0` | `open`, `read`, `write`, `ioctl`, `map` allowed; no `allowxperm` ioctl allowlist | VALIDATED |
| `/proc/mali/memory_usage` | prints live `kctx` kernel pointers | VALIDATED |
| `/proc/mali/jit_can_reclaim` | mode `0664`, value `1` | VALIDATED |
| sysfs under `/sys/class/misc/mali0/device` | `gpuinfo`, `core_mask`, `js_timeouts`, `power_policy`, `dvfs_period`, memory pool sizes | VALIDATED |
| debugfs kbase nodes | exist when debugfs is mounted; most values use `%pK`; mounting and reading were blocked by session rules | BLOCKED |
| tracefs kprobe surface | `kprobe_events` mode `0777`; `enable` files `0640` root:readtracefs | VALIDATED |

The `/dev/mali0` mode and the generic SELinux grant are the reason the GPU is
reachable from a plain `shell` or an ordinary app. There is no GPU ioctl
allowlist in the shipped policy. [VALIDATED]

## Kernel pointer leak

`/proc/mali/memory_usage` prints `kctx-0x...` values for live GPU contexts. It is
a kernel-address disclosure that does not depend on `kptr_restrict`. On this
build it was readable as root and is documented as visible to the matching
process. The exact live values are not reproduced here. [VALIDATED for the
interface content, INFERRED for the unprivileged read scope]

## Mali race work

A separate line of work validated a page-UAF race in `kbase_do_syncset()`:

- The shipped module lacks the `KBASE_REG_DONT_NEED` guard that later r56p0
  adds before page sync. [VALIDATED by binary comparison]
- A controlled `MEM_SYNC` race produced two panics in `kbase_sync_single+0xa8`
  with a poisoned physical address. [VALIDATED]
- The panic is a crash oracle, not a write primitive. The stale-PFN one-page
  `dc ivac` path was not proven. [VALIDATED for the oracle, UNVALIDATED for the
  primitive]

The exploit chain that achieved root uses GhostLock's futex/PI path instead.
Mali remains a documented surface and a validated crash oracle, not the root
path.

## Related pages

- [Device overview](overview.md)
- [SELinux](selinux.md)
- [Kernel config](kernel-config.md)
- [Memory map](memory-map.md)
- [Offsets](../../reference/offsets.md)
- [Glossary](../../reference/glossary.md)
