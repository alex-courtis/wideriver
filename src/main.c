#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signalfd.h>
#include <unistd.h>
#include <wayland-client-core.h>

#include "args.h"
#include "displ.h"
#include "log.h"

#define NPFDS 2
static struct pollfd pfds[NPFDS];
static struct pollfd *pfd_wayland = NULL;
static struct pollfd *pfd_signal = NULL;

static void init_pfds(void) {

	// wayland FD
	if (!pfd_wayland) {
		pfds[0].fd = wl_display_get_fd(displ->wl_display);
		pfds[0].events = POLLIN;
		pfds[0].revents = 0;
		pfd_wayland = &pfds[0];
	}

	// interesting signals
	if (!pfd_signal) {
		sigset_t mask;
		sigemptyset(&mask);
		sigaddset(&mask, SIGINT);
		sigaddset(&mask, SIGQUIT);
		sigaddset(&mask, SIGTERM);
		sigprocmask(SIG_BLOCK, &mask, NULL);

		pfds[1].fd = signalfd(-1, &mask, 0);
		pfds[1].events = POLLIN;
		pfds[1].revents = 0;
		pfd_signal = &pfds[1];
	}
}

// EXIT_SUCCESS on EPIPE otherwise error with op and errno
static int rc_errno_pipe_ok(const char *op) {
	if (errno == EPIPE) {
		log_i("Wayland display terminated, exiting.");
		return EXIT_SUCCESS;
	} else {
		log_f_errno("%s failed, exiting", op);
		return errno;
	}
}

// see Wayland Protocol docs Appendix B wl_display_prepare_read_queue
static int loop(void) {

	while (true) {
		init_pfds();

		// dispatch and prepare for next wayland event
		while (wl_display_prepare_read(displ->wl_display) != 0) {
			if (wl_display_dispatch_pending(displ->wl_display) == -1) {
				return rc_errno_pipe_ok("wl_display_dispatch_pending");
			}
		}

		// send dispatched
		if (wl_display_flush(displ->wl_display) == -1) {
			return rc_errno_pipe_ok("wl_display_read_events");
		}

		// poll for all events
		if (poll(pfds, NPFDS, -1) < 0) {
			log_f_errno("poll failed, exiting");
			return EXIT_FAILURE;
		}

		if (pfd_signal->revents & pfd_signal->events) {

			// signal received: int, quit, term
			struct signalfd_siginfo fdsi;
			if (read(pfd_signal->fd, &fdsi, sizeof(fdsi)) == sizeof(fdsi)) {
				log_i("Received signal %d %s", fdsi.ssi_signo, strsignal(fdsi.ssi_signo));
				return EXIT_SUCCESS;
			}
		} else if (pfd_wayland->revents & pfd_wayland->events) {

			// wayland events
			if (wl_display_read_events(displ->wl_display) == -1) {
				return rc_errno_pipe_ok("wl_display_read_events");
			}
		} else {
			log_f("Unknown event received, exiting");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
	int rc = EXIT_SUCCESS;

	args_cli(argc, argv);

	if (!getenv("WAYLAND_DISPLAY")) {
		log_f("Environment variable WAYLAND_DISPLAY not set, exiting");
		exit(EXIT_FAILURE);
	}

	if (!displ_init()) {
		rc = EXIT_FAILURE;
		goto done;
	}

	log_i("wideriver started");

	rc = loop();

	log_i("wideriver done %d", rc);

done:
	displ_destroy();

	return rc;
}
