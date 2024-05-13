#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include "user.h"
#include <fstream>
#include <vector>

class StorageManager {
public:
    static constexpr char FILE_NAME[] = "/Users/crenat/filesystem.dat";
    static constexpr int USERS_COUNT = 100;
    static constexpr int INODES_COUNT = 5000;

    static StorageManager& instance();

    template<typename EntityType>
    void writeEntity(const EntityType& entity, std::streamoff startPos) const;

    template<typename EntityType>
    EntityType readEntity(std::streamoff startPos) const;

    void buildFileSystem() const;

    // Users related methods
    std::vector<User> getUsers() const;
    User getUserByUid(int uid) const;
    User getUserByUsername(const std::string& username) const;
    bool addUser(const User& user);
    void persistUsersInBinaryFile(const std::vector<User>& users);

private:
    // Private constructor for Singleton pattern
    StorageManager() = default;

    // Private methods for internal use
    std::fstream& getFileStream() const;

    template<typename EntityType>
    std::streamoff getEntityStartPosition(int entityIndex) const;

    // Private data members
    mutable std::fstream fileStream_;
};

#endif // STORAGEMANAGER_H
