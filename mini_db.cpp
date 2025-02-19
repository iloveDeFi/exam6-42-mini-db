#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <csignal>
#include <cstdlib>
#include <netinet/in.h>
#include <unistd.h>

std::map<std::string, std::string> db;
std::string dbf;
int ss; // serveur socket

void load_db()
{
    std::ifstream file(dbf);
    std::string k, v;
    while (file >> k >> v)
        db[k] = v;
}

void save_db(int)
{
    std::ofstream file(dbf);

    if (!file.is_open())
        return;

    for (auto &p : db)
        file << p.first << " " << p.second << "\n";

    file.close();
}

std::string handle_cmd(const std::string &cmd)
{
    std::istringstream iss(cmd);
    std::string op, k, v;
    iss >> op >> k;

    if (op == "GET")
        return db.count(k) ? "0 " + db[k] + "\n" : "1\n";
    if (op == "POST" && iss >> v)
    {
        db[k] = v;
        save_db(0);
        return "0\n";
    }
    if (op == "DELETE")
    {
        bool erased = db.erase(k);
        save_db(0);
        return erased ? "0\n" : "1\n";
    }
    return "2\n";
}

void run_server(int port)
{
    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    socklen_t aln = sizeof(addr);
    char buf[1024];

    ss = socket(AF_INET, SOCK_STREAM, 0);
    bind(ss, (struct sockaddr *)&addr, sizeof(addr));
    listen(ss, 5);

    while (1)
    {
        int client = accept(ss, (struct sockaddr *)&addr, &aln);
        if (client < 0)
            continue;

        ssize_t r;
        while ((r = recv(client, buf, sizeof(buf) - 1, 0)) > 0)
        {
            buf[r] = '\0';
            std::istringstream in(buf);
            std::string line;

            while (std::getline(in, line))
            {
                std::string response = handle_cmd(line);
                send(client, response.c_str(), response.size(), 0);
            }
        }
        close(client);
    }
}

int main(int ac, char **av)
{
    if (ac != 3)
        return 1;
    dbf = av[2];
    signal(SIGINT, save_db);
    load_db();
    run_server(std::atoi(av[1]));
    return 0;
}
