#include "aisserver.h"

void aisserver::sighandler(int signum)
{
    waitpid(0, 0, WNOHANG);
}

void aisserver::daemonize()
{
    int pid;
    struct sigaction sa;

    cout << "Server start ok ! " << flush;
    pid = fork();

    switch(pid)
    {
    case 0:
        setsid();
        chdir("/");

        close(0);
        close(1);
        close(2);

        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = &sighandler;
        sigaction(SIGCHLD, &sa, 0);

        openlog("aiserver", 0, LOG_USER);
        mainloop();
        closelog();

        exit(0);

    case -1:
        cout << "fork() error" << endl;
        break;

    default:
        cout << "ok. PID=" << pid << endl;
        break;
    }
}

void aisserver::mainloop()
{
    int sockfd,newsockfd,anslen;
    socklen_t clilen;
    int n;
    char login[BUFLEN], pass[BUFLEN];
    struct sockaddr_in servaddr,cliaddr;
    string str_ans;
    char* ballance;

    //Создаем сокет
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket");
        exit(1);
    }

    //Заполняем структуру
    servaddr.sin_family= AF_INET;
    servaddr.sin_port=htons(PORT);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY); //Принимаем соединения с любого адреса

    //Биндим сокет к адресу
    if(bind(sockfd, (struct sockaddr *) &servaddr,
    sizeof(servaddr)) < 0){
        perror("bind");
        close(sockfd);
        exit(1);
    }
    //Переводим сокет в слушающее состояние
    if(listen(sockfd, 5) < 0){
        perror("listen");
        close(sockfd);
        exit(1);
    }

    while(1)
    {
        clilen=sizeof(cliaddr);
        if((newsockfd=accept(sockfd,(struct sockaddr*)&cliaddr,&clilen))<0) //Принимаем соединение
        {
            perror("accept");
            close(sockfd);
            exit(1);
        }


        //printf("Client %s connected\n",inet_ntoa(cliaddr.sin_addr));
        syslog(LOG_NOTICE, "connection from %s", inet_ntoa(cliaddr.sin_addr));
        char* ip;
        ip=inet_ntoa(cliaddr.sin_addr);

        ballance = pg_connect(ip);


        str_ans ="|"+str()+"\r\n";
        char answer[500];
        strcpy( answer, str_ans.c_str() );

        if((n=write(newsockfd, ballance, strlen(ballance)))<0)		//Отправляем ответ
        {
            perror("write");
            close(sockfd);
            close(newsockfd);
            exit(1);
        }

        if((n=write(newsockfd, answer, strlen(answer)))<0)		//Отправляем ответ
        {
            perror("write");
            close(sockfd);
            close(newsockfd);
            exit(1);
        }

        if(n < 0) //При чтении произошла ошибка
        {
            perror("read");
            close(sockfd);
            close(newsockfd);
            exit(1);
        }
    }

    close(newsockfd);
    close(sockfd);
}

void aisserver::operate(int fd)
{
    char c;
    bool finished;
    string cmd, answer;

    finished = false;

    write(fd, answer.c_str(), answer.size());

    while(!finished)
    {
    cmd = answer = "";

    while(!finished) {
        finished = read(fd, &c, 1) != 1;

        if(c == '\n') break; else
        if(c != '\r') cmd += c;
    }

    if(!finished && !cmd.empty())
    {
        if(cmd == "info")
        {
                answer = "First Socket SERVER :)";
        }
        else if(cmd == "version")
        {
        answer = "version 1.0";
        }

        else if(cmd == "quit")
        {
        shutdown(fd, 2);
        close(fd);
        finished = true;
        }

        else
            {

        /*
            int i  = system(cmd.c_str());
        if(i == -1)
        {
        answer = "err";
        }
        else answer = "ok";
            */
        }

    }

    if(!finished && !answer.empty())
    {
        answer += "\n";
        write(fd, answer.c_str(), answer.size());
    }
    }

    exit(0);
}

void aisserver::exec()
{
    daemonize();
}
