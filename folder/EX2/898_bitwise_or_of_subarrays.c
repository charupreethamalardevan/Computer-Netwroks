int cmp(const void *a, const void *b) { return (*(int*)a - *(int*)b); }

int subarrayBitwiseORs(int* arr, int arrSize) {
    int* res = (int*)malloc(arrSize * 32 * sizeof(int));
    int cur[32], next[32], r_cnt = 0, c_size = 0;

    for (int i = 0; i < arrSize; i++) {
        int x = arr[i], n_size = 0;
        next[n_size++] = x;
        res[r_cnt++] = x;

        for (int j = 0; j < c_size; j++) {
            int val = cur[j] | x;
            if (next[n_size - 1] != val) {
                next[n_size++] = val;
                res[r_cnt++] = val;
            }
        }
        c_size = n_size;
        for (int j = 0; j < c_size; j++) cur[j] = next[j];
    }

    qsort(res, r_cnt, sizeof(int), cmp);
    int ans = (r_cnt > 0) ? 1 : 0;
    for (int i = 1; i < r_cnt; i++) {
        if (res[i] != res[i - 1]) ans++;
    }
    free(res);
    return ans;
}
