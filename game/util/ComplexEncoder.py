import json


class ComplexEncoder(json.JSONEncoder):
    def default(self, obj):
        if hasattr(obj, 'toJSON'):
            return obj.toJSON()
        elif type(obj).__name__ == 'ndarray':
            return json.dumps(obj.tolist())
        elif type(obj).__name__ == 'int64' or type(obj).__name__ == 'int32':
            return int(obj)
        else:

            return json.JSONEncoder.default(self, obj)