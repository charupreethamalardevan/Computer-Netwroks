typedef struct {
    int r, c;
} Point;

int canReach(int** heights, int rows, int cols, int max_effort) {
    int* visited = (int*)calloc(rows * cols, sizeof(int));
    Point* queue = (Point*)malloc(rows * cols * sizeof(Point));
    int head = 0, tail = 0;

    queue[tail++] = (Point){0, 0};
    visited[0] = 1;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (head < tail) {
        Point curr = queue[head++];
        if (curr.r == rows - 1 && curr.c == cols - 1) {
            free(visited);
            free(queue);
            return 1;
        }

        for (int i = 0; i < 4; i++) {
            int nr = curr.r + dr[i];
            int nc = curr.c + dc[i];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (!visited[nr * cols + nc]) {
                    int effort = abs(heights[curr.r][curr.c] - heights[nr][nc]);
                    if (effort <= max_effort) {
                        visited[nr * cols + nc] = 1;
                        queue[tail++] = (Point){nr, nc};
                    }
                }
            }
        }
    }

    free(visited);
    free(queue);
    return 0;
}

int minimumEffortPath(int** heights, int heightsSize, int* heightsColSize) {
    int rows = heightsSize;
    int cols = heightsColSize[0];

    int low = 0, high = 1000000;
    int ans = high;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (canReach(heights, rows, cols, mid)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return ans;
}
[24bcs005@mepcolinux ex4]$cat 743.txt
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
