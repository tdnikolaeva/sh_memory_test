#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>

const int SHM_SIZE = 1000;

int main() {
    int shm_id;
    char *shm_ptr;
    key_t key;
    char data[SHM_SIZE];
    int res;

    res = scanf("%d", &key);
    if (res != 1) {
        fprintf(stderr, "Invalid key.\n");
        return 1;
    }

    // get shm_id
    shm_id = shmget(key, SHM_SIZE, 0666);
    if (shm_id == -1) {
        fprintf(stderr, "shmget failed [%d]: %s\n", errno, strerror(errno));
        return 1;
    }

    // Attach
    shm_ptr = (char*)shmat(shm_id, NULL, 0);
    if (shm_ptr == (void*)-1) {
        fprintf(stderr, "shmat failed [%d]: %s\n", errno, strerror(errno));
        if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
            fprintf(stderr, "shmctl (remove) failed [%d]: %s\n", errno, strerror(errno));
        }
        return 1;
    }

    // data read from shmemory and write to stdout
    printf("%.*s\n", SHM_SIZE, shm_ptr);

    int ret_value = 0;

    // Detach
    if (shmdt(shm_ptr) == -1) {
        fprintf(stderr, "shmdt failed [%d]: %s\n", errno, strerror(errno));
        ret_value = 1;
    }

    // Remove
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "shmctl failed [%d]: %s\n", errno, strerror(errno));
        ret_value = 1;
    }

    return ret_value;
}
