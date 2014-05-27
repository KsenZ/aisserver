#ifndef AISSERVER_H
#define AISSERVER_H

class aisserver
{

protected:
    void daemonize();
    void mainloop();
    void operate(int fd);
    static void sighandler(int signum);

public:
    void exec();
};

#endif // AISSERVER_H
