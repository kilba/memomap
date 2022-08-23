#include <windows.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    void* hFileMap;
    void* pData;
    char MapName[256];
    size_t Size;
} SharedMemory;

typedef struct {
    char index;

    float PosX;
    float PosY;
    float PosZ;
} IPC_Data;
IPC_Data *data = NULL;

bool initMemoryMap(SharedMemory* shm) {
    if ((shm->hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, shm->Size, shm->MapName)) == NULL) {
        return false;
    }

    if ((shm->pData = MapViewOfFile(shm->hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, shm->Size)) == NULL) {
        CloseHandle(shm->hFileMap);
        return false;
    }
    return true;
}

bool freeMemoryMap(SharedMemory* shm) {
    if (shm && shm->hFileMap) {
        if (shm->pData) {
            UnmapViewOfFile(shm->pData);
        }

        if (shm->hFileMap) {
            CloseHandle(shm->hFileMap);
        }
        return true;
    }
    return false;
}

void memomap_init() {
    SharedMemory shm = {0};
    shm.Size = 1024*1; // 8kb

    sprintf(shm.MapName, "ipc0");
    if (initMemoryMap(&shm)) {
        char* ptr = (char*)shm.pData;
        memset(ptr, 0, shm.Size);

        printf("memomap: Waiting for connection...\n");

        while (ptr && (*ptr == 0)) {
            Sleep(100);
        }

        printf("memomap: Connected!\n");

        data = (IPC_Data*)ptr;
        while(1) {
            Sleep(1000);
        }

        freeMemoryMap(&shm);
    } else {
        printf("memomap: Failed to init memory map\n");
    }
}

int main() {
    memomap_init();
    return 0;
}