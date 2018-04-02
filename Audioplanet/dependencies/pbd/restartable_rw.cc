#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

/* Very useful functions for pthreads-dependent programs. pthread
   condition variables involve signals, and thus threads can end 
   up with a lot of interrupted system calls. it would be possible to 
   reset the default behaviour of a system call, but this has possibly
   severe implications for other libraries that don't expect this. so
   instead, we use these functions which work properly even if the
   system call in interrupted.

   In theory, we need versions of these for other system calls, but
   because of timing issues, these are the two that are normally
   affected.
*/

int
restartable_write (int fd, unsigned char *buf, size_t cnt)

{
	size_t written;
	off_t offset = 0;
	size_t ocnt = cnt;

	if (cnt == 0) {
		return 0;
	}

	while (cnt) {
		written = write (fd, buf+offset, cnt);

		if (written == 0) {
			continue;
		}

		if (errno == EINTR) {
			offset += written;
			cnt -= written;
			continue;
		}
		
		if (written != cnt) {
			return -1;
		}
	}

	return ocnt;
}

int
restartable_read (int fd, unsigned char *buf, size_t cnt)

{
	size_t nread;
	off_t offset = 0;
	size_t ocnt = cnt;

	if (cnt == 0) {
		return 0;
	}

	while (cnt) {
		nread = read (fd, buf+offset, cnt);

		if (nread == 0) {
			continue;
		}

		if (errno == EINTR) {
			offset += nread;
			cnt -= nread;
			continue;
		}
		
		if (nread != cnt) {
			return -1;
		}
	}

	return ocnt;
}
