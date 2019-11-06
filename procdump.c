#include <./procdump.h>

pd_mapping map(char *fp){
    char filename[MAX_FILE_PATH];
    char m_line[MAX_LINE_LEN];
    if (strlen(fp) > MAX_FILE_PATH){
        puts("File path too long, exiting...");
        exit(-1);
    }
    strcpy(filename, fp);

    int fd = open(filename, O_RDONLY);
    FILE *p_file = fdopen(fd, "r");
    if (p_file == NULL) {
        puts("Error opening maps file...");
        exit(-1);
    }

    pd_mapping *pd_list = malloc(sizeof(pd_mapping));
    pd_map *pd_map_prev = NULL;

    int i = 0;
    while (fgets(m_line, sizeof(m_line), p_file)){

        // Parse the line with sscanf
        unsigned long int from, to, offset, major, minor;
        char flags[5];
        sscanf(m_line, "%lx-%lx %4c %lx %lx:%lx", &from, &to, flags, &offset, &major, &minor);

        //Create pd_map object
        pd_map *pd = malloc(sizeof(pd_map));
        pd->map_end = to;
        pd->map_start = from;
        pd->map_next = NULL;
        if(pd_map_prev != NULL){
            pd_map_prev->map_next = pd;
        } else
        {
            pd_list->pd_map_first = pd;
        }
        
        pd_map_prev = pd;

        i++;
    }
    return *pd_list;
}

void testTraverse(pd_mapping *p){
    pd_map *map = p->pd_map_first;
    int i = 0;
    while (map->map_next != NULL){
        printf("%d element:\tstart %lx\tend %lx\n", i, map->map_start, map->map_end);
        map = map->map_next;
        i++;
    }
}

int rmem(char *fp, pd_mapping *p){
    
    char filename[MAX_FILE_PATH];
    char m_line[MAX_MEM_READ];
    FILE * p_dumpfile = NULL;
    pd_map *map = p->pd_map_first;

    // Check passed path len
    if (strlen(fp) > MAX_FILE_PATH){
        puts("File path too long, exiting...");
        exit(-1);
    }

    strcpy(filename, fp);
    // Check if it's possible to read mem
    int fd = open(filename, O_RDONLY);
    if (fd == -1){
        puts("Error opening memory file...");
        exit(-1);
    }

    

    // We start with just one dump of the sections
    // Create dump file on disk.
    char dumpfile[MAX_FILE_PATH];
    while (map->map_next != NULL) {
	memset(dumpfile, 0, sizeof(dumpfile));	
    	sprintf(dumpfile, "./%lx.dump", map->map_start);
    	p_dumpfile = fopen(dumpfile, "a");

    	// Read from mem file using lseek64 and read...
    	// Read should suffice since sections mapped are quite small.
    	size_t size = map->map_end - map->map_start;
    	printf("Trying to read from %lx, to %lx\tsize: %ld\n", map->map_start, map->map_end, size);
    	off64_t progess = lseek64(fd, map->map_start, SEEK_SET);
    	int b_read;
	b_read = read(fd, m_line, sizeof(m_line));
	while(b_read > 0){
		fwrite(m_line, 1, b_read, p_dumpfile);
		b_read = read(fd, m_line, sizeof(m_line));
	}
    	fclose(p_dumpfile);
	map = map->map_next;
    }

    return 1;
}

int main(int argc, char *argv[]){
    char f_maps[MAX_FILE_PATH];
    char f_mem[MAX_FILE_PATH];
    pid_t pid;

    int wpid_stat;
    if(argc < 2){
        printf("%s [PID]\n", argv[0]);
        exit(-1);
    }
    pid = atoi(argv[1]);
    sprintf(f_maps, "/proc/%d/maps", pid);
    sprintf(f_mem, "/proc/%d/mem", pid);
    // debug
    printf("PATH TO MAPS FILE: %s\n", f_maps);
    ptrace(PT_ATTACH, pid, NULL, NULL);
    waitpid(pid, &wpid_stat, NULL);
    if(!WIFSTOPPED(wpid_stat)){
        puts("Process not stopped, could not attach...");
        exit(-1);
    }

    pd_mapping elems = map(f_maps);
    testTraverse(&elems);
    int p = rmem(f_mem, &elems);
    ptrace(PT_DETACH, pid, NULL, NULL);
}
