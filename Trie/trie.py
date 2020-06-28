class Trie:
  def __init__(self):
    self.root = TrieNode(None, None, None)
  def insert(self, key, value):
    current = self.root
    for ch in key:
      current = current.lookupOrInsertChild(ch)
    current.value = value
    current.terminator = True

nodes = []

class TrieNode:
  def __init__(self, char, parent, next):
    self.char = char
    self.value = None
    self.parent = parent
    self.next = next
    self.child = None
    self.terminator = False
  def lookupOrInsertChild(self, char):
    if self.child == None or self.child.char > char:
      self.child = TrieNode(char, self, self.child)
      nodes.append(self.child)
      return self.child
    current = self.child
    while current.next != None and current.next.char <= char:
      current = current.next
    if current.char != char:
      current.next = TrieNode(char, self, current.next)
      nodes.append(current.next)
    return current
  def __str__(self):
    return "TrieNode(char=%s, value=%s, terminator=%s, parent=%s)" % (self.char, self.value, self.terminator, self.parent.char if self.parent != None else None)

def main():
  trie = Trie()
  trie.insert("cat", 1)
  trie.insert("catana", 1)
  trie.insert("can", False)
  for node in nodes:
    print(node)

if __name__ == "__main__":
    main()