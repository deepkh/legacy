/*
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/select.h>

#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
*/
#include "Common.h"

#define MAX_LINE 16384

char rot13_char(char c)
{
    /* We don't want to use isalpha here; setting the locale would change
     * which characters are considered alphabetical. */
    if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c + 13;
    else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c - 13;
    else
        return c;
}

struct fd_state 
{
	int skfd;
    char buffer[MAX_LINE];
    size_t buffer_used;

    int writing;
    size_t n_written;
    size_t write_upto;
};

struct fd_state *alloc_fd_state(int skfd)
{
    struct fd_state *state = (struct fd_state *)malloc(sizeof(struct fd_state));
    if (!state)
        return NULL;
	state->skfd = skfd;
    state->buffer_used = state->n_written = state->writing =
        state->write_upto = 0;
    return state;
}

void free_fd_state(struct fd_state *state)
{
	closeSocket(state->skfd);
    free(state);
}

int make_nonblocking(int fd)
{
#ifdef WIN32
	unsigned long flags = 1;
	gtrace("%s\n", __FUNCTION__);
	int ret = ioctlsocket(fd, FIONBIO, &flags);
	if (ret == SOCKET_ERROR)
	{
		PrtErr(__FUNCTION__);
		exit(1);
	}
#else
	gtrace("%s\n", __FUNCTION__);
	return (fcntl(fd, F_SETFL, O_NONBLOCK) != -1);
#endif
}

int do_read(struct fd_state *state)
{
    char buf[1024];
    int i;
    ssize_t result;
	gtrace("%s\n", __FUNCTION__);
    while (1) {
        result = recv(state->skfd, buf, sizeof(buf), 0);
        if (result <= 0)
		{
			gtrace("%s break.\n", __FUNCTION__);
            break;
		}

        for (i=0; i < result; ++i)  {
			gtrace("%c ", buf[i]);
            if (state->buffer_used < sizeof(state->buffer))
                state->buffer[state->buffer_used++] = buf[i];//rot13_char(buf[i]);
            if (buf[i] == '\n') {
                state->writing = 1;
                state->write_upto = state->buffer_used;
            }
        }
    }
			gtrace("%s result == %d WSAGetLastError:%d\n", __FUNCTION__, result, WSAGetLastError());

    if (result == 0) {
		
        return 1;
    } else if (result < 0) {
        if (errno == EAGAIN)
            return 0;
		
        return -1;
    }

    return 0;
}

int do_write(struct fd_state *state)
{
    while (state->n_written < state->write_upto) {
        ssize_t result = send(state->skfd, state->buffer + state->n_written,
                              state->write_upto - state->n_written, 0);
        if (result < 0) 
		{
            if (errno == EAGAIN)
                return 0;
            return -1;
        }
        assert(result != 0);

        state->n_written += result;
    }

    if (state->n_written == state->buffer_used)
        state->n_written = state->write_upto = state->buffer_used = 1;

    state->writing = 0;

    return 0;
}

void run(int nListenPort)
{
    int listener;
    //struct fd_state *state[FD_SETSIZE];
	vector<fd_state *> state;
    struct sockaddr_in sin;
    int i, n, maxfd;
    fd_set readset, writeset, exset;

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(nListenPort);

	/*
    for (i = 0; i < FD_SETSIZE; ++i)
        state[i] = NULL;
	*/

    listener = socket(AF_INET, SOCK_STREAM, 0);
	gtrace("%s sk:%d\n", __FUNCTION__, listener);

    make_nonblocking(listener);

#ifndef WIN32
    {
        int one = 1;
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    }
#endif

    if (bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        perror("bind");
        return;
    }
	gtrace("%s bind\n", __FUNCTION__);

    if (listen(listener, 16)<0) 
	{
        perror("listen");
        return;
    }
	gtrace("%s listen\n", __FUNCTION__);

    FD_ZERO(&readset);
    FD_ZERO(&writeset);
    FD_ZERO(&exset);

    while (1) 
	{
        maxfd = listener;

        FD_ZERO(&readset);
        FD_ZERO(&writeset);
        FD_ZERO(&exset);

		//add server socket to server and client
        FD_SET(listener, &readset);
        for (i=0; i < state.size(); ++i)
		{
			FD_SET(state[i]->skfd, &readset);
			if (state[i]->writing)
			{
				FD_SET(state[i]->skfd, &writeset);
			}
		}

		//select server
        n = select(FD_SETSIZE, &readset, &writeset, &exset, NULL);
        if (FD_ISSET(listener, &readset)) 
		{
			struct sockaddr_storage ss;
            socklen_t slen = sizeof(ss);
            int fd = accept(listener, (struct sockaddr*)&ss, &slen);
            if (fd < 0) {
                perror("accept");
            } /*else if (fd > FD_SETSIZE) {
				closeSocket(fd);
            }*/ else {
				gtrace("accept\n");
				make_nonblocking(fd);
                //state[fd] = alloc_fd_state();
				state.push_back(alloc_fd_state(fd));
                assert(state[fd]);/*XXX*/
            }
        }

		//select client
        for (i=0; i < state.size(); ++i) {
            int r = 0;

			gtrace("%d\n", i);
            if (FD_ISSET(state[i]->skfd, &readset)) {
                r = do_read(state[i]);
            }
            if (r == 0 && FD_ISSET(state[i]->skfd, &writeset)) {
                r = do_write(state[i]);
            }
            if (r) {
				vector<fd_state *>::iterator iter;
				for (iter = state.begin(); iter != state.end(); )
				{
					if ((*iter) == state[i])
					{
						gtrace("remove %d\n", (*iter)->skfd);
                		free_fd_state(*iter);
						state.erase(iter);
					}
				}
            }
        }
    }
}

int main(int c, char *v[])
{
	gtrace("%s %d\n", __FUNCTION__,  atoi(v[1]));
	initializeWinsockIfNecessary();
    setvbuf(stdout, NULL, _IONBF, 0);
    run(atoi(v[1]));
    return 0;
}

