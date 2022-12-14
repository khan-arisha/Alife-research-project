#!/usr/bin/python3
"""
Generate toroidal graphs.

This script makes use of NetworkX to generate
toroidal graphs (nodes are connected akin to a
torus).

This tool creates adjacency list files (.adj)
whose filename represent the characteristics
of the graph created.
"""

from keyname import keyname as kn
import networkx as nx

dims = [
    [1, 1],
    [2, 2],
    [3, 3],
    [8, 8],
    [9, 9]
]

def make_filename(dims):
    pack = {
        **{
            'name'  : 'toroidal_grid',
            'ndims' : len(dims),
            'ext'   : '.adj',
        },
        **{'dim' + str(i) : dim for i, dim in enumerate(dims)},
    }

    return kn.pack(pack)

for dim in dims:
    G_torus = nx.grid_graph(dim=dim, periodic=True).to_directed()

    nodes = sorted(list(G_torus.nodes()))
    map = {edge : idx for idx, edge in enumerate(nodes)}

    nx.relabel_nodes(G_torus, mapping=map, copy=False)

    with open("assets/" + make_filename(dim), "w") as file:
        for line in nx.generate_adjlist(G_torus):
            file.write(line + '\n')
