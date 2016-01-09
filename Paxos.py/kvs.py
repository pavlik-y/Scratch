class SetValueRequest:
    def __init__(self, key, value, src):
        self.key = key
        self.value = value
        self.src = src
class GetValueRequest:
    def __init__(self, key, src):
        self.key = key
        self.src = src
class GenericResponse:
    def __init__(self, status, value = None):
        self.status = status
        self.value = value
    def __repr__(self):
        return "GenericResponse(%s, %s)" % (self.status, self.value)

class KVSState:
    def __init__(self):
        self.values = {}
    def HandleRequest(self, req):
        if isinstance(req, SetValueRequest):
            self._SetValue(req.key, req.value)
            resp = GenericResponse("Ok")
        elif isinstance(req, GetValueRequest):
            try:
                value = self._GetValue(req.key)
                resp = GenericResponse("Ok", value)
            except KeyError:
                resp = GenericResponse("KeyError", None)
        else:
            assert(False)
        return resp
    def _GetValue(self, key):
        return self.values[key]
    def _SetValue(self, key, value):
        self.values[key] = value
        
def _Test_KVSState():
    kvs = KVSState()
    req = SetValueRequest("abra", "cadabra", "src")
    resp = kvs.HandleRequest(req)
    assert(resp.status == "Ok")
    req = GetValueRequest("abra", "src")
    resp = kvs.HandleRequest(req)
    assert(resp.status == "Ok" and resp.value == "cadabra")
    req = GetValueRequest("qwerty", "src")
    resp = kvs.HandleRequest(req)
    assert(resp.status == "KeyError")

if __name__ == "__main__":
    _Test_KVSState()
