import collections
import sys

Instance = collections.namedtuple('Instance',
    ['id', 'type', 'size', 'reachable', 'soft_reference', 'references'])

def ParseInstance(line):
  metadata, references = line.strip().split(':', 1)
  id, type, size, reachable, soft_reference = metadata.split(',', 4)
  id, type, soft_reference = id.strip(), type.strip(), soft_reference.strip()
  size = int(size)
  reachable = (reachable == 'true')
  references = map(lambda r: r.strip(), references.split(','))
  references = set(references)
  references.discard('')
  return Instance(id, type, size, reachable, soft_reference, list(references))

class Snapshot:
  def __init__(self, instances):
    self.instances = instances
    self.inst_map = dict([(inst.id, inst) for inst in self.instances])
  def Inst(self, id):
    return self.inst_map[id]
  def Instances(self):
    return self.instances
  def EmitDirectReferences(self, ids):
    for id in ids:
      inst = self.inst_map[id]
      for ref in inst.references:
        yield ref

  def ExpandClosedCloud(self, cloud):
    result = cloud.copy()
    closed_references = result
    while True:
      closed_references = set(self.EmitDirectReferences(closed_references))
      closed_references -= result
      for inst in self.Instances():
        if inst.id in result:
          continue
        closed_references -= set(inst.references)
      if not closed_references:
        break
      result |= closed_references
    return result

  def ExpandSet(self, cloud, iterations = None):
    result = cloud.copy()
    references = result
    while True:
      references = set(self.EmitDirectReferences(references))
      references -= result
      if not references:
        break
      result |= references
      if iterations != None:
        iterations -= 1
        if iterations == 0:
          break
    return result

  def EmitExternalReferences(self, cloud):
    for inst in self.Instances():
      if inst.id in cloud:
        continue
      for ref in inst.references:
        if ref in cloud:
          yield (inst.id, ref)

  def FilterByType(self, ids, type):
    for id in ids:
      if self.inst_map[id].type == type:
        yield id
  def FilterByTypePrefix(self, ids, type_prefix):
    for id in ids:
      if self.inst_map[id].type.startswith(type_prefix):
        yield id
  def Ids(self):
    return self.inst_map.iterkeys()
  def GetType(self, id):
    return self.inst_map[id].type

  def BuildTypeCounts(self, ids):
    types = collections.defaultdict(int)
    for id in ids:
      types[self.Inst(id).type] += 1
    types = sorted(types.iteritems(), key=lambda t: t[1])
    return types

def PrintExternalReferences(snapshot, feed_cloud):
  external_references = set(
      [(snapshot.GetType(src), snapshot.GetType(dst))
       for (src, dst) in snapshot.EmitExternalReferences(feed_cloud)])
  print("\n".join([str(e) for e in external_references]))
  sources = set([r[0] for r in external_references])

def PrintTypeSizesAndCounts(snapshot, feed_cloud):
  # Calculate type sizes and counts
  type_counts = collections.defaultdict(int)
  type_sizes = collections.defaultdict(int)
  types = set()
  total_size = 0
  total_count = 0
  for id in feed_cloud:
    inst = snapshot.Inst(id)
    type_counts[inst.type] += 1
    type_sizes[inst.type] += inst.size
    types.add(inst.type)
    total_count += 1
    total_size += inst.size
  result = []
  for t in types:
    result.append((t, type_counts[t], type_sizes[t]))
  result = sorted(result, key=lambda x: x[2])
  print("\n".join([str(r) for r in result]))
  print(total_count, total_size)

def GenerateObjectGraph(snapshot, root, levels=2):
  ids = snapshot.ExpandSet(set([root]), iterations=levels)
  edges = set()
  for id in ids:
    inst = snapshot.Inst(id)
    for ref in inst.references:
      if ref in ids:
        edges.add((id, ref))
  for id in ids:
    print('object "%s@%s" as id%s' % (snapshot.Inst(id).type, id, id))
  for edge in edges:
    print('id%s --> id%s' % edge)


def main():
  with open(sys.argv[1], 'rt') as f:
    lines = f.readlines()
  instances = [ParseInstance(l) for l in lines]
  snapshot = Snapshot(instances)

  # feed_inst_ids = set(snapshot.FilterByTypePrefix(
  #     snapshot.Ids(), 'com.google.android.libraries.feed'))
  # feed_cloud = snapshot.ExpandClosedCloud(feed_inst_ids)

  # PrintTypeSizesAndCounts(snapshot, feed_cloud)
  # PrintExternalReferences(snapshot, feed_cloud)
  # inst = snapshot.Inst('12ded780')
  # print(inst)
  # print(set(inst.references))
  GenerateObjectGraph(snapshot, '12c229c0', levels=2)


if __name__ == "__main__":
 main()