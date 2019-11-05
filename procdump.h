#define _LARGEFILE64_SOURCE     /* See feature_test_macros(7) */
#include <sys/ptrace.h> //ptrace
#include <stdio.h> // IO
#include <stdlib.h> // malloc
#include <sys/stat.h> // open
#include <fcntl.h> // open
#include <string.h> // strlen etc.
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> // for lseek



#define MAX_FILE_PATH 254
#define MAX_LINE_LEN 1024
#define MAX_MEM_READ 4096
typedef struct {
    unsigned long int map_start;
    unsigned long int map_end;
    struct pd_map *map_next;
} pd_map;

typedef struct {
    struct pd_map *pd_map_first;
} pd_mapping;
