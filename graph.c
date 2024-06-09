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

typedef struct Queue {
  int items[NUM_STATIONS];
  int front;
  int rear;
} Queue;

typedef struct Node {
  int vertex;
  struct Node *next;
} Node;

typedef struct Graph {
  int num_vertices;
  Node **adj_lists;
  int* visited;
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

  graph->visited = (int *)malloc(vertices * sizeof(int));
  if (graph->visited == NULL) {
    printf("Memory error\n");
    exit(1);
  }

  for (int i = 0; i < vertices; i++) {
    graph->adj_lists[i] = NULL;
    graph->visited[i] = 0;
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

Queue* create_queue() {
  Queue* queue = (Queue*)malloc(sizeof(Queue));
  if (queue == NULL) {
    printf("Memory error\n");
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
      printf("\nResetting queue\n");
      queue->front = queue->rear = -1;
    }
  }

  return item;
}

void print_queue(Queue *queue) {
  if (is_empty(queue)) {
    printf("queue empty.");
  } else {
    printf("\nqueue: \n");
    for (int i = queue->front; i < queue->rear + 1; i++) {
      printf("%d ", queue->items[i]);
    }
    printf("\n");
  }
}

void bfs_algo(Graph* graph, int start_vertex) {
  Queue* queue = create_queue();

  graph->visited[start_vertex] = 1;
  enqueue(queue, start_vertex);

  while (!is_empty(queue)) {
    int current_vertex = dequeue(queue);
    printf("\nVisited %d\n", current_vertex);
    
    Node* temp = graph->adj_lists[current_vertex];

    while (temp) {
      int adj_vertex = temp->vertex;

      if (graph->visited[adj_vertex] == 0) {
        graph->visited[adj_vertex] = 1;
        enqueue(queue, adj_vertex);
      }

      temp = temp->next;
    }
  }

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
  free(graph);
}

int main() {
  Graph *graph = create_graph(NUM_STATIONS);
  Stations stations[NUM_STATIONS] = {Bogor, Manggarai, Depok, Tebet, LA, PM, Citayam, Cawang, TB, PC};

  // rute st bogor
  add_edge(graph, stations[0], stations[2]); // bogor <-> depok
  add_edge(graph, stations[0], stations[5]); // bogor <-> PM
  add_edge(graph, stations[0], stations[7]); // bogot <-> cawang

  // rute st manggarai
  add_edge(graph, stations[1], stations[3]); // manggarai <-> tebet
  add_edge(graph, stations[1], stations[6]); // manggarai <-> citayam
  add_edge(graph, stations[1], stations[9]); // manggarai <-> PC

  // rute st depok
  add_edge(graph, stations[2], stations[8]); // depok <-> TB
  add_edge(graph, stations[2], stations[9]); // depok <-> PC

  // rute st tebet
  add_edge(graph, stations[3], stations[2]); // tebet <-> depok
  add_edge(graph, stations[3], stations[5]); // tebet <-> PM
  add_edge(graph, stations[3], stations[7]); // tebet <-> cawang
  add_edge(graph, stations[3], stations[9]); // tebet <-> PC

  // rute st LA
  add_edge(graph, stations[4], stations[1]); // LA <-> manggarai
  add_edge(graph, stations[4], stations[6]); // LA <-> citayam
  
  // rute st PM
  add_edge(graph, stations[5], stations[1]); // PM <-> manggarai
  add_edge(graph, stations[5], stations[8]); // PM <-> TB
  
  // rute st Cawang
  add_edge(graph, stations[7], stations[1]); // cawan <-> manggarai
  add_edge(graph, stations[7], stations[9]); // cawan <-> PC
  
  // rute st TB
  add_edge(graph, stations[8], stations[4]); // TB <-> LA
  
  bfs_algo(graph, stations[0]);
  free_graph(graph);

  return 0;
}
