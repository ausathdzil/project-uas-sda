#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct Graph {
    int num_vertices;
    Node** adj_lists;
} Graph;

Node* create_node(int vertex) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory error\n");
        exit(1);
    }
    new_node->vertex = vertex;
    new_node->next = NULL;

    return new_node;
}

Graph* create_graph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (graph == NULL) {
        printf("Memory error\n");
        exit(1);
    }
    graph->num_vertices = vertices;
    graph->adj_lists = (Node**)malloc(vertices * sizeof(Node*));
    if (graph->adj_lists == NULL) {
        printf("Memory error\n");
        exit(1);
    }

    for (int i = 0; i < vertices; i++) {
        graph->adj_lists[i] = NULL;
    }

    return graph;
}

void add_edge(Graph* graph, int source, int destination) {
    if (source >= graph->num_vertices || destination >= graph->num_vertices) {
        printf("Vertex does not exist\n");
        return;
    }

    Node* new_node = create_node(destination);
    new_node->next = graph->adj_lists[source];
    graph->adj_lists[source] = new_node;

    new_node = create_node(source);
    new_node->next = graph->adj_lists[destination];
    graph->adj_lists[destination] = new_node;
}

void print_graph(Graph* graph) {
    printf("\nAdjacency list\n");
    for (int vertex = 0; vertex < graph->num_vertices; vertex++) {
        Node* temp = graph->adj_lists[vertex];
        printf("vertex %d: ", vertex);
        while (temp) {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

void free_graph(Graph* graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        Node* temp = graph->adj_lists[i];
        while (temp) {
            Node* prev = temp;
            temp = temp->next;
            free(prev);
        }
    }
    free(graph->adj_lists);
    free(graph);
}

int main() {
    Graph* graph = create_graph(5);
    add_edge(graph, 0, 1);
    add_edge(graph, 0, 2);
    add_edge(graph, 0, 3);
    add_edge(graph, 1, 4);

    print_graph(graph);

    free_graph(graph);

    return 0;
}
