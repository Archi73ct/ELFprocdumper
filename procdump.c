#include <./procdump.h>

int map(char *fp){
    char filename[MAX_FILE_PATH];
    char m_line[MAX_LINE_LEN];
    if (strlen(fp) > MAX_FILE_PATH){
        puts("File path too long, exiting...");
        exit(-1);
    }
    strcpy(filename, fp);
    // debug
    puts("Copied filename");
    printf("%s\n", filename);
    //
    int fd = open(filename, O_RDONLY);
    FILE *p_file = fdopen(fd, "r");
    printf("Open fd status on map file: %d\n", fd);
    while (fgets(m_line, sizeof(m_line), p_file)){
        // debug
        puts("Got line");
        printf("%s", m_line);

        // Parse the line with sscanf
        unsigned long int from, to, offset, major, minor;
        char flags[5];
        sscanf(m_line, "%lx-%lx %4c %lx %lx:%lx", &from, &to, flags, &offset, &major, &minor);
        pd_map *pd = malloc(sizeof(pd_map));
        pd->map_end = to;
        pd->map_start = from;
        pd->map_next = NULL;
        printf("Parsed: f: %lx, t: %lx, f: %4c, ma: %lx, mi: %lx\n", pd->map_start, to, flags, major, minor);
    }
    return 0;
}

int main(int argc, char *argv[]){
    char f_maps[MAX_FILE_PATH];
    pid_t pid;

    int wpid_stat;
    if(argc < 2){
        printf("%s [PID]\n", argv[0]);
        exit(-1);
    }
    pid = atoi(argv[1]);
    sprintf(f_maps, "/proc/%d/maps", pid);
    // debug
    printf("PATH TO MAPS FILE: %s\n", f_maps);
    ptrace(PT_ATTACH, pid);
    waitpid(pid, &wpid_stat, NULL);
    if(!WIFSTOPPED(wpid_stat)){
        puts("Process not stopped, could not attach...");
        exit(-1);
    }

    map(f_maps);

    ptrace(PT_DETACH, pid);
}