#include "../include/wechat.h"

/* 注册epoll事件 */
void register_epoll_fd(int epollfd,int fd,int oneshot)
{
    //int epollfd = *epoll_fd;
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLERR;
    if(oneshot)
    {
        event.events |= EPOLLONESHOT;
    }
    if(epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event) == -1) 
    {
        fprintf(stderr,"fd:%d register failed!",fd);
    }
}
