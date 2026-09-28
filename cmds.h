#ifndef CMDS_H
#define CMDS_H

#include "usrmanager.h"
typedef struct Command {
    const char* trigger_token; 
    void (*Act)(struct Command* self); 
} Command; 

typedef struct { Command base; } Help;
typedef struct { Command base; UserManager* manager; } NewUser;
typedef struct { Command base; UserManager* manager; } ReadUsers;
typedef struct { Command base; UserManager* manager; } DeleteUser;

void Help_Act(struct Command* self);
void NewUser_Act(struct Command* self); 
void ReadUsers_Act(struct Command* self);  
void DeleteUser_Act(struct Command* self); 

Command** get_commands(UserManager* manager); 

#endif  