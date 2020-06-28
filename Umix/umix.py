# import readline
import subprocess
import select
import sys

p = None
history = []
def Start():
  global p
  if p:
    p.kill()
  p = subprocess.Popen(["out/emul"], bufsize=0,
                      stdout=subprocess.PIPE,
                      stdin=subprocess.PIPE)

def Run():
  while p.poll() == None:
    pipes, _, _ = select.select([p.stdout, sys.stdin],[], [])
    for pipe in pipes:
      if pipe == p.stdout:
        l = p.stdout.read(1)
        sys.stdout.write(l)
        sys.stdout.flush()
      if pipe == sys.stdin:
        l = raw_input()
        if l == "{PAV}":
          return
        history.append(l)
        p.stdin.write(l+"\n")

def ExecuteCommand(cmd):
  p.stdin.write(cmd+"\n")
  # print(cmd)
  output = b""
  while p.poll() == None:
    pipes, _, _ = select.select([p.stdout],[], [], 0.5)
    if not pipes and output:
      return output
    for pipe in pipes:
      if pipe == p.stdout:
        l = p.stdout.read(1)
        output += l
        sys.stdout.write(l)
        sys.stdout.flush()

def ReplayCommands(commands):
  for cmd in commands:
    ExecuteCommand(cmd)

adventure_first_room = [
  "howie",
  "xyzzy",
  "adventure",
  "north",
  "ex bolt",
  "ex spring",
  "ex button",
  "ex processor",
  "ex red pill",
  "ex radio",
  "ex cache",
  "ex blue transistor",
  "ex antenna",
  "ex screw",
  "ex motherboard",
  "ex A-1920-IXB",
  "ex red transistor",
  "ex keypad",
]