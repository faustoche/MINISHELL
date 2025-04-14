#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int	main(void)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL); // le fils doit recevoir SIGINT normalement
		execlp("sleep", "sleep", "50", NULL);
		perror("execve failed");
		exit(1);
	}
	else
	{
		signal(SIGINT, SIG_IGN); // le parent ignore SIGINT
		waitpid(pid, &status, 0);

		if (WIFSIGNALED(status))
		{
			int	code = 128 + WTERMSIG(status);
			printf("Child killed by signal %d (exit = %d)\n", WTERMSIG(status), code);
			exit(code); // <= très important
		}
		else if (WIFEXITED(status))
		{
			int code = WEXITSTATUS(status);
			printf("Child exited normally with code %d\n", code);
			exit(code);
		}
	}
}

