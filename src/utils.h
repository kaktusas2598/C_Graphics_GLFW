#pragma once

char* loadFileToString(const char* filePath);
// Get timestamp for the last modified filetime, useful to implement simple shader hot-reloading
long getFileLastWriteTime(const char* filePath);
