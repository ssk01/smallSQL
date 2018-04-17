#include "util.h"
int Int(char *value) {
	int a;
	memcpy(&a, value, sizeof(a));
	return a;
}
float Float(char *value) {
	float a;
	memcpy(&a, value, sizeof(a));
	return a;
}
