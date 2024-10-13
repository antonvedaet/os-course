#include <iostream>
#include <cstring>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>
#include <chrono>
#include <linux/sched.h>
#include <sched.h>
#include <cstdlib>
#include <sys/mman.h>

int child_func(char *cmd[])
{
    execvp(cmd[0], cmd);
    std::cerr << "Ошибка выполнения команды: " << cmd[0] << std::endl;
    return 1;
}

void execute_command(char *cmd[])
{
    auto start = std::chrono::high_resolution_clock::now();

    struct clone_args
    {
        __aligned_u64 flags;
        __aligned_u64 pidfd;
        __aligned_u64 child_tid;
        __aligned_u64 parent_tid;
        __aligned_u64 exit_signal;
        __aligned_u64 stack;
        __aligned_u64 stack_size;
        __aligned_u64 tls;
        __aligned_u64 tls_size;
        __aligned_u64 set_tid;
        __aligned_u64 close_fd;
    };

    struct clone_args args = {
        .flags = CLONE_VM | CLONE_FS | CLONE_UNTRACED,
        .stack = reinterpret_cast<unsigned long>(malloc(1024 * 1024)),
        .stack_size = 1024 * 1024};

    pid_t pid = syscall(SYS_clone3, &args, sizeof(args));
    // pid_t pid = fork();
    if (pid == -1)
    {
        std::cerr << "Ошибка при создании процесса: " << strerror(errno) << std::endl;
        return;
    }
    else if (pid == 0)
    {
        int result = child_func(cmd);
        free(reinterpret_cast<void *>(args.stack));
        _exit(result);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Время выполнения: " << elapsed.count() << " секунд.\n";

        free(reinterpret_cast<void *>(args.stack));
    }
}

int main()
{
    while (true)
    {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit")
        {
            break;
        }

        char *cmd[100];
        char *token = std::strtok(const_cast<char *>(input.c_str()), " ");
        int i = 0;

        while (token != nullptr)
        {
            cmd[i++] = token;
            token = std::strtok(nullptr, " ");
        }
        cmd[i] = nullptr;

        execute_command(cmd);
    }
    return 0;
}
