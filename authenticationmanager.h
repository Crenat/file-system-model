#ifndef AUTHENTICATIONMANAGER_H
#define AUTHENTICATIONMANAGER_H

#include "storagemanager.h"

class AuthenticationManager
{
public:
    AuthenticationManager(StorageManager& storageManager) : storageManager(storageManager) {}

    bool authenticate(const std::string& username, const std::string& password) const;
    bool createUser(const std::string& username, const std::string& password, const std::string& firstName, const std::string& lastName) const;

private:
    StorageManager& storageManager;
};

#endif // AUTHENTICATIONMANAGER_H
