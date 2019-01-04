import collections

Node = collections.namedtuple('Node', ['id', 'duration', 'dependencies'])

def FindCriticalPathForNodes(graph, nodes):
  max_duration = 0
  for node_id in nodes:
    node = graph[node_id]
    node_duration = FindCriticalPathForNode(graph, node)
    if max_duration < node_duration:
      max_duration = node_duration
  return max_duration

def FindCriticalPathForNode(graph, node):
  return node.duration + FindCriticalPathForNodes(graph, node.dependencies)

def FindCriticalPath(graph):
  tail_nodes = FindTailNodes(graph)
  return FindCriticalPathForNodes(graph, tail_nodes)

def FindTailNodes(graph):
  tail_nodes = set(graph.iterkeys())
  for node in graph.itervalues():
    for dep in node.dependencies:
      tail_nodes.discard(dep)
  return tail_nodes

input = [
  Node('a', 10, []),
  Node('b', 20, []),
  Node('c', 10, ['a', 'b'])
]

def main():
  graph = {}
  for n in input:
    graph[n.id] = n
  critical_path = FindCriticalPath(graph)
  print(critical_path)

if __name__ == "__main__":
    main()