#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

const int SHM_SIZE = 1000;
const int PROJ_ID = 65;
const char *message = "Hello world!";

key_t generate_key(){
    key_t key;
    char *temp_filename;

    // Generate tmp filename
    temp_filename = tmpnam(NULL);
    if (temp_filename == NULL) {
        fprintf(stderr, "tmpnam failed [%d]: %s\n", errno, strerror(errno));
        exit(1);
    }

    int fd = open(temp_filename, O_CREAT | O_EXCL, 0666);
    if (fd == -1) {
        fprintf(stderr, "open failed [%d]: %s\n", errno, strerror(errno));
        exit(1);
    }

    // Generate key
    key = ftok(temp_filename, PROJ_ID);
    if (key == -1) {
        fprintf(stderr, "ftok failed [%d]: %s\n", errno, strerror(errno));
        close(fd);
        remove(temp_filename);
        exit(1);
    }

    close(fd);
    remove(temp_filename);
    return key;
}

void free_sh_memory(char *shm_ptr, int shm_id){
    if (shmdt(shm_ptr) == -1) {
        fprintf(stderr, "shmdt failed [%d]: %s\n", errno, strerror(errno));
    }
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "shmctl failed [%d]: %s\n", errno, strerror(errno));
    }
    exit(1);
}

int main() {
    int shm_id;
    char *shm_ptr;

    key_t key = generate_key();

    // Create the shmemory segment
    shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1) {
        fprintf(stderr, "shmget failed [%d]: %s\n", errno, strerror(errno));
        return 1;
    }

    // Attach
    shm_ptr = (char*)shmat(shm_id, NULL, 0);
    if (shm_ptr == (void*)-1) {
        fprintf(stderr, "shmat failed [%d]: %s\n", errno, strerror(errno));
        if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
            fprintf(stderr, "shmctl failed [%d]: %s\n", errno, strerror(errno));
        }
        return 1;
    }

    // data write
    size_t message_len = strlen(message);

    if (message_len > SHM_SIZE - 1) {
        fprintf(stderr, "Message too long for shared memory\n");
        free_sh_memory(shm_ptr, shm_id);
    }

    strncpy(shm_ptr, message, message_len);
    shm_ptr[message_len] = '\0';

    // Detach
    if (shmdt(shm_ptr) == -1) {
        fprintf(stderr, "shmdt failed [%d]: %s\n", errno, strerror(errno));
        free_sh_memory(shm_ptr, shm_id);
    }

    printf("%d\n", key);

    return 0;
}
