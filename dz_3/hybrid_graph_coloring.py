def hybrid_graph_coloring(graph):
    import heapq

    # Preprocessing
    isolated = [v for v in graph if not graph[v]]
    subgraph = {v: neighbors for v, neighbors in graph.items() if neighbors}

    # Initialization
    degrees = {v: len(neighbors) for v, neighbors in subgraph.items()}
    max_degree = max(degrees.values())
    saturation = {v: 0 for v in subgraph}
    coloring = {}

    # Vertex Ordering and Coloring
    def select_vertex():
        """Selects the next vertex based on DSatur and degree."""
        uncolored = [v for v in subgraph if v not in coloring]
        return max(uncolored, key=lambda v: (saturation[v], degrees[v]))

    while len(coloring) < len(subgraph):
        vertex = select_vertex()
        neighbor_colors = {coloring[neighbor] for neighbor in subgraph[vertex] if neighbor in coloring}
        color = 0
        while color in neighbor_colors:
            color += 1
        coloring[vertex] = color

        for neighbor in subgraph[vertex]:
            if neighbor not in coloring:
                saturation[neighbor] = len({coloring[n] for n in subgraph[neighbor] if n in coloring})

    # Postprocessing (Kempe chains optimization)
    num_colors = max(coloring.values()) + 1
    coloring = refine_coloring(subgraph, coloring, num_colors)

    # Combine with isolated vertices
    for v in isolated:
        coloring[v] = 0

    return coloring

def refine_coloring(graph, coloring, num_colors):
    for color1 in range(num_colors):
        for color2 in range(color1 + 1, num_colors):
            kempe_chain = {v for v, c in coloring.items() if c == color1 or c == color2}
            for vertex in kempe_chain:
                swap_color(vertex, coloring, color1, color2, kempe_chain)
    return coloring

def swap_color(vertex, coloring, color1, color2, kempe_chain):
    queue = [vertex]
    visited = set()
    while queue:
        current = queue.pop(0)
        visited.add(current)
        for neighbor in kempe_chain:
            if neighbor not in visited:
                if coloring[neighbor] == color1:
                    coloring[neighbor] = color2
                    queue.append(neighbor)
                elif coloring[neighbor] == color2:
                    coloring[neighbor] = color1
                    queue.append(neighbor)

import unittest

class TestHybridGraphColoring(unittest.TestCase):
    def setUp(self):
        self.test_graphs = {
            "triangle": {  # A simple triangle
                'A': ['B', 'C'],
                'B': ['A', 'C'],
                'C': ['A', 'B']
            },
            "planar_graph": {  # A planar graph
                'A': ['B', 'C', 'D'],
                'B': ['A', 'C', 'E'],
                'C': ['A', 'B', 'D', 'E'],
                'D': ['A', 'C', 'E'],
                'E': ['B', 'C', 'D']
            },
        }

    def is_valid_coloring(self, graph, coloring):
        for vertex, neighbors in graph.items():
            for neighbor in neighbors:
                if coloring[vertex] == coloring.get(neighbor):
                    return False
        return True

    def test_valid_coloring(self):
        for name, graph in self.test_graphs.items():
            with self.subTest(graph=name):
                coloring = hybrid_graph_coloring(graph)
                self.assertTrue(self.is_valid_coloring(graph, coloring),
                                f"Invalid coloring for {name}")

    def test_number_of_colors(self):
        expected_colors = {
            "triangle": 3,           # Triangle requires 3 colors
            "planar_graph": 4        # Planar graph can be colored with at most 4 colors
        }
        for name, graph in self.test_graphs.items():
            with self.subTest(graph=name):
                coloring = hybrid_graph_coloring(graph)
                num_colors = max(coloring.values()) + 1
                self.assertLessEqual(num_colors, expected_colors[name],
                                     f"Too many colors used for {name}")


if __name__ == "__main__":
    unittest.main()