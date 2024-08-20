#define MAGIC 1234567

void * _malloc(int size);

void _free(void *ptr);

void _coalesce();

typedef struct node {
		int          size;
		struct node* next;
} node;

typedef struct {
		int size;
		int magic;
} header;