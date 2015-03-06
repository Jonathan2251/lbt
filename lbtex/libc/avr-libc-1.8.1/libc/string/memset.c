#define size_t unsigned int

void* memset(void* ptr, int value, size_t num) {
	size_t i;
	unsigned char *p;

	p = (unsigned char *)ptr;
	for (i = 0; i < num; i++) {
		*p = (char)value;
		p++;
	}
	return ptr;
}


