int findCheapestPrice(int n, int** flights, int flightsSize, int* flightsColSize, int src, int dst, int k) {
    int* dist = (int*)malloc(n * sizeof(int));
    int* temp = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
    }
    dist[src] = 0;

    for (int i = 0; i <= k; i++) {
        memcpy(temp, dist, n * sizeof(int));
        for (int j = 0; j < flightsSize; j++) {
            int u = flights[j][0];
            int v = flights[j][1];
            int price = flights[j][2];

            if (dist[u] != INT_MAX && dist[u] + price < temp[v]) {
                temp[v] = dist[u] + price;
            }
        }
        memcpy(dist, temp, n * sizeof(int));
    }

    int ans = dist[dst] == INT_MAX ? -1 : dist[dst];

    free(dist);
    free(temp);

    return ans;
}
