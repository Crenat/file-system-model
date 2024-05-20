#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <fstream>
#include <iostream>
#include <vector>

#include "user.h"
#include "inode.h"

class StorageManager {
public:
    static constexpr char FILE_NAME[] = "/Users/crenat/filesystem.dat";
    static constexpr int USERS_COUNT = 100;
    static constexpr int INODES_COUNT = 20000;
    static constexpr int BLOCK_SIZE = 512;

    static StorageManager& getInstance();

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

    // Inodes related methods
    std::vector<Inode> getInodes() const;
    Inode getInodeByUid(int uid) const;
    Inode getInodeByLocation(const std::string& location) const;
    std::vector<Inode> getInodesForLocation(const std::string& location) const;
    std::vector<Inode> getDirectoriesForLocation(const std::string& location) const;
    std::vector<Inode> getFilesForLocation(const std::string& location) const;
    bool addInode(const Inode& inode);
    void persistInodesInBinaryFile(const std::vector<Inode>& inodes);
    int getAvailableSpace() const;

private:
    static StorageManager instance_;

    // Private methods for internal use
    std::fstream& getFileStream() const;

    template<typename EntityType>
    std::streamoff getEntityStartPosition(int entityIndex) const;

    template<typename EntityType>
    int getEntityGroupSize(int entitiesCount) const;

    template<typename EntityType>
    int resolveEntityGroupStartPosition() const;

    // Private data members
    mutable std::fstream fileStream_;
};

#endif // STORAGEMANAGER_H
