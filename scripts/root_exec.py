#!/usr/bin/env python3
"""Run a command through the GhostLock root socket.

Prerequisite: the root server was started with GHOSTLOCK_ROOT_SOCKET=ghostlock-root
and the host has:
  adb -s <serial> forward tcp:12345 localabstract:ghostlock-root

Usage: ./scripts/root_exec.py --serial <serial> 'id; getenforce'
"""
import argparse, re, socket, sys, uuid

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--serial", required=True)
    ap.add_argument("--port", type=int, default=12345)
    ap.add_argument("command", nargs=argparse.REMAINDER)
    a = ap.parse_args()
    cmd = " ".join(a.command).strip()
    if not cmd:
        ap.error("empty command")
    marker = "__GL_RC_" + uuid.uuid4().hex + "__"
    s = socket.socket()
    s.settimeout(15)
    s.connect(("127.0.0.1", a.port))
    s.sendall((cmd + "\n").encode())
    s.sendall(("echo " + marker + "$?\n").encode())
    data = b""
    try:
        while marker.encode() not in data:
            chunk = s.recv(65536)
            if not chunk:
                break
            data += chunk
    finally:
        s.close()
    text = data.decode("utf-8", "replace")
    # Strip the shell prompt/banner before the first command output only when
    # the marker is the first thing; keep output otherwise.
    idx = text.find(marker)
    body = text[:idx] if idx >= 0 else text
    print(body, end="" if body.endswith("\n") else "\n")
    if idx >= 0:
        tail = text[idx + len(marker):].strip()
        m = re.match(r"(\d+)", tail)
        if m:
            print(f"[root_exec rc={int(m.group(1))}]")
        else:
            print(f"[root_exec rc=unknown tail={tail[:80]!r}]")

if __name__ == "__main__":
    main()
