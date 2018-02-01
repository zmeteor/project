#include "../include/wechat.h"
#include "../include/threadpool.h"
int pthread_setconcurrency(int);


/* 这里相当于main函数 */
void main_thread_func() 
{

    printf("main thread start run\n");
    printf("backlog:%d\n",BACKLOG);

    pthread_setconcurrency(6);

    epollfd = epoll_create(MAX_CONN);   //创建epoll文件描述符

    int listenfd = Socket(TCPSERV);
    open_noblock(listenfd);
    //Connect_mysql();
    //int epollfd = epoll_create(MAX_CONN);   //创建epoll文件描述符
    assert(epollfd != -1);
    struct epoll_event ready_fd[MAX_CONN]; //存放epoll_wait返回的可读写事件


    /* 线程所需参数结构 */
    thread_arg_t args;
    bzero(&args,sizeof(args));
    args.listenfd = listenfd;
    args.epollfd = epollfd;
    
    
    for(int i = 0;i < MAX_CONN;i++)
    {
        ready_fd[i].data.fd = -1;
        clients_msg[i].fd = -1;
    }

    register_epoll_fd(epollfd,listenfd,0);

    threadpool_t *first_pool = threadpool_create(6,1500,0);
    while(1)
    {
        int rtnum = epoll_wait(epollfd,ready_fd,MAX_CONN,-1);
        for(int i = 0;i < rtnum;i++)
        {
            int fd = ready_fd[i].data.fd;
            
            /* 有客户连接 */

            if(fd == listenfd )//&& (ready_fd[i].events & EPOLLIN))
            {
                threadpool_add(first_pool,handle_connection,listenfd,0);

            }
            
            else if( ready_fd[i].events & EPOLLIN )
            {
                printf("EPOLLIN\n");
                threadpool_add(first_pool,handle_read_request,fd,0);
            }

            else if(ready_fd[i].events & EPOLLOUT)
            {
                printf("EPOLLOUT\n");
                threadpool_add(first_pool,prequest_func[fd],fd,0);
            }
            
        }
        
    }
}