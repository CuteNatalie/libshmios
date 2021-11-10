#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>

#define MAP_SIZE_POW2       16
#define MAP_SIZE            (1 << MAP_SIZE_POW2)

static int g_shm_fd = -1;
static unsigned char *g_shm_base = NULL;
static char *g_shm_file_path = NULL;

int shmctl (int __shmid, int __cmd, struct shmid_ds *__buf)
{
	int ret=0;
  if (g_shm_base != NULL) {
     munmap(g_shm_base, MAP_SIZE);
     g_shm_base = NULL;
   }

   if (g_shm_fd != -1) {
     close(g_shm_fd);
     g_shm_fd = -1;
   }

   if (g_shm_file_path != NULL) {
     free(g_shm_file_path);
     g_shm_file_path = NULL;
   }
	return ret;
}

int shmget (key_t __key, size_t __size, int __shmflg)
{
	int fd,ret;
	char key[11];
    g_shm_file_path = (char *)malloc(L_tmpnam);
    memset(g_shm_file_path, 0x0, L_tmpnam);
    mktemp(g_shm_file_path);
	
	fd = shm_open(g_shm_file_path, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (fd < 0)
		return fd;
    if (fd = -1)
        goto error;

	return fd;
	
error:
	close(fd);
	return ret;
}

void *shmat (int __shmid, const void *__shmaddr, int __shmflg)
{
    size_t *ptr, size;
	ptr = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, g_shm_fd, 0);
	*ptr = size;
	return &ptr[1];
}

int shmdt (const void *__shmaddr)
{
	size_t *ptr, size;
	ptr = (size_t *)__shmaddr;
	ptr--;
	size = *ptr;
    return shm_unlink((void *)ptr);
}
