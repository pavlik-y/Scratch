import os.path
import subprocess
import time


def main():
  fname = '/home/pi/christmas-lights/christmas.py'
  while True:
    print("start process")
    mtime = os.path.getmtime(fname)
    p = subprocess.Popen(['python', fname])
    while p.poll:
      time.sleep(10)
      if os.path.getmtime(fname) != mtime:
        p.kill()
        break


if __name__ == '__main__':
  main()
