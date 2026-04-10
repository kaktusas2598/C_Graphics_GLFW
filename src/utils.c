#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <sys/stat.h>
#endif

char* loadFileToString(const char* filePath) {
    FILE* file = fopen(filePath, "rb");
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    size_t readSize = fread(buffer, 1, size, file);
    buffer[readSize] = '\0';

    fclose(file);
    return buffer;
}

long getFileLastWriteTime(const char* filePath) {
    FILE* file = fopen(filePath, "rb");
    if (!file)
        return 0;

    #ifdef _WIN32
        struct _stat fileStat;
        _stat(filePath, &fileStat);
        fclose(file);
        return fileStat.st_mtime;
    #else
        struct stat fileStat;
        stat(filePath, &fileStat);
        fclose(file);
        return fileStat.st_mtime;
    #endif
}
