# Termux GhostLock wrappers

Installed into `$PREFIX/bin` on the device: `gl-root`, `su`, `sudo`.
The original Termux `su` is preserved as `su.termux-orig`.

- `gl-root` bootstraps the per-boot root server over the phone's own wireless
  adb (`adb connect 127.0.0.1:<port>` with `GL_ADB_PORT=<port>`), then opens a root shell
  over the private socket `~/.gl/root.sock`.
- `su -c 'cmd'` / `sudo -c 'cmd'` run one command as root through that socket.
- The private socket is `0600`, owner Termux, parent `0700`; other apps cannot
  reach it. The abstract `@ghostlock-root` socket remains for
  `adb forward ... localabstract:ghostlock-root` and accepts only peer UIDs
  0/2000 (root/shell).
- Per-boot only. After reboot, run `gl-root` again; the Termux files and the
  authorized key persist, but root/P0/server do not.
- Security: the private socket is a root channel. Do not share the Termux UID
  or the `~/.gl` directory. `su.termux-orig` restores Termux's stock no-root
  wrapper.
