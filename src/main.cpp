#include <argumentsea.hpp>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <print>
#include <string>

extern "C" {
    #include <scok.h>
}

#define PORT 45434

void help(ArgumentsManager* manager) {
    std::println("Help index: {}", manager->get_index());
}
void catcher(ArgumentsManager* manager) {
    std::println("Wrong argument index: {}", manager->get_index());
}
int main(int argc, char** argv) {
    ArgumentsManager manager(argv, argc, 1);

    manager.set_catcher(catcher);
    manager.add("help", help);

    manager.add("send", [](ArgumentsManager* manager){
        if (!(manager->get_blocked_arguments().size() > 1)) {
            std::println("File or ip not selected");
            exit(1);
        }
        const std::string filepath = manager->get_blocked_arguments().at(0);
        const std::string to = manager->get_blocked_arguments().at(1);

        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::println("File not found");
            exit(1);
        }

        std::string buffer;
        std::string line;
        while (std::getline(file, line)) { buffer += line + '\n'; }

        file.close();

        struct tcp client;

        init_tcp(&client, PORT);
        set_tcp_addr(&client, to.c_str());
        connect_socket(&client);

        int size = TO_INT(buffer.size());
        s_write(client, TO_SOCKET_MESSAGE(size));
        size = TO_INT(std::filesystem::path(filepath).filename().string().size());
        s_write(client, TO_SOCKET_MESSAGE(size));
        
        for (char c : std::filesystem::path(filepath).filename().string()) {
            int cc = TO_INT(c);
            s_write(client, TO_SOCKET_MESSAGE(cc));
        }
        for (char c : buffer) {
            int cc = TO_INT(c);
            s_write(client, TO_SOCKET_MESSAGE(cc));
        }

        closesocket(client);
    });
    manager.block_next_arguments("send", 2);
    manager.add("get", [](ArgumentsManager* manager){
        struct tcp server;

        init_tcp(&server, PORT);
        set_tcp_addr(&server, OPENADDRESS);
        bind_tcp(&server, 1);

        std::println("Waiting for file...");

        struct tcp client;
        get_connect(server, &client);

        struct tcpclient info;
        set_tcp_struct(&client, &info);

        std::println("Ip {} port {} connected", info.ip, info.port);

        int size = 0;
        s_read(client, TO_SOCKET_MESSAGE(size));
        size = FROM_INT(size);
        std::println("File size: {} bytes", size);
        int size_name = 0;
        s_read(client, TO_SOCKET_MESSAGE(size_name));
        size_name = FROM_INT(size_name);

        std::string buffer;
        std::string name;
        for (int i = 0; i < size_name; i++) {
            int c = 0;
            s_read(client, TO_SOCKET_MESSAGE(c));
            name += FROM_INT(c);
        }
        std::println("File name: {}", name);
        for (int i = 0; i < size; i++) {
            int c = 0;
            s_read(client, TO_SOCKET_MESSAGE(c));
            buffer += FROM_INT(c);
        }
        std::ofstream f(name);
        f.write(buffer.c_str(), buffer.size());
        f.close();

        closesocket(client);
        closesocket(server);
    });

    manager.run();

    return 0;
}