import unittest

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

def is_valid_coloring(graph, coloring):
    for vertex, neighbors in graph.items():
        for neighbor in neighbors:
            # Adjacent vertices should not have the same color
            if coloring[vertex] == coloring.get(neighbor):
                return False
    return True

def generate_random_graph(num_vertices, max_degree):
    import random
    graph = {i: set() for i in range(num_vertices)}
    for vertex in range(num_vertices):
        neighbors = random.sample(range(num_vertices), random.randint(0, max_degree))
        for neighbor in neighbors:
            if neighbor != vertex:  # Avoid self-loops
                graph[vertex].add(neighbor)
                graph[neighbor].add(vertex)  # Ensure symmetry
    return {k: list(v) for k, v in graph.items()}

class TestDSatur(unittest.TestCase):
    def setUp(self):
        self.test_graphs = {
            "triangle": {  # A simple triangle
                'A': ['B', 'C'],
                'B': ['A', 'C'],
                'C': ['A', 'B']
            },
            "square": {  # A square with a diagonal
                'A': ['B', 'D', 'C'],
                'B': ['A', 'C'],
                'C': ['A', 'B', 'D'],
                'D': ['A', 'C']
            },
            "tree": {  # A tree structure
                'A': ['B', 'C'],
                'B': ['A', 'D', 'E'],
                'C': ['A'],
                'D': ['B'],
                'E': ['B']
            },
            "clique": {  # A complete graph K5
                'A': ['B', 'C', 'D', 'E'],
                'B': ['A', 'C', 'D', 'E'],
                'C': ['A', 'B', 'D', 'E'],
                'D': ['A', 'B', 'C', 'E'],
                'E': ['A', 'B', 'C', 'D']
            }
        }

    def test_valid_coloring(self):
        for name, graph in self.test_graphs.items():
            with self.subTest(graph=name):
                coloring = dsatur(graph)
                self.assertTrue(is_valid_coloring(graph, coloring), f"Invalid coloring for {name}")

    def test_expected_colors(self):
        expected_colors = {
            "triangle": 3,  # Triangle requires 3 colors
            "square": 3,    # A square with a diagonal can be colored with 3 colors
            "tree": 2,      # A tree is bipartite and requires 2 colors
            "clique": 5     # A K5 complete graph requires 5 colors
        }
        for name, graph in self.test_graphs.items():
            with self.subTest(graph=name):
                coloring = dsatur(graph)
                max_color_used = max(coloring.values()) + 1
                self.assertEqual(max_color_used, expected_colors[name], 
                                 f"Unexpected number of colors for {name}")

    def test_large_graph(self):
        large_graph = large_graph = generate_random_graph(100, 5)
        coloring = dsatur(large_graph)
        self.assertTrue(is_valid_coloring(large_graph, coloring), "Invalid coloring for large graph")


if __name__ == "__main__":
    unittest.main()
