import logging

log = logging.getLogger(__name__)

class Struct:
  def __init__(self, **kwargs):
    for k, v in kwargs.iteritems():
      setattr(self, k, v)
  def __repr__(self):
    return str(vars(self))
  def copy(self):
    return Struct(**vars(self))

class Server:
  def __init__(self):
    self.entities = {}
    self.last_entry_id = 0
    self.last_version = 0
  def GenNextId(self):
    self.last_entry_id += 1
    return "s:%d" % (self.last_entry_id)
  def GenNextVersion(self):
    self.last_version += 1
    return self.last_version
  def GetUpdates(self, request, callback):
    log.debug("SS:GetUpdates")
    entities = []
    response = Struct(entities=[], progress_markers={})
    progress_markers = request.progress_markers
    for id, entity in self.entities.iteritems():
      if not entity.type in progress_markers:
        continue
      if entity.version <= progress_markers[entity.type]:
        continue
      entities.append(entity.copy())
    entities = sorted(entities, key=lambda x: x.version)
    page_size = request.page_size if hasattr(request, "page_size") else 25
    entities = entities[:page_size]
    if len(entities) > 0:
      last_version = entities[-1].version
    else:
      last_version = self.last_version
    for dt in progress_markers:
      response.progress_markers[dt] = last_version
    return response

  def Commit(self, request):
    log.debug("SS:Commit")
    response = Struct(entities=[])
    for entity in request.entities:
      entity = entity.copy()
      if entity.id in self.entities:
        assert(entity.type == self.entities[entity.id].type)
      else:
        entity.id = self.GenNextId()
      entity.version = self.GenNextVersion()
      self.entities[entity.id] = entity
      response.entities.append(Struct(id=entity.id, version=entity.version))
    return response

