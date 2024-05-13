#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include "user.h"

class StorageManager
{
protected:
    StorageManager();

    static StorageManager *instance;

    static int const USERS_COUNT = 100;
    
private:
    inline static const char* FILE_NAME;

public:
    StorageManager(StorageManager &other) = delete;

    void operator=(const StorageManager &) = delete;

    static StorageManager *getInstance();


    User **users;
    User **getUsers();
    User *getUserByUid(int uid);
    User *getUserByUsername(const char *username);

    bool readUsers();

    void addUser(User *user);

    template <typename T>
    bool saveToBinaryFile(T *data, int count, int startBytePosition = 0, const char *filename = FILE_NAME);

    template <typename T>
    bool readFromBinaryFile(T *data, int count, int startBytePosition = 0, const char *filename = FILE_NAME);
};

#endif // STORAGEMANAGER_H
