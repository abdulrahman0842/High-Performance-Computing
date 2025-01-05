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

// Queue structure for BFS
typedef struct Queue {
    int items[MAX_NODES];
    int front, rear;
} Queue;

void init_queue(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

int is_empty(Queue* q) {
    return (q->front == -1);
}

void enqueue(Queue* q, int value) {
    if (q->rear == MAX_NODES - 1)
        return;
    if (q->front == -1)
        q->front = 0;
    q->items[++q->rear] = value;
}

int dequeue(Queue* q) {
    if (is_empty(q))
        return -1;
    int value = q->items[q->front++];
    if (q->front > q->rear)
        q->front = q->rear = -1;
    return value;
}

// BFS function
void bfs(Graph* graph, int start) {
    bool visited[MAX_NODES] = { false };
    Queue q;
    init_queue(&q);
    
    visited[start] = true;
    enqueue(&q, start);
    
    while (!is_empty(&q)) {
        int node = dequeue(&q);
        printf("Visited %d\n", node);
        
        #pragma omp parallel for
        for (Node* neighbor = graph->adj[node]; neighbor != NULL; neighbor = neighbor->next) {
            int neighbor_id = neighbor->data;
            if (!visited[neighbor_id]) {
                #pragma omp critical
                {
                    if (!visited[neighbor_id]) {
                        visited[neighbor_id] = true;
                        enqueue(&q, neighbor_id);
                    }
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
    
    printf("BFS Traversal starting from node 0:\n");
    bfs(&graph, 0);
    
    return 0;
}
