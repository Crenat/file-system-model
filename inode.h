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
    bool isDirectory;
    int size;
    InodeAccessRights accessRights;
};


#endif // INODE_H
