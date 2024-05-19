#include "storagemanager.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <QDebug>

#include "user.h"
#include "inode.h"

constexpr char StorageManager::FILE_NAME[];

StorageManager& StorageManager::getInstance() {
    static StorageManager instance_;
    return instance_;
}

template<typename EntityType>
void StorageManager::writeEntity(const EntityType& entity, std::streamoff startPos) const {
    std::fstream& fileStream = getFileStream();
    fileStream.seekp(startPos, std::ios::beg);
    fileStream.write(reinterpret_cast<const char*>(&entity), sizeof(EntityType));
    fileStream.close();
}

template<typename EntityType>
EntityType StorageManager::readEntity(std::streamoff startPos) const {
    EntityType entity;
    std::fstream& fileStream = getFileStream();
    fileStream.seekg(startPos, std::ios::beg);
    fileStream.read(reinterpret_cast<char*>(&entity), sizeof(EntityType));
    fileStream.close();
    return entity;
}

void StorageManager::buildFileSystem() const {
    std::fstream& fileStream = getFileStream();

    fileStream.seekp(0, std::ios::beg);

    for (int i = 0; i < USERS_COUNT; ++i) {
        User user;
        user.uid = -1;
        writeEntity<User>(user, getEntityStartPosition<User>(i));
    }

    fileStream.close();
}

std::vector<User> StorageManager::getUsers() const {
    std::vector<User> users;
    getEntityStartPosition<User>(0);
    for (int i = 0; i < USERS_COUNT; ++i) {
        std::streamoff startPos = getEntityStartPosition<User>(i);
        User user = readEntity<User>(startPos);
        if (user.uid != -1 && user.uid < USERS_COUNT + 1)
        users.push_back(user);
    }
    return users;
}

User StorageManager::getUserByUid(int uid) const {
    for (int i = 0; i < USERS_COUNT; ++i) {
        std::streampos startPos = getEntityStartPosition<User>(i);
        User user = readEntity<User>(startPos);
        if (user.uid == uid) {
            return user;
        }
    }
    return User();
}

User StorageManager::getUserByUsername(const std::string& username) const {
    for (int i = 0; i < USERS_COUNT; ++i) {
        std::streampos startPos = getEntityStartPosition<User>(i);
        User user = readEntity<User>(startPos);
        if (strcmp(user.username, username.c_str()) == 0) {
            return user;
        }
    }
    return User();
}

bool StorageManager::addUser(const User& user) {
    std::vector<User> users = getUsers();
    users.push_back(user);
    persistUsersInBinaryFile(users);
    return true;
}

void StorageManager::persistUsersInBinaryFile(const std::vector<User>& users) {
    for (size_t i = 0; i < users.size(); ++i) {
        std::streampos startPos = getEntityStartPosition<User>(i);
        writeEntity<User>(users[i], startPos);
    }
}

std::fstream& StorageManager::getFileStream() const {
    if (!fileStream_.is_open()) {
        fileStream_.open(FILE_NAME, std::ios::binary | std::ios::in | std::ios::out);
        if (!fileStream_.is_open()) {

            std::ios_base::iostate state = fileStream_.rdstate();

            if (state & std::ios_base::eofbit)
            {
                std::cout << "End of file reached." << std::endl;
            }
            if (state & std::ios_base::failbit)
            {
                std::cout << "Non-fatal I/O error occurred." << std::endl;
            }
            if (state & std::ios_base::badbit)
            {
                std::cout << "Fatal I/O error occurred." << std::endl;
            }

            std::perror("Error: ");

            // In this case the file is not created yet, so it should be created
            fileStream_.open(FILE_NAME, std::ios::binary | std::ios::out);
            if (!fileStream_.is_open())
                throw std::runtime_error("Failed to open file");

            buildFileSystem();

            fileStream_.close();
            return getFileStream();
        }
    }

    return fileStream_;
}

template<typename EntityType>
std::streamoff StorageManager::getEntityStartPosition(int entityIndex) const {
    return sizeof(EntityType) * entityIndex;
}

template<typename EntityType>
int StorageManager::getEntityGroupSize(int entitiesCount) const {
    return sizeof(EntityType) * entitiesCount + resolveEntityGroupStartPosition<EntityType>();
}

template<typename EntityType>
int StorageManager::resolveEntityGroupStartPosition() const {
    switch (sizeof(EntityType)) {
        case sizeof(User):
            return 0;
        case sizeof(Inode):
            return USERS_COUNT * sizeof(User);
        default:
            return 0;
    }
}
