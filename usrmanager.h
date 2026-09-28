#ifndef USRMANAGER_H 
#define USRMANAGER_H

typedef struct {
    int id; 
    char usr_name[100]; 
    int age;
    char quote[100];
} User;

typedef struct {
    User* users; 
    int count; 
    int capacity;
} UserManager;

void init_manager(UserManager* manager);
void new_usr(UserManager* manager, const char* name, const int age, const char quote[100]);
int delete_usr(UserManager* manager, int id); 
void free_manager(UserManager* manager);

void save_to_file(const UserManager* manager, const char* filename); 
void load_from_file(UserManager* manager, const char* filename); 

#endif