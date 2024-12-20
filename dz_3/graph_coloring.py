def dsatur(graph):
    degrees = {v: len(adj) for v, adj in graph.items()}
    saturation = {v: 0 for v in graph}
    coloring = {}
    
    def choose_vertex():
        uncolored = [v for v in graph if v not in coloring]
        return max(uncolored, key=lambda v: (saturation[v], degrees[v]))
    
    while len(coloring) < len(graph):
        vertex = choose_vertex()
        neighbor_colors = {coloring[neighbor] for neighbor in graph[vertex] if neighbor in coloring}
        color = 0
        while color in neighbor_colors:
            color += 1
        coloring[vertex] = color
        for neighbor in graph[vertex]:
            if neighbor not in coloring:
                saturation[neighbor] += 1
    
    return coloring

graph = {
    'A': ['B', 'C', 'E'],
    'B': ['A', 'C', 'D'],
    'C': ['A', 'B', 'D', 'E'],
    'D': ['B', 'C', 'E'],
    'E': ['A', 'C', 'D']
}

coloring = dsatur(graph)
print(coloring)
