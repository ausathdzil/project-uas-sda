#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define NUM_STATIONS 10

typedef enum Stations {
  Bogor,
  Manggarai,
  Depok,
  Tebet,
  LA,
  PM,
  Citayam,
  Cawang,
  TB,
  PC,
} Stations;

typedef struct Node {
  int vertex;
  struct Node *next;
} Node;

typedef struct Graph {
  int num_vertices;
  Node **adj_lists;
  int* visited;
  int* parent;
} Graph;

Node *create_node(int vertex) {
  Node *new_node = (Node *)malloc(sizeof(Node));
  if (new_node == NULL) {
    printf("memory error\n");
    exit(1);
  }
  new_node->vertex = vertex;
  new_node->next = NULL;

  return new_node;
}

Graph *create_graph(int vertices) {
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  if (graph == NULL) {
    printf("memory error\n");
    exit(1);
  }

  graph->num_vertices = vertices;
  graph->adj_lists = (Node **)malloc(vertices * sizeof(Node *));
  if (graph->adj_lists == NULL) {
    printf("memory error\n");
    exit(1);
  }

  graph->visited = (int *)malloc(vertices * sizeof(int));
  if (graph->visited == NULL) {
    printf("memory error\n");
    exit(1);
  }

  graph->parent = (int *)malloc(vertices * sizeof(int));
  if (graph->parent == NULL) {
    printf("memory error\n");
    exit(1);
  }

  for (int i = 0; i < vertices; i++) {
    graph->adj_lists[i] = NULL;
    graph->visited[i] = 0;
    graph->parent[i] = -1;
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

typedef struct Queue {
  int items[NUM_STATIONS];
  int front;
  int rear;
} Queue;

Queue* create_queue() {
  Queue* queue = (Queue*)malloc(sizeof(Queue));
  if (queue == NULL) {
    printf("memory error\n");
    exit(1);
  }
  queue->front = -1;
  queue->rear = -1;

  return queue;
}

bool is_empty(Queue* queue) {
  return queue->rear == -1;
}

void enqueue(Queue *queue, int value) {
  if (queue->rear == NUM_STATIONS - 1) {
    printf("\nqueue full.");
  } else {
    if (queue->front == -1) {
      queue->front = 0;
    }
    queue->rear++;
    queue->items[queue->rear] = value;
  }
}

int dequeue(Queue* queue) {
  int item;

  if (is_empty(queue)) {
    printf("queue empty.");
    item = -1;
  } else {
    item = queue->items[queue->front];
    queue->front++;

    if (queue->front > queue->rear) {
      queue->front = queue->rear = -1;
    }
  }

  return item;
}

void print_queue(Queue *queue) {
  if (is_empty(queue)) {
    printf("queue empty.");
  } else {
    printf("queue: ");
    for (int i = queue->front; i < queue->rear + 1; i++) {
      printf("%d ", queue->items[i]);
    }
    printf("\n");
  }
}

void bfs_algo(Graph* graph, int start_vertex, int end_vertex) {
  Queue* queue = create_queue();

  graph->visited[start_vertex] = 1;
  enqueue(queue, start_vertex);

  while (!is_empty(queue)) {
    int current_vertex = dequeue(queue);

    if (current_vertex == end_vertex) {
      printf("\nshortest path from %d to %d: ", start_vertex, end_vertex);

      int path[NUM_STATIONS];
      int path_length = 0;

      for (int vertex = end_vertex; vertex != -1; vertex = graph->parent[vertex]) {
        path[path_length++] = vertex;
      }

      for (int i = path_length - 1; i >= 0; i--) {
        if (i == 0) {
          printf("%d", path[i]);
          break;
        }
        printf("%d - ", path[i]);
      }
      printf("\n");
      
      free(queue);

      return;
    }
    
    Node* temp = graph->adj_lists[current_vertex];

    while (temp) {
      int adj_vertex = temp->vertex;

      if (graph->visited[adj_vertex] == 0) {
        graph->visited[adj_vertex] = 1;
        graph->parent[adj_vertex] = current_vertex;
        enqueue(queue, adj_vertex);
      }

      temp = temp->next;
    }
  }

  printf("no path found from %d to %d\n", start_vertex, end_vertex);
  free(queue);
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
  free(graph->visited);
  free(graph->parent);
  free(graph);
}

int main() {
  Graph *graph = create_graph(NUM_STATIONS);
  Stations stations[NUM_STATIONS] = {Bogor, Manggarai, Depok, Tebet, LA, PM, Citayam, Cawang, TB, PC};

  add_edge(graph, stations[Bogor], stations[Depok]);
  add_edge(graph, stations[Bogor], stations[PM]);
  add_edge(graph, stations[Bogor], stations[Cawang]);

  add_edge(graph, stations[Manggarai], stations[Tebet]);
  add_edge(graph, stations[Manggarai], stations[Citayam]);
  add_edge(graph, stations[Manggarai], stations[PC]);

  add_edge(graph, stations[Depok], stations[TB]);
  add_edge(graph, stations[Depok], stations[PC]);

  add_edge(graph, stations[Tebet], stations[Depok]);
  add_edge(graph, stations[Tebet], stations[PM]);
  add_edge(graph, stations[Tebet], stations[Cawang]);
  add_edge(graph, stations[Tebet], stations[PC]);

  add_edge(graph, stations[LA], stations[Manggarai]);
  add_edge(graph, stations[LA], stations[Citayam]);
  
  add_edge(graph, stations[PM], stations[Manggarai]);
  add_edge(graph, stations[PM], stations[TB]);
  add_edge(graph, stations[Cawang], stations[Manggarai]);
  add_edge(graph, stations[Cawang], stations[PC]);
  add_edge(graph, stations[TB], stations[LA]);
  
  print_graph(graph);
  bfs_algo(graph, stations[Depok], stations[LA]);
  free_graph(graph);

  return 0;
}
