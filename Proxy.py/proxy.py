import select
import socket

kBufferSize = 4096

class Proxy:
  def __init__(self, listen_addr, forward_addr):
    self.forward_addr = forward_addr
    self.listening_socket = self.StartListening(listen_addr)
    self.sockets = [self.listening_socket]
    self.socket_map = {}

  def StartListening(self, listen_addr):
    listening_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listening_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listening_socket.bind(listen_addr)
    listening_socket.listen(200)
    return listening_socket

  def Run(self):
    while True:
      active_sockets, _, _ = select.select(self.sockets, [], [])
      for s in active_sockets:
        self.ProcessActiveSocket(s)
  def ProcessActiveSocket(self, s):
    if s == self.listening_socket:
      self.AcceptConnection(s)
    else:
      data = s.recv(kBufferSize)
      if len(data) == 0:
        self.CloseConnection(s)
      else:
        self.ForwardData(s, data)

  def AcceptConnection(self, s):
    client_socket, client_addr = s.accept()
    forward_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
      forward_socket.connect(self.forward_addr)
    except Exception as e:
      forward_socket.close()
      client_socket.close()
      return
    print("Accepted connection from %s:%s" % (client_addr))
    self.socket_map[client_socket] = forward_socket
    self.socket_map[forward_socket] = client_socket
    self.sockets.append(client_socket)
    self.sockets.append(forward_socket)
  def CloseConnection(self, s):
    print("ConnectionClosed")
    pair = self.socket_map[s]
    s.close()
    pair.close()
    del self.socket_map[s]
    del self.socket_map[pair]
    self.sockets.remove(s)
    self.sockets.remove(pair)
  def ForwardData(self, s, data):
    print(data)
    pair = self.socket_map[s]
    pair.send(data)

def main():
  proxy = Proxy(("localhost", 2208), ("localhost", 2209))
  proxy.Run()

if __name__ == '__main__':
  main()