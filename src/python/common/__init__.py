import sys


def warn(stuff: str):
    sys.stderr.write(f"[!] {stuff}\n")
    sys.stderr.flush()
def info(stuff: str):
    print(f"[*] {stuff}...")
    sys.stdout.flush()
def okay(stuff: str):
    print(f"[+] {stuff}\n")
    sys.stdout.flush()

