#include <stdio.h>
#include <sys/types.h>

int renameat2(int olddirfd, const char *oldpath, int newdirfd,
              const char *newpath, unsigned int flags);
int truncate64(const char *path, __off64_t length);
int ftruncate64(int fd, __off64_t length);
int open64(const char *pathname, int flags, ...);
FILE *fopen64(const char *pathname, const char *mode);
int creat64(const char *pathname, __mode_t mode);
