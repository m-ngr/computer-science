#Name: Mahmoud Elnagar
#Date: 31/10/2021

class Node(object):
    '''Node of the graph, represents the properties of the node
    Currently name'''
    def __init__(self, name) -> None:
        self.name = name
    def __str__(self) -> str:
        return self.name
    def get_name(self) -> str:
        return self.name
class Edge(object):
    def __init__(self, src_node, dst_node) -> None:
        self.src = src_node
        self.dst = dst_node
    def __str__(self) -> str:
        return Node.get_name(self.src) + " -> " + Node.get_name(self.dst)
    def get_source(self) -> Node: return self.src
    def get_destination(self) -> Node: return self.dst 
class Digraph(object):
    def __init__(self) -> None:
        self.edges = {} #empty dictonary
    def add_node(self, node:Node) -> None:
        if node in self.edges:
            raise ValueError("Duplicate Node")
        else:
            self.edges[node] = []
    def add_edge(self, edge:Edge) -> None:
        if edge.get_source() in self.edges and edge.get_destination() in self.edges:
            self.edges[edge.get_source()].append(edge.get_destination())
        else:
            raise ValueError("Node not in the graph")
    def get_destinations(self,node:Node) -> list:
        return self.edges[node]
    def has_node(self, node:Node) -> bool:
        return node in self.edges
    def get_node(self, name:str)->Node:
        for node in self.edges:
            if Node.get_name(node) == name : return node
        raise NameError("node " + name + " not found in graph")
    def __str__(self) -> str:
        result = ""
        for src in self.edges:
            for dst in self.edges[src]:
                result += Edge.__str__(Edge(src,dst)) + "\n"
        return result[:-1]
class Graph(Digraph):
    def add_edge(self, edge: Edge) -> None:
        super().add_edge(edge)
        reverse_edge = Edge(edge.get_destination(), edge.get_source())
        super().add_edge(reverse_edge)

def DFS(graph:Graph, start:Node, end:Node, path:list, shortest:list)->list:
    '''produces a list of the shortest path from start to end recursivly'''
    path += start
    if start == end: return path
    for node in graph.get_destinations(start):
        if node in path: continue #avoid cycles
        if shortest == None or len(path) < len(shortest): 
            new_path = DFS(graph,node,end,path,shortest)
            if new_path != None: shortest = new_path
    return shortest
def BFS(graph:Graph, start:Node, end:Node)->list:
    path = [start]
    path_queue = [path]
    while len(path_queue) != 0:
        tmp_path = path_queue.pop(0)
        last_node = tmp_path[-1]
        if last_node == end: return tmp_path
        for next_node in graph.get_destinations(last_node):
            if next_node not in tmp_path: #avoid cycles
                new_path = tmp_path + [next_node]
                path_queue.append(new_path)
    return None
