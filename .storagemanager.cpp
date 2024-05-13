#include "storagemanager.h"
#include <string.h>

#include <stdio.h>

StorageManager *StorageManager::instance = nullptr;

StorageManager *StorageManager::getInstance() {
    if (instance == nullptr) {
        instance = new StorageManager();
    }

    return instance;
}

const char* StorageManager::FILE_NAME = "filesystem.dat";

StorageManager::StorageManager() {
  users = new User*[USERS_COUNT];
  const char* FILE_NAME = "filesystem.dat";

  readUsers();
}

template <typename T>
    bool StorageManager::saveToBinaryFile(T *data, int count, int startBytePosition, const char *filename)
    {
        FILE *file = fopen(filename, "wb");
        if (file != NULL)
        {
            fseek(file, startBytePosition, SEEK_SET);
            fwrite(data, sizeof(T), count, file);
            fclose(file);
            return true;
        }

        return false;
    }

template <typename T>
    bool StorageManager::readFromBinaryFile(T *data, int count, int startBytePosition, const char *filename)
    {
        FILE *file = fopen(filename, "rb");
        if (file != NULL)
        {
            fseek(file, startBytePosition, SEEK_SET);
            fread(data, sizeof(T), count, file);
            fclose(file);
            return true;
        }

        return false;
    }

User *StorageManager::getUserByUid(int uid) {
    for (int i = 0; i < 10; i++) {
        if (users[i]->uid == uid) {
            return users[i];
        }
    }

    return nullptr;
}

bool StorageManager::readUsers() {
    return readFromBinaryFile(users, USERS_COUNT);
}

User *StorageManager::getUserByUsername(const char *username) {
    for (int i = 0; i < 10; i++) {
        if (strcmp(users[i]->username, username) == 0) {
            return users[i];
        }
    }

    return nullptr;
}

User **StorageManager::getUsers() {
    return users;
}

void StorageManager::addUser(User *user) {
    for (int i = 0; i < 10; i++) {
        if (users[i] == nullptr) {
            users[i] = user;
            return;
        }
    }
}
