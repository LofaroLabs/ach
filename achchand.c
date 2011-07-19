/* -*- mode: C; c-basic-offset: 4  -*- */
/*
 * Copyright (c) 2011, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name of the Georgia Tech Research Corporation nor
 *       the names of its contributors may be used to endorse or
 *       promote products derived from this software without specific
 *       prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GEORGIA TECH RESEARCH CORPORATION ''AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GEORGIA
 * TECH RESEARCH CORPORATION BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \file achchand.c
 *  \author Neil T. Dantam
 */


#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include <assert.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <amino.h>
#include "ach.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <syslog.h>
#include <poll.h>

// from linux man page
#define UNIX_PATH_MAX    108

#define TIMEOUT_MS_POLL 1000


enum achd_fdtype {
    ACHD_FD_CTRL_BAD = 0,
    ACHD_FD_CTRL_SRV,
    ACHD_FD_CTRL_CONN
};

struct achd_fd{
    enum achd_fdtype type;
    uint8_t *in;
    size_t n_in;
    size_t max_in;

    uint8_t *out;
    size_t n_out;
    size_t max_out;
};

struct achd_cx {
    size_t n;
    struct achd_fd *fds;
    struct pollfd *pfds;
    struct sockaddr_un addr;
};

static struct achd_cx d_cx;


static void init(void);
//static void run_channel(void);
static void run_io(void);
static void destroy(void);



int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    // initialize
    init();
    // start other thread
    // run
    run_io();
    // destroy
    destroy();
}



void init() {
    openlog("achchand", LOG_CONS | LOG_NDELAY | LOG_PERROR, LOG_USER );

    // open channel

    // allocate
    d_cx.n = 1;
    d_cx.pfds = AA_NEW0(struct pollfd);
    d_cx.fds = AA_NEW0( struct achd_fd );
    d_cx.fds[0].type = ACHD_FD_CTRL_SRV;

    // setup control socket
    d_cx.pfds[0].fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if( d_cx.pfds[0].fd < 0 ) {
        syslog(LOG_EMERG,"socket failed: %s", strerror(errno));
        exit(-1);
    }
    // bind
    memset(&d_cx.addr, 0, sizeof d_cx.addr);
    d_cx.addr.sun_family = AF_LOCAL;
    snprintf(d_cx.addr.sun_path, UNIX_PATH_MAX, "%s", "/tmp/asock");
    if( 0 > unlink(d_cx.addr.sun_path) &&
        errno != ENOENT ) {
        syslog(LOG_EMERG,"unlink failed: %s", strerror(errno));
        exit(-1);
    }
    if( 0 > bind(d_cx.pfds[0].fd, (struct sockaddr *)&d_cx.addr,
                 strlen(d_cx.addr.sun_path) + sizeof(d_cx.addr.sun_family)) ) {
    }
    // listen
    if( 0 > listen(d_cx.pfds[0].fd,5) ) {
        syslog(LOG_EMERG,"listen failed: %s", strerror(errno));
        exit(-1);
    }

}

void destroy() {
    closelog();
}

void run_channel() {
    while( 1 /*something*/ ) {
        // wait on channel (wakeup every so often to check stuff)

        // signal subscribers
    }
}

void dump_pollevents( int x ) {
    if( x & POLLIN )
        printf("POLLIN:");
    if( x & POLLOUT )
        printf("POLLOUT:");
    if( x & POLLERR )
        printf("POLLERR:");
    if( x & POLLHUP )
        printf("POLLHUP:");
    if( x & POLLNVAL )
        printf("POLLNVAL:");
}

void run_io() {

    void new_fd(int fd, int type) {
        d_cx.n++;
        // reallocate
        d_cx.pfds = (struct pollfd*)realloc(d_cx.pfds, d_cx.n * sizeof(*d_cx.pfds) );
        memset(&d_cx.pfds[d_cx.n-1], 0, sizeof(*d_cx.pfds));
        d_cx.fds = (struct achd_fd*)realloc(d_cx.fds, d_cx.n * sizeof(*d_cx.fds) );
        memset(&d_cx.fds[d_cx.n-1], 0, sizeof(*d_cx.fds));
        // set
        size_t i = d_cx.n - 1;
        d_cx.pfds[i].fd = fd;
        d_cx.pfds[i].events = POLLIN;
        d_cx.fds[i].type = type;
        // create buffers
        d_cx.fds[i].in = (uint8_t*)malloc(4000);
        d_cx.fds[i].out = (uint8_t*)malloc(4000);
        d_cx.fds[i].max_in = 4000;
        d_cx.fds[i].max_out = 4000;
        d_cx.fds[i].n_in = 0;
        d_cx.fds[i].n_out = 0;
    }


    d_cx.pfds[0].events = POLLIN;

    int r;

    while(1) {
        r = poll(d_cx.pfds, d_cx.n, TIMEOUT_MS_POLL);
        if( r > 0 ) {
            printf("poll %d\n", r);
            size_t oldn = d_cx.n;
            for( size_t i = 0; i < oldn; i ++ ) {
                //struct pollfd *pfd = &d_cx.pfds[i];
                //struct achd_fd *afd = &d_cx.fds[i];
                switch( d_cx.fds[i].type ) {
                case ACHD_FD_CTRL_SRV:
                    // check new connections
                    printf("SRV:  ");
                    dump_pollevents( d_cx.pfds[i].revents );
                    fputc('\n',stdout);
                    if( d_cx.pfds[i].revents & POLLIN ) {
                        int newconn;
                        do { newconn = accept( d_cx.pfds[i].fd, NULL, NULL ); }
                        while( newconn < 0 && EINTR == newconn );
                        if( newconn < 0 ) {
                            syslog(LOG_ERR, "failed accept: %s", strerror(errno));
                        } else {
                            new_fd( newconn, ACHD_FD_CTRL_CONN );
                        }
                    }
                    // check for weird events
                    if( d_cx.pfds[i].revents & (POLLERR|POLLHUP|POLLNVAL|POLLOUT) ) {
                        syslog(LOG_ERR, "unknown event on srv sock: "
                               "err: %d, hup: %d, nval: %d, out: %d",
                               d_cx.pfds[i].revents&POLLERR, d_cx.pfds[i].revents&POLLHUP,
                               d_cx.pfds[i].revents&POLLNVAL, d_cx.pfds[i].revents&POLLOUT);
                    }
                    break;
                case ACHD_FD_CTRL_CONN:
                    // check ctrl connections for input
                    if( d_cx.pfds[i].revents | POLLIN ) {
                        printf("conn pollin\n");
                        int s = aa_read_realloc( d_cx.pfds[i].fd,(void**)&d_cx.fds[i].in,
                                                 d_cx.fds[i].n_in, &d_cx.fds[i].max_in);
                        if( s < 0 && errno != EINTR ) {
                            syslog( LOG_ERR, "read failed: %s", strerror(errno) );
                        } else if (s > 0) {
                            fprintf(stdout, "read: %u\n", s);
                            d_cx.fds[i].n_in += (size_t)s;
                            size_t ss = fwrite(d_cx.fds[i].in,
                                               (size_t)1,
                                               d_cx.fds[i].n_in,
                                               stdout );
                            assert( ss > 0 );
                            d_cx.fds[i].n_in = 0;
                        }

                    }
                    // check ctrl connections for output
                    break;
                case ACHD_FD_CTRL_BAD:
                    // check ctrl connections for output
                    syslog(LOG_ERR, "bad fd type at index %d", i );
                }
            }
        } else if (r < 0 && EINTR != errno && EAGAIN != errno ) {
            syslog(LOG_EMERG, "poll failed: %s", strerror(errno) );
            exit(-1);
        } else {
            printf("poll nop\n");
        }
    }
}