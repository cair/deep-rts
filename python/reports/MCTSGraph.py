from plotly.graph_objs import *
import plotly

# http://www.lfd.uci.edu/~gohlke/pythonlibs/#python-igraph
# Must be run with 3.5 because of python-igraph

import igraph
from igraph import *

PLOTLY = True
CAIRO = True


def df(tree):
    edges = []
    labels = []
    visited, stack = set(), [tree]
    while stack:
        vertex = stack.pop()
        if vertex.parent: edges.append((vertex.id, vertex.parent.id))
        #labels.append(vertex.id)
        labels.append(vertex.dscore)
        if vertex not in visited:
            visited.add(vertex)
            stack.extend(vertex.children)

    return edges, labels
def bf(tree):
    edges = []
    labels = []


    visited, queue = set(), [tree]
    while queue:
        vertex = queue.pop(0)
        if vertex.parent: edges.append((vertex.id, vertex.parent.id))
        labels.append(vertex.dscore)
        if vertex not in visited:
            visited.add(vertex)
            queue.extend(vertex.children)

    return edges, labels

def generate_tree_data(tree):
    edges, labels = bf(tree)

    num_verticles = len(labels)

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

    if CAIRO:
        igraph.plot(graph, "mcts-graph-cairo.png", layout=layout)
    return Xe, Ye, Xn, Yn, M, position, labels



def draw_tree(tree):

    Xe, Ye, Xn, Yn, M, position, labels = generate_tree_data(tree)



    lines = Scatter(x=Xe,
                       y=Ye,
                       mode='lines',
                       line=dict(color='rgb(210,210,210)', width=1),
                       hoverinfo='none'
                       )

    dots = Scatter(x=Xn,
                      y=Yn,
                      mode='markers',
                      name='',
                      marker=dict(symbol='dot',
                                  size=12,
                                  color='#6175c1',    #'#DB4551',
                                  line=dict(color='rgb(50,50,50)', width=1)
                                  ),
                      text=labels,
                      hoverinfo='text',
                      opacity=0.8
                  )

    axis = dict(showline=False, # hide axis line, grid, ticklabels and  title
                zeroline=False,
                showgrid=False,
                showticklabels=False,
                )

    layout = dict(title='MCTS Generated Graph',
                  annotations=make_annotations(position, labels, M),
                  font=dict(size=24),
                  showlegend=False,
                  xaxis=XAxis(axis),
                  yaxis=YAxis(axis),
                  margin=dict(l=40, r=40, b=85, t=100),
                  hovermode='closest',
                  plot_bgcolor='rgb(248,248,248)'
                  )
    data = Data([lines, dots])
    fig = dict(data=data, layout=layout)
    fig['layout'].update(annotations=make_annotations(position, labels, M))
    plotly.offline.plot(fig, filename='mcts-graph-plotly.html', image='png', auto_open=False)


def make_annotations(pos, text, M, font_size=10, font_color='rgb(250,250,250)'):
    L=len(pos)
    if len(text)!=L:
        raise ValueError('The lists pos and text must have the same len')
    annotations = Annotations()
    for k in range(L):
        annotations.append(
            Annotation(
                text=text[k], # or replace labels with a different list for the text within the circle
                x=pos[k][0], y=2*M-pos[k][1],
                xref='x1', yref='y1',
                font=dict(color=font_color, size=font_size),
                showarrow=False)
        )
    return annotations

