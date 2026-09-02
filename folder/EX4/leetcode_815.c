
#define MAX_STOPS 1000000

typedef struct {
    int bus_id;
    int count;
} Node;

int numBusesToDestination(int** routes, int routesSize, int* routesColSize, int source, int target) {
    if (source == target) return 0;

    int* stop_counts = (int*)calloc(MAX_STOPS, sizeof(int));
    for (int i = 0; i < routesSize; i++) {
        for (int j = 0; j < routesColSize[i]; j++) {
            stop_counts[routes[i][j]]++;
        }
    }

    int** stop_to_buses = (int**)malloc(MAX_STOPS * sizeof(int*));
    int* curr_idx = (int*)calloc(MAX_STOPS, sizeof(int));
    for (int i = 0; i < MAX_STOPS; i++) {
        if (stop_counts[i] > 0) {
            stop_to_buses[i] = (int*)malloc(stop_counts[i] * sizeof(int));
        } else {
            stop_to_buses[i] = NULL;
        }
    }

    for (int i = 0; i < routesSize; i++) {
        for (int j = 0; j < routesColSize[i]; j++) {
            int stop = routes[i][j];
            stop_to_buses[stop][curr_idx[stop]++] = i;
        }
    }

    int* visited_buses = (int*)calloc(routesSize, sizeof(int));
    int* visited_stops = (int*)calloc(MAX_STOPS, sizeof(int));

    Node* queue = (Node*)malloc((routesSize + 1) * sizeof(Node));
    int head = 0, tail = 0;

    visited_stops[source] = 1;
    for (int i = 0; i < stop_counts[source]; i++) {
        int bus_id = stop_to_buses[source][i];
        visited_buses[bus_id] = 1;
        queue[tail++] = (Node){bus_id, 1};
    }

    while (head < tail) {
        Node curr = queue[head++];
        int bus_id = curr.bus_id;
        int count = curr.count;

        for (int i = 0; i < routesColSize[bus_id]; i++) {
            int stop = routes[bus_id][i];
            if (stop == target) {
                for (int k = 0; k < MAX_STOPS; k++) free(stop_to_buses[k]);
                free(stop_to_buses); free(stop_counts); free(curr_idx);
                free(visited_buses); free(visited_stops); free(queue);
                return count;
            }

            if (!visited_stops[stop]) {
                visited_stops[stop] = 1;
                for (int j = 0; j < stop_counts[stop]; j++) {
                    int next_bus = stop_to_buses[stop][j];
                    if (!visited_buses[next_bus]) {
                        visited_buses[next_bus] = 1;
                        queue[tail++] = (Node){next_bus, count + 1};
                    }
                }
            }
        }
    }

    for (int k = 0; k < MAX_STOPS; k++) free(stop_to_buses[k]);
    free(stop_to_buses); free(stop_counts); free(curr_idx);
    free(visited_buses); free(visited_stops); free(queue);
    return -1;
}
