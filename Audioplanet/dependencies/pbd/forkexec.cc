#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

/*
  Run a program with program name and args specified by argv, in the
  environment specified by envp.

  The exec'ed program uses inpipe[READ_DESC] and outpipe[WRITE_DESC]
  as stdin and stdout, resp.

  You can safely leave outpipe and inpipe unspecified (0) if you
  want the command to read/write from/to stdin/stdout. envp could just
  be the environ extern that all C/C++ programs have access to, or it
  could be some filtered version of it. This function doesn't care
  what it is, but the cmd might (it might try to use $HOME for
  example). I don't know if it can a 0 pointer.  
*/

#define READ_DESC 0
#define WRITE_DESC 1
#define MAXARGS 512

/* Return codes:

   -1: dup of read-end of pipe failed
   -2: dup of write-end of pipe failed
   -3: execve failed
   -4: fork failed
*/

pid_t
forkexec (char **argv, char **envp, int outpipe[2], int inpipe[2])

{
	extern char **environ;
	pid_t pid;

	if ((pid = fork ()) == 0) {

		/* In the child process */

		if (inpipe) {
			if (inpipe[READ_DESC] != 0) {
				close (0);
				if (dup (inpipe[READ_DESC]) < 0) {
					return (-1);
				}
			}
			if (inpipe[WRITE_DESC]) {
				close (inpipe[WRITE_DESC]);
			}
		}

		if (outpipe) {
			if (outpipe[WRITE_DESC] != 1) {
				close (1);
				if (dup (outpipe[WRITE_DESC]) < 0) {
					return (-2);
				}
			}
			if (outpipe[READ_DESC]) {
				close (outpipe[READ_DESC]);
			}
		}

		/* this is stupid - it shouldn't be necessary to do this ! */
	
		environ = envp;
	
		/* do it */

		execve (argv[0], argv, envp);
		return (-3);
	
	} else if (pid == -1) {
		return (-4);

	} else {
		/* In the parent process */
	
		if (outpipe && outpipe[WRITE_DESC]) {
			close (outpipe[WRITE_DESC]);
		}
	}

	return (pid);
}

/*
  This is like system(3), only safe from all shell side effects and
  security concerns. Its also faster, and it doesn't wait for the
  forked child to return.
*/

pid_t
forkexec_cmd (char *cmd, char **envp, int outpipe[2], int inpipe[2])

{
	char *argv[MAXARGS];
	int i;
	char *cpcmd;
	char *p, *q;
	pid_t retval;
	int should_break = 0;

	cpcmd = strdup (cmd);

	for (p = cpcmd, q = cpcmd, i = 0; *p && !should_break &&
		     (i < MAXARGS); p = q , i++) {

		while ((*q) && !isspace (*q)) q++;
		if (!*q) {
			should_break = 1;
		} else {
			should_break = 0;
			*q = '\0';
		}
		argv[i] = p;
		q++;
	}

	argv[i] = 0;
	retval = forkexec (argv, envp, outpipe, inpipe);
	free (cpcmd);
	return retval;
}
