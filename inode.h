#ifndef INODE_H
#define INODE_H

struct InodeAccessRights
{
  bool read;
  bool write;
};

struct Inode
{
    int uid;
    char name[50];
    char location[100];
    bool isDirectory;
    int size;
    int ownerUid;
    InodeAccessRights accessRights;
};


#endif // INODE_H
