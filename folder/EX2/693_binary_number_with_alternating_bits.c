bool hasAlternatingBits(int n) {
    int x = n ^ (n >> 1);
    return (x & ((unsigned int)x + 1)) == 0;
}
