#include <unistd.h>
#include <iostream>
#include <linux/sched.h> /* Definition of struct clone_args */
#include <sched.h>       /* Definition of CLONE_* constants */
#include <sys/syscall.h> /* Definition of SYS_* constants */
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    std::cout << "[agent] My pid is: " << getpid() << std::endl;
    int cpid = syscall(
        SYS_clone,
        SIGCHLD | CLONE_NEWPID,
        nullptr /*stack*/,
        nullptr /*parent_tid*/,
        nullptr /*child_tid*/,
        0 /*tls: only meaningful if CLONE_SETTLS flag is set*/);

    if (cpid == -1)
    {
        std::perror("Failed to spawn child process");
        return 1;
    }

    if (cpid == 0)
    {
        std::cout << "[container] I think my pid is: " << getpid() << std::endl;
        if (execv("/bin/sh", &argv[1]) == -1)
        {
            std::perror("Failed to execv");
            return 1;
        }
    }

    std::cout << "[agent] My child's pid is: " << cpid << std::endl;
    if (waitpid(cpid, nullptr, 0) == -1)
    {
        std::perror("Failed to wait for child process");
        return 1;
    }

    return 0;
}