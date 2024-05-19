#include "authenticationmanager.h"
#include "user.h"

#include <QDebug>

bool AuthenticationManager::authenticate(const std::string& username, const std::string& password) const {
    User candidate = storageManager.getUserByUsername(username);
    return candidate.uid != -1 && candidate.password == password;
}

bool AuthenticationManager::createUser(const std::string& username, const std::string& password, const std::string& firstName, const std::string& lastName) const {
    User candidate = storageManager.getUserByUsername(username);
    if (candidate.uid != -1) {
        return false;
    }

    User newUser;
    newUser.uid = storageManager.getUsers().size() + 1;
    strcpy(newUser.username, username.c_str());
    strcpy(newUser.password, password.c_str());
    strcpy(newUser.firstName, firstName.c_str());
    strcpy(newUser.lastName, lastName.c_str());

    return storageManager.addUser(newUser);
}
