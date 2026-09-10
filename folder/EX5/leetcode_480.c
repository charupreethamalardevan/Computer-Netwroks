
typedef struct {
    int val;
    int id;
} Node;

typedef struct {
    Node* data;
    int size;
    int capacity;
    int isMinHeap;
} Heap;

typedef struct {
    int id;
    int count;
} HashEntry;

Heap* createHeap(int cap, int isMinHeap) {
    Heap* h = (Heap*)malloc(sizeof(Heap));
    h->data = (Node*)malloc((cap + 1) * sizeof(Node));
    h->size = 0;
    h->capacity = cap;
    h->isMinHeap = isMinHeap;
    return h;
}

int compare(Node a, Node b, int isMinHeap) {
    if (a.val != b.val) {
        return isMinHeap ? (a.val > b.val ? 1 : -1) : (a.val < b.val ? 1 : -1);
    }
    return a.id > b.id ? 1 : -1;
}

void swap(Node* a, Node* b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void push(Heap* h, int val, int id) {
    h->size++;
    h->data[h->size].val = val;
    h->data[h->size].id = id;
    int i = h->size;
    while (i > 1 && compare(h->data[i / 2], h->data[i], h->isMinHeap) > 0) {
        swap(&h->data[i / 2], &h->data[i]);
        i /= 2;
    }
}

void pop(Heap* h) {
    if (h->size == 0) return;
    h->data[1] = h->data[h->size];
    h->size--;
    int i = 1;
    while (2 * i <= h->size) {
        int left = 2 * i;
        int right = 2 * i + 1;
        int best = left;
        if (right <= h->size && compare(h->data[left], h->data[right], h->isMinHeap) > 0) {
            best = right;
        }
        if (compare(h->data[i], h->data[best], h->isMinHeap) <= 0) break;
        swap(&h->data[i], &h->data[best]);
        i = best;
    }
}

Node top(Heap* h) {
    return h->data[1];
}

int hashFunc(int id, int tableSize) {
    unsigned int h = (unsigned int)id;
    return h % tableSize;
}

void hashInsert(HashEntry* table, int tableSize, int id) {
    int idx = hashFunc(id, tableSize);
    while (table[idx].count != 0 && table[idx].id != id) {
        idx = (idx + 1) % tableSize;
    }
    table[idx].id = id;
    table[idx].count++;
}

void prune(Heap* h, HashEntry* table, int tableSize) {
    while (h->size > 0) {
        Node t = top(h);
        int idx = hashFunc(t.id, tableSize);
        while (table[idx].count != 0 && table[idx].id != t.id) {
            idx = (idx + 1) % tableSize;
        }
        if (table[idx].count > 0 && table[idx].id == t.id) {
            table[idx].count--;
            pop(h);
        } else {
            break;
        }
    }
}

double* medianSlidingWindow(int* nums, int numsSize, int k, int* returnSize) {
    *returnSize = numsSize - k + 1;
    double* result = (double*)malloc((*returnSize) * sizeof(double));
    
    Heap* small = createHeap(numsSize, 0); 
    Heap* large = createHeap(numsSize, 1); 
    
    int tableSize = numsSize * 2;
    HashEntry* table = (HashEntry*)calloc(tableSize, sizeof(HashEntry));
    int* inLarge = (int*)calloc(numsSize, sizeof(int));
    
    int smallSize = 0;
    int largeSize = 0;
    
    for (int i = 0; i < k; i++) {
        push(small, nums[i], i);
        smallSize++;
    }
    
    int toMove = k / 2;
    for (int i = 0; i < toMove; i++) {
        Node t = top(small);
        pop(small);
        push(large, t.val, t.id);
        inLarge[t.id] = 1;
        smallSize--;
        largeSize++;
    }
    
    if (k % 2 == 1) {
        result[0] = (double)top(small).val;
    } else {
        result[0] = ((double)top(small).val + (double)top(large).val) / 2.0;
    }
    
    int resIdx = 1;
    for (int i = k; i < numsSize; i++) {
        int outId = i - k;
        hashInsert(table, tableSize, outId);
        
        if (inLarge[outId]) {
            largeSize--;
        } else {
            smallSize--;
        }
        
        prune(small, table, tableSize);
        prune(large, table, tableSize);
        
        if (small->size > 0 && nums[i] <= top(small).val) {
            push(small, nums[i], i);
            smallSize++;
        } else {
            push(large, nums[i], i);
            inLarge[i] = 1;
            largeSize++;
        }
        
        int targetSmallSize = (k + 1) / 2;
        while (smallSize < targetSmallSize) {
            Node t = top(large);
            pop(large);
            push(small, t.val, t.id);
            inLarge[t.id] = 0;
            smallSize++;
            largeSize--;
            prune(large, table, tableSize);
        }
        while (smallSize > targetSmallSize) {
            Node t = top(small);
            pop(small);
            push(large, t.val, t.id);
            inLarge[t.id] = 1;
            smallSize--;
            largeSize++;
            prune(small, table, tableSize);
        }
        
        prune(small, table, tableSize);
        prune(large, table, tableSize);
        
        if (k % 2 == 1) {
            result[resIdx++] = (double)top(small).val;
        } else {
            result[resIdx++] = ((double)top(small).val + (double)top(large).val) / 2.0;
        }
    }
    
    free(small->data);
    free(small);
    free(large->data);
    free(large);
    free(table);
    free(inLarge);
    
    return result;
}
