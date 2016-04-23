/*
 * Copyright (c) 2016
 * Geco Gaming Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for GECO purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. Geco Gaming makes no
 * representations about the suitability of this software for GECO
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

/**
 * Created on 22 April 2016 by Jake Zhang
 */

#ifndef __INCLUDE_POLLER_H
#define __INCLUDE_POLLER_H

#include "globals.h"
#include "gecotimer.h"

#include <random>
#include <algorithm>

#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifndef _WIN32
#include <sys/timeout.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netdb.h>
#include <arpa/inet.h>      /* for inet_ntoa() under both SOLARIS/LINUX */
#include <sys/errno.h>
#include <sys/uio.h>        /* for struct iovec */
#include <sys/param.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <net/if.h>
#ifdef SCTP_OVER_UDP
#include <netinet/udp.h>
#endif
#include <asm/types.h>
#include <linux/rtnetlink.h>
#else
#include <winsock2.h>
#include <WS2tcpip.h>
#include <sys/timeb.h>
#endif

#if defined (__linux__)
#include <asm/types.h>
#include <linux/rtnetlink.h>
#else /* this may not be okay for SOLARIS !!! */
#ifndef _WIN32
#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <net/route.h>
#ifndef __sun
#include <net/if_var.h>
#include <machine/param.h>
#else
#include <sys/sockio.h>
#endif
#endif
#endif

#ifndef IN_EXPERIMENTAL
#define  IN_EXPERIMENTAL(a)   ((((int) (a)) & 0xf0000000) == 0xf0000000)
#endif

#ifndef IN_BADCLASS
#define  IN_BADCLASS(a)    IN_EXPERIMENTAL((a))
#endif

#if defined( __linux__) || defined(__unix__)
#include <sys/poll.h>
#else
#define POLLIN     0x001
#define POLLPRI    0x002
#define POLLOUT    0x004
#define POLLERR    0x008
#endif

#define IFA_BUFFER_LENGTH   1024
#define POLL_FD_UNUSED     -1
#define MAX_FD_SIZE     20
#define    EVENTCB_TYPE_SCTP       1
#define    EVENTCB_TYPE_UDP        2
#define    EVENTCB_TYPE_USER       3
#define    EVENTCB_TYPE_ROUTING    4

namespace geco
{
    namespace net
    {
        /*=====================cb event defs=====================*/

        //! Structure for callback events. The function "action" is called by the event-handler,
        //! when an event occurs on the file-descriptor.
        struct event_cb_t
        {
            int sfd;
            int eventcb_type;
            /* pointer to possible arguments, associations etc. */
            void(*action)();
            void *arg1, *arg2, *userData;
        };

        struct data_t
        {
            char* dat;
            int   len;
            void(*cb)();
        };


        struct socket_despt_t
        {
            int       fd;
            int events;
            int revents;
            long      revision;
        };

        class poller_t
        {
            private:
            long revision;
            event_cb_t *event_callbacks[MAX_FD_SIZE];
            /* a static counter - for stats we should have more counters !  */
            unsigned int stat_send_event_size;
            /* a static value that keeps currently treated timer id */
            geco::ultils::timer_mgr::timer_id_t curr_timer;

            /* a static receive buffer  */
            uchar internal_receive_buffer[MAX_MTU_SIZE + 20];

            socket_despt_t socket_despts[MAX_FD_SIZE];
            int socket_despts_size;

            int ip4_socket_despt;       /* socket fd for standard SCTP port....      */
            int ip6_socket_despt;
            int icmp_socket_despt;       /* socket fd for ICMP messages */

            /*
            * poll_socket_despts()
            * An extended poll() implementation based on select()
            *
            * During the select() call, another thread may change the FD list,
            * a revision number keeps track that results are only reported
            * when the FD has already been registered before select() has
            * been called. Otherwise, the event will be reported during the
            * next select() call.
            * This solves the following problem:
            * - Thread #1 registers user callback for socket n
            * - Thread #2 starts select()
            * - A read event on socket n occurs
            * - poll_socket_despts() returns
            * - Thread #2 sends a notification (e.g. using pthread_condition) to thread #1
            * - Thread #2 again starts select()
            * - Since Thread #1 has not yet read the data, there is a read event again
            * - Now, the thread scheduler selects the next thread
            * - Thread #1 now gets CPU time, deregisters the callback for socket n
            *      and completely reads the incoming data. There is no more data to read!
            * - Thread #1 again registers user callback for socket n
            * - Now, thread #2 gets the CPU again and can send a notification
            *      about the assumed incoming data to thread #1
            * - Thread #1 gets the read notification and tries to read. There is no
            *      data, so the socket blocks (possibily forever!) or the read call
            *      fails.

            poll()���������������ĳЩUnixϵͳ�ṩ������ִ����select()����ͬ�ȹ��ܵĺ�����
            ���������������������
            #include <poll.h>
            int poll(struct pollfd fds[], nfds_t nfds, int timeout)��
            ����˵��:
            fds����һ��struct pollfd�ṹ���͵����飬���ڴ����Ҫ�����״̬��Socket��������
            ÿ�������������֮��ϵͳ�������������飬���������ȽϷ��㣻�ر��Ƕ���
            socket���ӱȽ϶������£���һ���̶��Ͽ�����ߴ����Ч�ʣ���һ����select()��
            ����ͬ������select()����֮��select()�����������������socket���������ϣ�
            ����ÿ�ε���select()֮ǰ�������socket���������¼��뵽�����ļ����У�
            ��ˣ�select()�����ʺ���ֻ���һ��socket�������������
            ��poll()�����ʺ��ڴ���socket�������������
            nfds��nfds_t���͵Ĳ��������ڱ������fds�еĽṹ��Ԫ�ص���������
            timeout����poll��������������ʱ�䣬��λ�����룻
            ����ֵ:
            >0������fds��׼���ö���д�����״̬����Щsocket����������������
            ==0������fds��û���κ�socket������׼���ö���д���������ʱpoll��ʱ��
            ��ʱʱ����timeout���룻���仰˵�����������socket��������û���κ��¼�����
            �Ļ�����ôpoll()����������timeout��ָ���ĺ���ʱ�䳤��֮�󷵻أ����
            timeout==0����ôpoll() �����������ض������������timeout==INFTIM����ôpoll()
            ������һֱ������ȥ��ֱ��������socket�������ϵĸ���Ȥ���¼������ǲŷ��أ�
            �������Ȥ���¼���Զ����������ôpoll()�ͻ���Զ������ȥ��
            -1�� poll��������ʧ�ܣ�ͬʱ���Զ�����ȫ�ֱ���errno��
            */
            int poller_t::poll_socket_despts(socket_despt_t* despts,
                int* count,
                int timeout,
                void(*lock)(void* data),
                void(*unlock)(void* data),
                void* data);

            //! function to set an event mask to a certain socket despt
            void set_event_mask(int fd_index, int sfd, int event_mask)
            {
                if (fd_index > MAX_FD_SIZE)
                    error_log(loglvl_fatal_error_exit, "FD_Index bigger than MAX_FD_SIZE ! bye !\n");

                socket_despts[fd_index].fd = sfd; /* file descriptor */
                socket_despts[fd_index].events = event_mask;
                /*
                * Set the entry's revision to the current poll_socket_despts() revision.
                * If another thread is currently inside poll_socket_despts(), poll_socket_despts()
                * will notify that this entry is new and skip the possibly wrong results
                * until the next invocation.
                */
                socket_despts[fd_index].revision = revision;
                socket_despts[fd_index].revents = 0;
            }

            public:
            poller_t()
            {
                revision = 0;
                stat_send_event_size = 0;
                socket_despts_size = 0;
                ip4_socket_despt = -1;
                ip6_socket_despt = -1;
                icmp_socket_despt = -1;
            }

            int ip4_socket_despt(){ return this->ip4_socket_despt; }
            int ip6_socket_despt(){ return this->ip6_socket_despt; }
        };
    }
}
#endif
