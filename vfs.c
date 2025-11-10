#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define NAME_LEN 64
#define CMD_LEN 1024
#define PATH_SEGMENTS 128

typedef struct FreeBlock
{
    int index;
    struct FreeBlock *prev;
    struct FreeBlock *next;
} FreeBlock;

typedef struct FileNode
{
    char name[NAME_LEN];
    int isDirectory;
    int blockPointers[NUM_BLOCKS];
    int blockCount;
    struct FileNode *next;
    struct FileNode *parent;
    struct FileNode *child;
} FileNode;

unsigned char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];
FreeBlock *freeListHead = NULL;
FileNode *root = NULL;
FileNode *cwd = NULL;

void my_strcpy(char *dest, const char *src)
{
    while (*src)
    {
        *dest++ = *src++;
    }
    *dest = '\0';
}

int my_streq(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (*a != *b)
        {
            return 0;
        }
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}

int my_strlen(const char *str)
{
    int len = 0;
    while (*str++)
    {
        len++;
    }
    return len;
}

void initFreeList()
{
    freeListHead = NULL;
    for (int index = 0; index < NUM_BLOCKS; ++index)
    {
        FreeBlock *newBlock = (FreeBlock*)malloc(sizeof(FreeBlock));
        newBlock->index = index;
        newBlock->prev = newBlock->next = NULL;
        if (!freeListHead)
        {
            freeListHead = newBlock;
        }
        else
        {
            FreeBlock *temp = freeListHead;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = newBlock;
            newBlock->prev = temp;
        }
    }
}

FreeBlock* popFreeHead()
{
    if (!freeListHead)
    {
        return NULL;
    }
    FreeBlock *freeBlock = freeListHead;
    freeListHead = freeBlock->next;
    if (freeListHead)
    {
        freeListHead->prev = NULL;
    }
    freeBlock->next = freeBlock->prev = NULL;
    return freeBlock;
}

void pushFreeTail(int index)
{
    FreeBlock *freeBlock = (FreeBlock*)malloc(sizeof(FreeBlock));
    freeBlock->index = index;
    freeBlock->next = freeBlock->prev = NULL;
    if (!freeListHead)
    {
        freeListHead = freeBlock;
        return;
    }
    FreeBlock *temp = freeListHead;
    while (temp->next)
    {
        temp = temp->next;
    }
    temp->next = freeBlock;
    freeBlock->prev = temp;
}

FileNode* newNode(const char *name, int isDir, FileNode *parent)
{
    FileNode *node = (FileNode*)malloc(sizeof(FileNode));
    my_strcpy(node->name, name);
    node->isDirectory = isDir;
    node->child = NULL;
    node->next = NULL;
    node->parent = parent;
    node->blockCount = 0;
    for (int idx = 0; idx < NUM_BLOCKS; idx++)
    {
        node->blockPointers[idx] = -1;
    }
    return node;
}

void insertChild(FileNode *parent, FileNode *child)
{
    if (!parent || !child)
    {
        return;
    }
    child->parent = parent;
    if (!parent->child)
    {
        parent->child = child;
        child->next = child;
        return;
    }
    FileNode *temp = parent->child;
    while (temp->next != parent->child)
    {
        temp = temp->next;
    }
    temp->next = child;
    child->next = parent->child;
}

FileNode* findChild(FileNode *dir, const char *name)
{
    if (!dir || !dir->child)
    {
        return NULL;
    }
    FileNode *temp = dir->child;
    do
    {
        if (my_streq(temp->name, name))
        {
            return temp;
        }
        temp = temp->next;
    } while (temp != dir->child);
    return NULL;
}

int removeChildFromParent(FileNode *parent, FileNode *child)
{
    if (!parent || !child || !parent->child)
    {
        return 0;
    }
    FileNode *current = parent->child;
    FileNode *previous = NULL;
    do
    {
        if (current == child)
        {
            break;
        }
        previous = current;
        current = current->next;
    } while (current != parent->child);
    if (current != child)
    {
        return 0;
    }
    if (current->next == current)
    {
        parent->child = NULL;
        current->next = NULL;
        return 1;
    }
    if (previous == NULL)
    {
        FileNode *last = parent->child;
        while (last->next != parent->child)
        {
            last = last->next;
        }
        last->next = current->next;
        parent->child = current->next;
    }
    else
    {
        previous->next = current->next;
        if (parent->child == current)
        {
            parent->child = current->next;
        }
    }
    current->next = NULL;
    return 1;
}

void cmd_mkdir(const char *name)
{
    if (findChild(cwd, name))
    {
        printf("Directory already exists.\n\n");
        return;
    }
    FileNode *dirNode = newNode(name, 1, cwd);
    insertChild(cwd, dirNode);
    printf("Directory '%s' created successfully.\n\n", name);
}

void cmd_create(const char *name)
{
    if (findChild(cwd, name))
    {
        printf("File already exists.\n\n");
        return;
    }
    FileNode *fileNode = newNode(name, 0, cwd);
    insertChild(cwd, fileNode);
    printf("File '%s' created successfully.\n\n", name);
}

void cmd_write(const char *name, const char *content)
{
    FileNode *fileNode = findChild(cwd, name);
    if (!fileNode || fileNode->isDirectory)
    {
        printf("File not found.\n\n");
        return;
    }
    int length = my_strlen(content);
    int blocksNeeded = (length + BLOCK_SIZE - 1) / BLOCK_SIZE;
    for (int idx = 0; idx < fileNode->blockCount; idx++)
    {
        if (fileNode->blockPointers[idx] >= 0)
        {
            pushFreeTail(fileNode->blockPointers[idx]);
            fileNode->blockPointers[idx] = -1;
        }
    }
    fileNode->blockCount = 0;
    for (int block = 0; block < blocksNeeded; block++)
    {
        FreeBlock *freeBlock = popFreeHead();
        if (!freeBlock)
        {
            for (int j = 0; j < fileNode->blockCount; j++)
            {
                if (fileNode->blockPointers[j] >= 0)
                {
                    pushFreeTail(fileNode->blockPointers[j]);
                    fileNode->blockPointers[j] = -1;
                }
            }
            fileNode->blockCount = 0;
            printf("Disk full!\n\n");
            return;
        }
        int blockIndex = freeBlock->index;
        free(freeBlock);
        fileNode->blockPointers[fileNode->blockCount++] = blockIndex;
        for (int j = 0; j < BLOCK_SIZE; j++)
        {
            int pos = block * BLOCK_SIZE + j;
            if (pos >= length)
            {
                break;
            }
            virtualDisk[blockIndex][j] = (unsigned char)content[pos];
        }
        for (int j = (length - block * BLOCK_SIZE); j < BLOCK_SIZE; j++)
        {
            if (j >= 0 && j < BLOCK_SIZE && (block * BLOCK_SIZE + j) >= length)
            {
                virtualDisk[blockIndex][j] = 0;
            }
        }
    }
    printf("Data written successfully (size=%d bytes).\n\n", length);
}

void cmd_read(const char *name)
{
    FileNode *fileNode = findChild(cwd, name);
    if (!fileNode || fileNode->isDirectory)
    {
        printf("File not found.\n\n");
        return;
    }
    if (fileNode->blockCount == 0)
    {
        printf("(empty)\n\n");
        return;
    }
    for (int b = 0; b < fileNode->blockCount; b++)
    {
        int blockIndex = fileNode->blockPointers[b];
        for (int j = 0; j < BLOCK_SIZE; j++)
        {
            unsigned char ch = virtualDisk[blockIndex][j];
            if (ch == 0)
            {
                break;
            }
            putchar(ch);
        }
    }
    printf("\n\n");
}

void cmd_ls()
{
    if (!cwd->child)
    {
        printf("(empty)\n\n");
        return;
    }
    FileNode *temp = cwd->child;
    do
    {
        printf("%s%s\n", temp->name, temp->isDirectory ? "/" : "");
        temp = temp->next;
    } while (temp != cwd->child);
    printf("\n");
}

void cmd_cd(const char *name)
{
    if (my_streq(name, ".."))
    {
        if (cwd->parent)
        {
            cwd = cwd->parent;
        }
        if (cwd == root)
        {
            printf("Moved to /\n\n");
        }
        else
        {
            printf("Moved to /%s\n\n", cwd->name);
        }
        return;
    }
    FileNode *dirNode = findChild(cwd, name);
    if (!dirNode || !dirNode->isDirectory)
    {
        printf("Directory not found.\n\n");
        return;
    }
    cwd = dirNode;
    if (cwd == root)
    {
        printf("Moved to /\n\n");
    }
    else
    {
        printf("Moved to /%s\n\n", cwd->name);
    }
}

void cmd_delete(const char *name)
{
    FileNode *fileNode = findChild(cwd, name);
    if (!fileNode || fileNode->isDirectory)
    {
        printf("File not found.\n\n");
        return;
    }
    for (int idx = 0; idx < fileNode->blockCount; idx++)
    {
        if (fileNode->blockPointers[idx] >= 0)
        {
            pushFreeTail(fileNode->blockPointers[idx]);
        }
    }
    removeChildFromParent(cwd, fileNode);
    free(fileNode);
    printf("File deleted successfully.\n\n");
}

void cmd_rmdir(const char *name)
{
    FileNode *dirNode = findChild(cwd, name);
    if (!dirNode || !dirNode->isDirectory)
    {
        printf("Directory not found.\n\n");
        return;
    }
    if (dirNode->child)
    {
        printf("Directory not empty. rmdir failed.\n\n");
        return;
    }
    removeChildFromParent(cwd, dirNode);
    free(dirNode);
    printf("Directory removed successfully.\n\n");
}

void cmd_pwd()
{
    char parts[PATH_SEGMENTS][NAME_LEN];
    int count = 0;
    FileNode *temp = cwd;
    while (temp && temp != root)
    {
        my_strcpy(parts[count++], temp->name);
        temp = temp->parent;
    }
    if (count == 0)
    {
        printf("/\n\n");
        return;
    }
    for (int i = count - 1; i >= 0; --i)
    {
        printf("/%s", parts[i]);
    }
    printf("\n\n");
}

void cmd_df()
{
    int freeCount = 0;
    FreeBlock *temp = freeListHead;
    while (temp)
    {
        freeCount++;
        temp = temp->next;
    }
    int used = NUM_BLOCKS - freeCount;
    double percent = 0.0;
    if (NUM_BLOCKS > 0)
    {
        percent = ((double)used / NUM_BLOCKS) * 100.0;
    }
    printf("Total Blocks: %d\n", NUM_BLOCKS);
    printf("Used Blocks: %d\n", used);
    printf("Free Blocks: %d\n", freeCount);
    printf("Disk Usage: %.2f%%\n\n", percent);
}

void cleanup_nodes(FileNode *dir)
{
    if (!dir)
    {
        return;
    }
    if (dir->child)
    {
        FileNode *start = dir->child;
        FileNode *current = start;
        do
        {
            FileNode *next = current->next;
            if (current->isDirectory)
            {
                cleanup_nodes(current);
            }
            for (int i = 0; i < current->blockCount; i++)
            {
                if (current->blockPointers[i] >= 0)
                {
                    pushFreeTail(current->blockPointers[i]);
                }
            }
            free(current);
            current = next;
        } while (current != start);
        dir->child = NULL;
    }
}

void cleanup_all()
{
    if (root)
    {
        cleanup_nodes(root);
        free(root);
        root = cwd = NULL;
    }
    FreeBlock *temp = freeListHead;
    while (temp)
    {
        FreeBlock *next = temp->next;
        free(temp);
        temp = next;
    }
    freeListHead = NULL;
}

void print_prompt()
{
    printf("%s > ", cwd == root ? "/" : cwd->name);
}

int main()
{
    initFreeList();
    root = newNode("/", 1, NULL);
    cwd = root;
    printf("Compact VFS - ready. Type 'exit' to quit.\n\n");
    char line[CMD_LEN];
    while (1)
    {
        print_prompt();
        if (!fgets(line, sizeof(line), stdin))
        {
            break;
        }
        int pos = 0;
        while (line[pos] == ' ' || line[pos] == '\t')
        {
            pos++;
        }
        if (line[pos] == '\n' || line[pos] == '\0')
        {
            continue;
        }
        char cmd[64] = {0}, arg1[NAME_LEN] = {0}, rest[CMD_LEN] = {0};
        int i = 0;
        while (line[pos] && line[pos] != ' ' && line[pos] != '\t' && line[pos] != '\n')
        {
            if (i < (int)sizeof(cmd) - 1)
            {
                cmd[i++] = line[pos];
            }
            pos++;
        }
        cmd[i] = '\0';
        while (line[pos] == ' ' || line[pos] == '\t')
        {
            pos++;
        }
        i = 0;
        while (line[pos] && line[pos] != ' ' && line[pos] != '\t' && line[pos] != '\n')
        {
            if (i < (int)sizeof(arg1) - 1)
            {
                arg1[i++] = line[pos];
            }
            pos++;
        }
        arg1[i] = '\0';
        while (line[pos] == ' ' || line[pos] == '\t')
        {
            pos++;
        }
        i = 0;
        while (line[pos] && line[pos] != '\n')
        {
            if (i < (int)sizeof(rest) - 1)
            {
                rest[i++] = line[pos];
            }
            pos++;
        }
        rest[i] = '\0';
        if (my_streq(cmd, "exit"))
        {
            printf("Memory released. Exiting program...\n");
            cleanup_all();
            break;
        }
        else if (my_streq(cmd, "mkdir"))
        {
            if (arg1[0] == '\0')
            {
                printf("Usage: mkdir <foldername>\n\n");
                continue;
            }
            cmd_mkdir(arg1);
        }
        else if (my_streq(cmd, "ls"))
        {
            cmd_ls();
        }
        else if (my_streq(cmd, "cd"))
        {
            if (arg1[0] == '\0')
            {
                printf("Usage: cd <foldername>\n\n");
                continue;
            }
            cmd_cd(arg1);
        }
        else if (my_streq(cmd, "create"))
        {
            if (arg1[0] == '\0')
            {
                printf("Usage: create <filename>\n\n");
                continue;
            }
            cmd_create(arg1);
        }
        else if (my_streq(cmd, "write"))
        {
            if (arg1[0] == '\0')
            {
                printf("Usage: write <filename> <content>\n\n");
                continue;
            }
            char *content = rest;
            while (*content == ' ')
            {
                content++;
            }
            if (*content == '"')
            {
                content++;
                int len = my_strlen(content);
                if (len > 0 && content[len - 1] == '"')
                {
                    content[len - 1] = '\0';
                }
            }
            cmd_write(arg1, content);
        }
        else if (my_streq(cmd, "read"))
        {
            if (arg1[0] == '\0')
            {
                printf("Usage: read <filename>\n\n");
                continue;
            }
            cmd_read(arg1);
        }
        else if (my_streq(cmd, "delete"))
        {
            if (arg1[0] == '\0')
            {
                printf("Usage: delete <filename>\n\n");
                continue;
            }
            cmd_delete(arg1);
        }
        else if (my_streq(cmd, "rmdir"))
        {
            if (arg1[0] == '\0')
            {
                printf("Usage: rmdir <foldername>\n\n");
                continue;
            }
            cmd_rmdir(arg1);
        }
        else if (my_streq(cmd, "pwd"))
        {
            cmd_pwd();
        }
        else if (my_streq(cmd, "df"))
        {
            cmd_df();
        }
        else
        {
            printf("Invalid command.\n\n");
        }
    }
    return 0;
}
          