#include <cstdio>

int main(int argc, char* argv[]) {
	if (argc < 2) return 0;
	FILE* fp;
	fopen_s(&fp, argv[1], "rb");
	if (!fp)return 0;
	printf("unsigned char ");
	int i = 0, j;
	for (; argv[1][i] != '.'; i++) {
		printf("%c", argv[1][i]);
	}
	printf("[]={\n");
	unsigned char c[1024];
	for (i = 0; j = fread(c, 1, 1024, fp); i += j) {
		for (int k = 0; k < j; k++)	printf("%d,", c[k]);
	}
	printf("\n};\nsize_t ");
	for (j = 0; argv[1][j] != '.'; j++) {
		printf("%c", argv[1][j]);
	}
	printf("_len=%d;\n", i); fflush(stdout);
	fclose(fp);
}