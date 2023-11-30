
int strcmp(char* str1, char* str2) {
    char *p1 = str1;
    char *p2 = str2;
    while (1) {
        char c1 = *p1;
        char c2 = *p2;
        if (c1 ==  c2) {
            // reach end of string
            if (c1 == '\0') {
                return 1;
            }
        }else {
            return 0;
        }
        p1++;
        p2++;
    }
}