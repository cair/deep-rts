


class FastDict(object):


    def __init__(self):
        self.v = dict()

    def __setitem__(self, key, value):
        if key[0] is None or key[1] is None:
            return 0
        self.v[key] = value

    def __getitem__(self, key):
        try:
            v = self.v[key]
            return v
        except:
            return 0

    def __delitem__(self, index):
        del self.v[index]