#include "headers.h"
void remove_html_tags(char *response)
{
    char *output = response;
    int flag = 0;

    for (int i = 0; response[i] != '\0'; i++)
    {
        if (response[i] == '<')
        {
            flag = 1;
        }
        else if (response[i] == '>')
        {
            flag = 0;
        }
        else if (!flag)
        {
            *output = response[i];
            output++;
        }
    }

    *output = '\0';
}
void implimenting_iman(char *command)
{
    char *t = strtok(command, " \n\t");
    t = strtok(NULL, " \n\t");

    char *hostname = "man.he.net";
    char *path = (char *)malloc(sizeof(char) * PATH_SIZE);
    snprintf(path, PATH_SIZE, "/?topic=%s&section=all", t);
    int port = 80; 

    struct addrinfo iman;
    struct addrinfo *result, *rp;
    int sock;

    // Initialize the iman structure
    memset(&iman, 0, sizeof(struct addrinfo));
    iman.ai_socktype = SOCK_STREAM; // Use TCP
    iman.ai_family = AF_UNSPEC;     // Allow both IPv4 and IPv6

    // DNS Resolve the hostname to an IP address
    int status = getaddrinfo("man.he.net", "http", &iman, &result);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Loop through the results and attempt to open a TCP socket
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sock == -1)
        {
            perror("socket");
            continue;
        }

        if (connect(sock, rp->ai_addr, rp->ai_addrlen) == -1)
        {
            perror("connect");
            close(sock); 
            continue;
        }

        break;                   
    }

    if (rp == NULL)
    {
        fprintf(stderr, "Failed to connect to any IP address\n");
        exit(EXIT_FAILURE);
    }

    // Construct the HTTP GET request
    char request[1024];
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n\r\n",
             path, hostname);

    // Send the HTTP request
    ssize_t bytes_sent = send(sock, request, strlen(request), 0);
    if (bytes_sent == -1)
    {
        perror("send");
        close(sock);
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    // Receive and print the HTTP response
    char response[100000];
    ssize_t bytes_received;
    while ((bytes_received = recv(sock, response, sizeof(response) - 1, 0)) > 0)
    {
        response[bytes_received] = '\0';
        remove_html_tags(response);
        if (strstr(response, "NAME") == NULL)
            printf("%s", response);
        else
        {
            char *str = strstr(response, "NAME");
            printf("%s", str);
        }

        if (bytes_received == -1)
        {
            perror("recv");
        }

        close(sock);
        freeaddrinfo(result);
    }
}