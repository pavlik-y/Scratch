import pygments
import pygments.lexers

class DocumentModification:
  def __init__(self, start, end, content):
    self.start = start
    self.end = end
    self.content = content

class Document:
  def __init__(self, initial_content):
    self.initial_content = initial_content
    self.content = self.initial_content
    self.document_version = 0
    self.content_version = 0
    self.modifications = []
    self.UpdateLineIndex()

  def ReplaceRange(self, start, end, content):
    assert(start <= len(self.content))
    assert(end <= len(self.content))
    self.modifications = self.modifications[:self.document_version]
    self.modifications.append(DocumentModification(start, end, content))
    self.content = self.content[:start] + content + self.content[end:]
    self.document_version += 1
    self.UpdateLineIndex()
    self.content_version += 1
    return self.document_version

  def RestoreVersion(self, version):
    assert(version <= len(self.modifications))
    self.content = self.initial_content
    self.document_version = 0
    for i in xrange(version):
      m = self.modifications[i]
      self.content = self.content[:m.start] + m.content + self.content[m.end:]
    self.document_version = version
    self.UpdateLineIndex()
    self.content_version += 1

  def UpdateLineIndex(self):
    self.line_offsets = [0]
    self.line_lengths = []
    i = 0
    while True:
      i = self.content.find('\n', i)
      if i == -1:
        break
      i += 1
      self.line_offsets.append(i)
      l = self.line_offsets[-1] - self.line_offsets[-2] - 1
      self.line_lengths.append(l)
    l = len(self.content) - self.line_offsets[-1]
    self.line_lengths.append(l)

  def LineCount(self):
    return len(self.line_lengths)

  def LineLength(self, line):
    return self.line_lengths[line]

  def PosToOffset(self, y, x):
    assert(x <= self.line_lengths[y])
    assert(y < len(self.line_lengths))
    return self.line_offsets[y] + x

  def OffsetToPos(self, offset):
    assert(offset <= len(self.content))
    for y in xrange(len(self.line_offsets) - 1):
      if self.line_offsets[y] <= offset < self.line_offsets[y + 1]:
        return (y, offset - self.line_offsets[y])
    return (len(self.line_offsets) - 1, offset - self.line_offsets[-1])

  def PosToValidPos(self, y, x):
    assert(y < len(self.line_lengths))
    x = min(x, self.LineLength(y))
    return(y, x)

class ParsedDocument:
  def __init__(self, document):
    self.document = document
    self.document_version = -1
    self.lexer = pygments.lexers.get_lexer_by_name("python", ensurenl=False) # {PAV} Get lexer name from buffer properties

  def GetTokens(self):
    self.UpdateIfNeeded()
    return self.tokens

  # def GetLineIndices(self):
  #   self.UpdateIfNeeded()
  #   return self.line_indices

  def UpdateIfNeeded(self):
    if self.document_version != self.document.content_version:
      self.Parse()
      self.document_version = self.document.content_version

  def Parse(self):
    tokens = pygments.lex(self.document.content, self.lexer)
    self.tokens = list(tokens)
    # self.line_indices = []
    # self.line_indices.append(0)
    # i = 0;
    # for (token, value) in self.tokens:
    #   i += 1
    #   if value == '\n':
    #     self.line_indices.append(i)
    #     continue

def TestDocument():
  d = Document("")
  ver1 = d.ReplaceRange(0, 0, "asdf")
  assert(d.content == "asdf")
  d.ReplaceRange(1, 3, "qwer")
  assert(d.content == "aqwerf")
  ver2 = d.ReplaceRange(6, 6, "zx")
  assert(d.content == "aqwerfzx")
  d.RestoreVersion(ver1)
  assert(d.content == "asdf")
  d.RestoreVersion(ver2)
  assert(d.content == "aqwerfzx")
  d = Document("a\nb")
  pd = ParsedDocument(d)
  tokens = pd.GetTokens()
  print(list(tokens))
  print(d.OffsetToPos(3))

def main():
  TestDocument()
if __name__ == '__main__':
  main()