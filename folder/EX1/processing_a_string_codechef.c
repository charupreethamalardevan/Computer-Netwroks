#include <stdio.h>

int main() {
	// your code goes here
    int T;
    scanf("%d", &T);

    while (T--) {
        char s[1001];
        scanf("%s", s);

        int sum = 0;

        for (int i = 0; s[i] != '\0'; i++) {
            if (isdigit(s[i])) {
                sum += s[i] - '0';
            }
        }

        printf("%d\n", sum);
    }

    return 0;
}

