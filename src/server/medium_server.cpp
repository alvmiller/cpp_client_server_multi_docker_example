// Server side C/C++ program to demonstrate Socket programming
// Server - inspired by GeeksforGeeks
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <string.h>

/*
docker

https://iceoryx.io/v2.0.5/examples/icedocker/
https://medium.com/better-programming/dockerizing-multiple-services-inside-a-single-container-96cdff286cef
https://www.freecodecamp.org/news/a-beginners-guide-to-docker-how-to-create-a-client-server-side-with-docker-compose-12c8cf0ae0aa/
https://www.geeksforgeeks.org/software-engineering/docker-compose-tool-to-run-multi-container-applications/
https://docs.docker.com/engine/containers/multi-service_container/
https://www.cyberciti.biz/faq/how-to-run-command-or-code-in-parallel-in-bash-shell-under-linux-or-unix/
https://www.tutorialspoint.com/processing-linux-commands-in-parallel
https://markus-x-buchholz.medium.com/docker-container-networking-c-client-server-app-e9750f003f8
https://www.baeldung.com/ops/docker-run-multiple-commands
https://stackoverflow.com/questions/54121031/multiple-commands-on-docker-entrypoint
https://taiwodevlab.hashnode.dev/running-multiple-commands-on-docker-container-start-cl3gc8etn04k4mynvg4ub3wss
https://jhooq.com/docker-error-executable-file/
https://www.cloudbees.com/blog/understanding-dockers-cmd-and-entrypoint-instructions
https://www.docker.com/blog/docker-best-practices-choosing-between-run-cmd-and-entrypoint/
https://stackoverflow.com/questions/52856059/how-to-launch-and-keep-a-background-process-inside-a-docker-container
https://www.baeldung.com/linux/kill-background-process
https://hannrul.medium.com/launch-a-client-and-http-server-connection-via-terminal-1ccceea9c4fa
https://ubidots.com/blog/how-to-simulate-a-tcpudp-client-using-netcat/
https://labex.io/tutorials/linux-how-to-create-a-tcp-server-and-client-with-netcat-nc-on-linux-415800
https://www.geeksforgeeks.org/devops/connecting-two-docker-containers-over-the-same-network/
https://spacelift.io/blog/docker-networking
https://docs.docker.com/reference/cli/docker/network/create/


sudo apt update && sudo apt upgrade -y
sudo apt install -y ca-certificates curl gnupg
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt update
sudo apt install -y docker-ce docker-ce-cli containerd.io
sudo usermod -aG docker $USER
sudo curl -L "https://github.com/docker/compose/releases/download/$(curl -s https://api.github.com/repos/docker/compose/releases/latest | grep 'tag_name' | cut -d'"' -f4)/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose
docker-compose --version
sudo systemctl enable docker

sudo docker build . -t server:1
sudo docker build . -t client:1

sudo docker network ls
sudo docker network inspect bridge
#sudo docker network create –-driver bridge home_net
sudo docker network create -d bridge home_net1
sudo docker network ls
sudo docker network inspect home_net1
sudo docker run -it -p 7777:7777 --network=home_net1 server:1
sudo docker network inspect home_net
sudo docker run -it --network=home_net1 client:1
sudo docker network inspect home_net1
# Use correct IP addresses!

sudo docker network inspect home_net1
[
    {
        "Name": "home_net1",
        "Id": "1f37188d5f13ad1e510c56c7600e837ef7e6d035ec4c48c5c31d23726bf17f0d",
        "Created": "2025-06-13T23:14:14.462045255+03:00",
        "Scope": "local",
        "Driver": "bridge",
        "EnableIPv4": true,
        "EnableIPv6": false,
        "IPAM": {
            "Driver": "default",
            "Options": {},
            "Config": [
                {
                    "Subnet": "172.19.0.0/16",
                    "Gateway": "172.19.0.1"
                }
            ]
        },
        "Internal": false,
        "Attachable": false,
        "Ingress": false,
        "ConfigFrom": {
            "Network": ""
        },
        "ConfigOnly": false,
        "Containers": {},
        "Options": {},
        "Labels": {}
    }
]

# If use shell -> use manual commands
./server_medium
./client_medium
*/

#define PORT 7777

int main()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cout << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        std::cout << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        std::cout << "bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        std::cout << "listen" << std::endl;
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        std::cout << "accept" << std::endl;
        exit(EXIT_FAILURE);
    }

    double pi = 3.1415;
    std::string str1 = "server => ";
    std::string str2 = std::to_string(pi);

    valread = read(new_socket, buffer, 1024);
    std::cout << buffer << std::endl;
    std::string str = str1 + " : " + str2 + " and " + buffer;
    char *cstr = &str[0];

    send(new_socket, cstr, strlen(cstr), 0);
    std::cout << "Message hass been sent!" << std::endl;

    return 0;
}
