#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
static pid_t spawn(const char *name, int samples, int exitValue)
{
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(EXIT_FAILURE); }
    if (pid == 0) {
        for (int i = 1; i <= samples; i++) {
            printf("  [%s sensor] sample %d/%d\n", name, i, samples);
            fflush(stdout);
            sleep(1);
        }
        printf("  [%s sensor] done, returning reading %d\n", name, exitValue);
        exit(exitValue);
    }
    return pid;
}
static int report(const char *name, pid_t pid)
{
    int status;
    if (waitpid(pid, &status, 0) < 0) { perror("waitpid"); return -1; }
    if (WIFEXITED(status)) {
        printf("[Gateway] %s child (pid %d) exited normally, status = %d\n", name, pid, WEXITSTATUS(status));
        return WEXITSTATUS(status);
    }
    if (WIFSIGNALED(status))
        printf("[Gateway] %s child (pid %d) killed by signal %d\n", name, pid, WTERMSIG(status));
    return -1;
}
int main(void)
{
    printf("[Gateway] pid %d starting sensor readers\n", getpid());
    fflush(stdout);
    pid_t tp = spawn("Temperature", 2, 35);
    pid_t pp = spawn("Pressure", 3, 12);
    printf("[Gateway] waiting for both sensors...\n");
    int t = report("Temperature", tp);
    int p = report("Pressure", pp);
    if (t >= 0 && p >= 0)
        printf("[Gateway] Safety index = %d\n", t * p);
    else
        printf("[Gateway] A sensor failed; index not computed.\n");
    printf("[Gateway] exiting.\n");
    return 0;
}
