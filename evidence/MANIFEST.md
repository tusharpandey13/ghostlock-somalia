# Evidence manifest

This manifest explains what the public evidence is, where it came from, and
what is deliberately omitted. It is not a dump of the audit store.

## Public evidence

| File | Content | Redaction |
|---|---|---|
| `root-success.md` | selected first-root markers and capability read | PIDs, long addresses, boot identifiers |
| `root-runs.md` | three-run table and selected markers | PIDs, boot identifiers, raw paths |
| `incidents.md` | selected pstore frames for debugfs and module_put | IPs, module list, long fault address |
| `recon.md` | read-only root recon summary | serial, IMEI, MAC, account ID, IP, SSID |

## Hashes

The kernel, firmware containers, Mali module, upstream commits, and build
hashes are in [reference/hashes.md](../reference/hashes.md). A hash identifies
a file; it does not make that file public.

## Private material

The private audit store holds the full raw captures, firmware, vendor source,
keys, ADB authorization material, and owner-specific logs. It is not part of
this repository. Public documents may summarize a private capture and cite the
public summary, but do not link to private paths.

## Sanitization rules

Removed or redacted:

- device serial, IMEI, MAC addresses, Wi-Fi SSID, IP addresses, account IDs
- ADB public/private keys and authorization files
- host paths and usernames
- process IDs and boot identifiers unless the claim needs them
- module lists, vendor blobs, firmware, and long nonessential addresses

Preserved where the claim depends on them:

- function names and offsets in panic stacks
- P0 values and per-boot slides
- root marker fields and hashes

If an identifier appears in a public file, report it through `SECURITY.md`
rather than opening a public issue.
