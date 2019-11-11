# procdumper
Dumps process ELF by reading /proc/mem file

### Usecases
This is an ordenary lightweight memory dumper, and thus can be used to dump running process memory of any PID on the system. In the case of reversing, this is effective in unpacking a staticly packed executable.
If a program "hangs" at some point during execution, it is possible to dump the unpacked code, by reading the memory mapped ELF using radare2 etc.

This method requires the process to be ptraced, and so any anti-debugging would defeat it pretty easily

### Usage
`./procdump [PID]`
