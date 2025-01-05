#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

#define MAX_NODES 1000

// Adjacency list node
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Graph structure
typedef struct Graph {
    int num_nodes;
    Node* adj[MAX_NODES];
} Graph;

// DFS function (recursive)
void dfs(Graph* graph, int node, bool visited[MAX_NODES]) {
    visited[node] = true;
    printf("Visited %d\n", node);
    
    #pragma omp parallel for
    for (Node* neighbor = graph->adj[node]; neighbor != NULL; neighbor = neighbor->next) {
        int neighbor_id = neighbor->data;
        if (!visited[neighbor_id]) {
            #pragma omp critical
            {
                if (!visited[neighbor_id]) {
                    dfs(graph, neighbor_id, visited);
                }
            }
        }
    }
}

// Function to add an edge to the graph
void add_edge(Graph* graph, int src, int dest) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = dest;
    new_node->next = graph->adj[src];
    graph->adj[src] = new_node;
    
    new_node = (Node*)malloc(sizeof(Node));
    new_node->data = src;
    new_node->next = graph->adj[dest];
    graph->adj[dest] = new_node;
}

int main() {
    Graph graph = { .num_nodes = 6 };
    
    // Add edges to the graph
    add_edge(&graph, 0, 1);
    add_edge(&graph, 0, 2);
    add_edge(&graph, 1, 3);
    add_edge(&graph, 1, 4);
    add_edge(&graph, 2, 5);
    
    bool visited[MAX_NODES] = { false };
    printf("DFS Traversal starting from node 0:\n");
    dfs(&graph, 0, visited);
    
    return 0;
}
