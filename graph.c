#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

const char *station_names[NUM_STATIONS] = {
  "Bogor",
  "Manggarai",
  "Depok",
  "Tebet",
  "Lenteng Agung",
  "Pasar Minggu",
  "Citayam",
  "Cawang",
  "Tanjung Barat",
  "Pondok Cina"
};

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
  graph->visited = (int *)malloc(vertices * sizeof(int));
  graph->parent = (int *)malloc(vertices * sizeof(int));
  if (graph->adj_lists == NULL || graph->visited == NULL || graph->parent == NULL) { 
    printf("Memory error\n");
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

typedef struct Queue {
  int items[NUM_STATIONS];
  int front;
  int rear;
} Queue;

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
    printf("\nQueue full.");
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
    printf("Queue empty.");
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

void bfs_algo(Graph* graph, int start_vertex, int end_vertex) {
  Queue* queue = create_queue();

  graph->visited[start_vertex] = 1;
  enqueue(queue, start_vertex);

  while (!is_empty(queue)) {
    int current_vertex = dequeue(queue);

    Node* temp = graph->adj_lists[current_vertex];
    while (temp) {
      int adj_vertex = temp->vertex;

      if (graph->visited[adj_vertex] == 0) {
        graph->visited[adj_vertex] = 1;
        graph->parent[adj_vertex] = current_vertex;
        enqueue(queue, adj_vertex);

        if (adj_vertex == end_vertex) {
          printf("\nShortest path from %s to %s: ", station_names[start_vertex], station_names[end_vertex]);

          int path[NUM_STATIONS];
          int path_length = 0;

          for (int vertex = end_vertex; vertex != -1; vertex = graph->parent[vertex]) {
            path[path_length++] = vertex;
          }

          for (int i = path_length - 1; i >= 0; i--) {
            if (i == 0) {
              printf("%s", station_names[path[i]]);
              break;
            }
            printf("%s - ", station_names[path[i]]);
          }
          printf("\nNumber of stations traversed: %d\n", path_length - 1);

          free(queue);
          return;
        }
      }
      temp = temp->next;
    }
  }

  printf("No path found from %s to %s\n", station_names[start_vertex], station_names[end_vertex]);
  free(queue);
}

void reset_visited(Graph *graph) {
  for (int i = 0; i < graph->num_vertices; i++) {
    graph->visited[i] = 0;
    graph->parent[i] = -1;
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
  free(graph->visited);
  free(graph->parent);
  free(graph);
}

int get_station_index(const char *name) {
  for (int i = 0; i < NUM_STATIONS; i++) {
    if (strcmp(station_names[i], name) == 0) {
      return i;
    }
  }
  return -1;
}

void display_routes(Graph *graph) {
  printf("\nStations adjacency list:\n");
  for (int vertex = 0; vertex < graph->num_vertices; vertex++) {
    Node *temp = graph->adj_lists[vertex];
    printf("%2d. %s -> ", vertex + 1, station_names[vertex]);
    while (temp) {
      if (temp->next == NULL) {
        printf("%s", station_names[temp->vertex]);
        break;
      }
      printf("%s --> ", station_names[temp->vertex]);
      temp = temp->next;
    }
    printf("\n");
  }
}

void display_stations() {
  printf("\nAvailable stations:\n");
  for (int i = 0; i < NUM_STATIONS; i++) {
    printf("%2d. %s\n", i + 1, station_names[i]);
  }
}

void find_routes(Graph *graph) {
  int choice;
  do {
    display_stations();

    char start_station[50], end_station[50];
    printf("\nEnter the start station: ");
    scanf(" %[^\n]s", start_station);
    printf("\nEnter the end station: ");
    scanf(" %[^\n]s", end_station);

    int start_vertex = get_station_index(start_station);
    int end_vertex = get_station_index(end_station);

    if (start_vertex == -1 || end_vertex == -1) {
      printf("Invalid station name entered.\n");
    } else if (start_vertex == end_vertex) {
      printf("Start and end stations are the same. No need to traverse.\n");
    } else {
      bfs_algo(graph, start_vertex, end_vertex);
    }

    printf("\n1. Find another route\n");
    printf("2. Back to menu\n");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    reset_visited(graph);
  } while (choice != 2);
}

int main() {
  Graph *graph = create_graph(NUM_STATIONS);

  add_edge(graph, Bogor, Depok);
  add_edge(graph, Bogor, PM);
  add_edge(graph, Bogor, Cawang);

  add_edge(graph, Manggarai, Tebet);
  add_edge(graph, Manggarai, Citayam);
  add_edge(graph, Manggarai, PC);

  add_edge(graph, Depok, TB);
  add_edge(graph, Depok, PC);

  add_edge(graph, Tebet, Depok);
  add_edge(graph, Tebet, PM);
  add_edge(graph, Tebet, Cawang);
  add_edge(graph, Tebet, PC);

  add_edge(graph, LA, Manggarai);
  add_edge(graph, LA, Citayam);

  add_edge(graph, PM, Manggarai);
  add_edge(graph, PM, TB);
  add_edge(graph, Cawang, Manggarai);
  add_edge(graph, Cawang, PC);
  add_edge(graph, TB, LA);

  printf("\nStation route finder\n");
  display_stations();

  int choice;
  do {
    printf("\n1. Display routes\n");
    printf("2. Find route\n");
    printf("3. Exit\n");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      display_routes(graph);
      break;
    
    case 2:
      find_routes(graph);
      break;

    case 3:
      break;

    default:
      printf("Invalid choice\n");
      break;
    }

  } while (choice != 3);

  free_graph(graph);

  return 0;
}
