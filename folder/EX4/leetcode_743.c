int networkDelayTime(int** times, int timesSize, int* timesColSize, int n, int k) {
    int dist[101];
    for (int i = 1; i <= n; i++) {
        dist[i] = INT_MAX;
    }
    dist[k] = 0;

    for (int i = 1; i < n; i++) {
        int updated = 0;
        for (int j = 0; j < timesSize; j++) {
            int u = times[j][0];
            int v = times[j][1];
            int w = times[j][2];
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                updated = 1;
            }
        }
        if (!updated) break;
    }

    int max_time = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INT_MAX) return -1;
        if (dist[i] > max_time) {
            max_time = dist[i];
        }
    }

    return max_time;
}
