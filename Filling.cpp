

#include <fstream>
#include <filesystem>

#include "Filling.h"

#include "Helper.h"
// extern Helper helper;

namespace fs = std::filesystem;

void FileManager::addPlayer(Player &P)
{
    if (!fs::exists("Data"))
        fs::create_directory("Data");

    string filename = "Data/" + P.getUsername() + ".txt";
    ofstream file(filename);

    if (file.is_open())
    {
        file << P << endl;

        file.close();
    }

    saveLeaderBoard(P);
}

void FileManager::addNewPlayer(const string &username)
{

    if (!fs::exists("Data"))
        fs::create_directory("Data");

    string filename = "Data/" + username + ".txt";
    ofstream file(filename);

    if (file.is_open())
    {
        file << username << endl;
        file << "100" << endl;
        file << "0" << endl
             << endl;

        file << "Dragon" << endl;
        file << "300 300 1" << endl;
        file << "50 50 1" << endl;
        file << "20 " << endl;
        file << "20 " << endl;
        file << "1" << endl;
        file << "0" << endl;
        file << "Tank" << endl
             << endl;

        file << "Phoenix" << endl;
        file << "300 300 1" << endl;
        file << "50 50 1" << endl;
        file << "20 " << endl;
        file << "20 " << endl;
        file << "1" << endl;
        file << "0" << endl;
        file << "Healer" << endl
             << endl;

        file << "Unicorn" << endl;
        file << "300 300 1" << endl;
        file << "50 50 1" << endl;
        file << "20 " << endl;
        file << "20 " << endl;
        file << "1" << endl;
        file << "0" << endl;
        file << "Guardian" << endl
             << endl;

        file << "Griffin" << endl;
        file << "100 100 1" << endl;
        file << "20 20 1" << endl;
        file << "10 " << endl;
        file << "10 " << endl;
        file << "1" << endl;
        file << "0" << endl;
        file << "Striker" << endl
             << endl;

        file << "0" << endl;
        file << "0" << endl;
        file << "0" << endl;
        file << "0" << endl;

        file << "null" << endl;
        file << "0" << endl;

        file.close();
    }
}

bool FileManager::isRegistered(const string &username)
{
    string filename = "Data/" + username + ".txt";
    return fs::exists(filename);
}

bool FileManager::getPlayer(const string &username, Player &P)
{
    string filename = "Data/" + username + ".txt";
    ifstream file(filename);
    if (!file.is_open())
        return false;

    file >> P;
    // file >> P.username;
    // file >> P.cash;

    file.close();
    return true;
}
