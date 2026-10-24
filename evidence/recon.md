# Sanitized reconnaissance excerpts

Selected lines from the read-only and root-context recon on one owned unit. This
is not a full log. Boot IDs are replaced with Boot A, Boot B, and Boot C. Serial
numbers, IMEI, MAC addresses, IP addresses, account IDs, and ADB keys are
removed.

Tags are VALIDATED, INFERRED, UNVALIDATED, BLOCKED.

## Root shell identity

Boot A, root context reached through the per-boot abstract socket: [VALIDATED]

```text
uid=0(root) gid=0(root) groups=0(root),1004(input),1007(log),1011(adb),1015(sdcard_rw),1028(sdcard_r),1078(ext_data_rw),1079(ext_obb_rw),3001(net_bt_admin),3002(net_bt),3003(inet),3006(net_bw_stats),3009(readproc),3011(uhid),3012(readtracefs) context=u:r:kernel:s0
Permissive
```

The context is `u:r:kernel:s0`. The root process is not a modified `su`; it is
the exploit's child plus a socket server. A fresh shell stays uid 2000.
[VALIDATED]

## Physical map

Boot A, `P0 = 0x80828000`. [VALIDATED]

```text
80000000-fff6ffff : System RAM
  80838000-82527fff : Kernel code
  82938000-82bd7fff : Kernel data
fff80000-17fffefff : System RAM
```

`Kernel code` starts at `P0 + 0x10000`. `Kernel data` starts at `P0 + 0x2110000`.
The two System RAM ranges cover about 4 GiB. The serial and all unique
identifiers from the same capture are omitted.

## Bootloader entry line

Boot A, from the root-only `uboot_log`: [VALIDATED]

```text
Start linux at PC:0x80828000
```

A historical scan found 25 such lines with 20 distinct load addresses from
`0x80118000` to `0x80828000`, all in `0x8000` steps. The current boot's line is
not reusable after a reboot.

## kptr_restrict

Boot A, default value: [VALIDATED]

```text
kptr_restrict = 2
```

At that value both files list names but mask addresses:

```text
/proc/kallsyms : every address 0000000000000000
/proc/modules  : every base    0x0000000000000000
```

A single temporary drop to 0 was used to capture symbols and then restored to 2
and verified. The default stayed 2. [VALIDATED]

## Partitions and slot

Boot A, root context: [VALIDATED]

```text
90 named GPT entries on sda, 61,176 MiB
spl_a = sdb, 4 MiB
spl_b = sdc, 4 MiB
active slot = _b
slot A: tries_remaining = 0
```

Slot A is not bootable. The `misc` partition held `bootonce-bootloader` and the
`BCAB` control block. A reboot could enter the bootloader instead of Android.
[VALIDATED]

## Tracefs and GPU modes

Boot A, root context: [VALIDATED]

```text
crw-rw-rw-  /dev/mali0
-rwxrwxrwx  /sys/kernel/tracing/kprobe_events
```

`/dev/mali0` is `0666` and labelled `u:object_r:gpu_device:s0`.
`kprobe_events` is `0777`, but the per-probe `enable` files are `0640`
root:readtracefs. `shell` can create a probe and cannot enable it. The
`trace_marker` plus `trace_pipe_raw` path is the working KASLR leak. [VALIDATED]

## What the evidence supports

| Claim | Status |
|---|---|
| A uid-2000 shell reached a uid-0 kernel credential patch | VALIDATED, demonstrated on three boots |
| SELinux became Permissive on the rooted boot | VALIDATED, in memory only |
| Root survived a reboot | No. A reboot restored Enforcing and dropped root. VALIDATED |
| The current selective-fd fix is stable | UNVALIDATED, under observation |
| The 90-entry partition map and slot-B-only state are stable | VALIDATED |
| The phone stayed locked and green through the work | VALIDATED |

## Redaction

Removed from these excerpts:

- device serial and IMEI,
- MAC address, Wi-Fi SSID, and IP address,
- Xiaomi account and service identifiers,
- ADB public keys and private key paths,
- boot IDs, replaced with Boot A/B/C,
- host paths and session metadata.

No raw full log, firmware image, vendor policy blob, or payload binary is
included. The excerpts are short enough to check a claim and too short to be a
turnkey recipe.

## Related pages

- [Device overview](../docs/device/overview.md)
- [Memory map](../docs/device/memory-map.md)
- [SELinux](../docs/device/selinux.md)
- [Offsets](../reference/offsets.md)
- [Glossary](../reference/glossary.md)
