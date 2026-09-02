
typedef struct Edge {
    int to;
    int original_dir;
    struct Edge* next;
} Edge;

int minReorder(int n, int** connections, int connectionsSize, int* connectionsColSize) {
    Edge** graph = (Edge**)calloc(n, sizeof(Edge*));

    for (int i = 0; i < connectionsSize; i++) {
        int u = connections[i][0];
        int v = connections[i][1];

        Edge* e1 = (Edge*)malloc(sizeof(Edge));
        e1->to = v;
        e1->original_dir = 1;
        e1->next = graph[u];
        graph[u] = e1;

        Edge* e2 = (Edge*)malloc(sizeof(Edge));
        e2->to = u;
        e2->original_dir = 0;
        e2->next = graph[v];
        graph[v] = e2;
    }

    int* visited = (int*)calloc(n, sizeof(int));
    int* queue = (int*)malloc(n * sizeof(int));
    int head = 0, tail = 0;

    queue[tail++] = 0;
    visited[0] = 1;

    int changes = 0;

    while (head < tail) {
        int curr = queue[head++];

        Edge* e = graph[curr];
        while (e != NULL) {
            if (!visited[e->to]) {
                visited[e->to] = 1;
                changes += e->original_dir;
                queue[tail++] = e->to;
            }
            e = e->next;
        }
    }

    for (int i = 0; i < n; i++) {
        Edge* e = graph[i];
        while (e != NULL) {
            Edge* tmp = e;
            e = e->next;
            free(tmp);
        }
    }
    free(graph);
    free(visited);
    free(queue);

    return changes;
}
