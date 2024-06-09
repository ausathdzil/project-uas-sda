#include <stdio.h>
#include <stdlib.h>
#define NUM_STATIONS 5

typedef enum Stations {
  Akihabara,
  Ueno,
  Tokyo,
  Shinjuku,
  Ikebukuro
} Stations;

typedef struct Node {
  int vertex;
  struct Node *next;
} Node;

typedef struct Graph {
  int num_vertices;
  Node **adj_lists;
} Graph;

Node *create_node(int vertex) {
  Node *new_node = (Node *)malloc(sizeof(Node));
  if (new_node == NULL) {
    printf("Memory error\n");
    exit(1);
  }
  new_node->vertex = vertex;
  new_node->next = NULL;

  return new_node;
}

Graph *create_graph(int vertices) {
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  if (graph == NULL) {
    printf("Memory error\n");
    exit(1);
  }

  graph->num_vertices = vertices;
  graph->adj_lists = (Node **)malloc(vertices * sizeof(Node *));
  if (graph->adj_lists == NULL) {
    printf("Memory error\n");
    exit(1);
  }

  for (int i = 0; i < vertices; i++) {
    graph->adj_lists[i] = NULL;
  }

  return graph;
}

void add_edge(Graph *graph, int source, int destination) {
  Node *new_node = create_node(destination);
  if (graph->adj_lists[source] == NULL) {
    graph->adj_lists[source] = new_node;
  } else {
    Node *temp = graph->adj_lists[source];
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = new_node;
  }

  new_node = create_node(source);
  if (graph->adj_lists[destination] == NULL) {
    graph->adj_lists[destination] = new_node;
  } else {
    Node *temp = graph->adj_lists[destination];
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = new_node;
  }
}

void print_graph(Graph *graph) {
  printf("\nadjacency list\n");
  for (int vertex = 0; vertex < graph->num_vertices; vertex++) {
    Node *temp = graph->adj_lists[vertex];
    printf("station %d -> ", vertex);
    while (temp) {
      printf("%d --> ", temp->vertex);
      temp = temp->next;
    }
    printf("NULL\n");
  }
}

void free_graph(Graph *graph) {
  for (int i = 0; i < graph->num_vertices; i++) {
    Node *temp = graph->adj_lists[i];
    while (temp) {
      Node *prev = temp;
      temp = temp->next;
      free(prev);
    }
  }
  free(graph->adj_lists);
  free(graph);
}

int main() {
  Graph *graph = create_graph(NUM_STATIONS);
  Stations stations[NUM_STATIONS] = {Akihabara, Ueno, Tokyo, Shinjuku, Ikebukuro};

  add_edge(graph, stations[0], stations[1]);
  add_edge(graph, stations[0], stations[2]);
  add_edge(graph, stations[1], stations[2]);
  add_edge(graph, stations[2], stations[4]);
  add_edge(graph, stations[3], stations[4]);

  print_graph(graph);
  free_graph(graph);

  return 0;
}
