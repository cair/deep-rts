# Copyright 2017 Per-Arne Andersen
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject
# to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# AND ONE LAST THING.. THIS LIBRARY IS ACTUALLY AWESOME

import sys
import modulefinder
import argparse
from collections import OrderedDict


class DependencyGraph(modulefinder.ModuleFinder):

    def __init__(self,filter, *args,**kwargs):

        self.node_mapping = {}
        self.labels = []
        self.edges_str = []
        self.edges = []
        self.nodes = []
        self.filter = filter



        self._depgraph = {}
        self._types = {}
        self._last_caller = None
        modulefinder.ModuleFinder.__init__(self,*args,**kwargs)
        
    def import_hook(self, name, caller=None, fromlist=None, level=None):
        old_last_caller = self._last_caller
        try:
            self._last_caller = caller
            return modulefinder.ModuleFinder.import_hook(self,name,caller,fromlist)
        except:
            pass
        finally:
            self._last_caller = old_last_caller
            
    def import_module(self, partnam, fqname, parent):
        r = modulefinder.ModuleFinder.import_module(self,partnam,fqname,parent)

        if r is not None:
            self.edges_str.append((self._last_caller.__name__, r.__name__))
            #self._depgraph.setdefault(self._last_caller.__name__ , {})[r.__name__] = 1
        return r
    
    def load_module(self, fqname, fp, pathname, arg):
        suffix, mode, type = arg
        r = modulefinder.ModuleFinder.load_module(self, fqname, fp, pathname, (suffix, mode, type))
        return r

def to_image(edges_str):
    import networkx as nx
    import matplotlib.pyplot as plt
    edges = []

    for l, r in edges_str:
        if not l.startswith(tuple(['game'])) or not r.startswith(tuple(['game'])):
            continue
        edges.append((l, r))


    G = nx.DiGraph()
    G.add_edges_from(edges)

    labels = {x: x for x in G.nodes()}
    plt.figure(3,figsize=(12,12))
    pos = nx.spring_layout(G,k=2,iterations=20)
    # k controls the distance between the nodes and varies between 0 and 1
    # iterations is the number of times simulated annealing is run
    # default k =0.1 and iterations=50
    nx.draw_networkx_labels(G,pos, labels, font_size=12, font_color='green')
    nx.draw_networkx_nodes(G, pos)
    nx.draw_networkx_edges(G, pos, edgelist=edges, default='black')

    plt.show()

    """
    num_verticles = len(node_map)

    graph = igraph.Graph(edges, directed=False)
    graph.vs["label"] = labels

    layout = graph.layout('rt', mode="IN", root=[0])

    position = {k: layout[k] for k in range(num_verticles)}

    Y = [position[k][1] for k in range(num_verticles)]
    M = max(Y)
    E = [e.tuple for e in graph.es] # list of edges

    L = len(position)
    Xn = [position[k][0] for k in range(L)]
    Yn = [2*M-position[k][1] for k in range(L)]
    Xe = []
    Ye = []
    for edge in E:
        Xe+=[position[edge[0]][0],position[edge[1]][0], None]
        Ye+=[2*M-position[edge[0]][1],2*M-position[edge[1]][1], None]

    igraph.plot(graph, "./letsgo.png", layout=layout)
    """



def main(arg):
    path = sys.path[:]
    filter = arg.package.split(",")
    exclusion = []
    file = arg.file
    output = arg.out

    depG = DependencyGraph(filter, path, 0, exclusion)
    depG.run_script(file)



    # Calculate node list

    if output == "image":
        to_image(depG.edges_str)






if __name__=='__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('--out', help='"image" or "stdout". Image requires cairo', default='image')
    parser.add_argument('--package', help='"example:  game.mechanics,game"', default='')
    parser.add_argument('--file', help='Which file to analyze')
    args = parser.parse_args()

    main(args)