#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usrmanager.h"

void init_manager(UserManager *manager) {
    manager->count = 0;
    manager->capacity = 4;
    manager->users = malloc(manager->capacity * sizeof(User));

    if (manager->users == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        manager->capacity = 0;
    }
}

void new_usr(UserManager *manager, const char *name, const int age, const char quote[100]) {
    if (manager->count >= manager->capacity) {
        User* tempUsrs = realloc(manager->users, (manager->capacity * 2) * sizeof(User)); 
        if (tempUsrs != NULL) {
            manager->users = tempUsrs; 
            manager->capacity *= 2;
        }
        else {
            fputs("Ошибка с realloc", stderr);
            return; 
        }
    }

    User* current_usr = &manager->users[manager->count];
    strncpy(current_usr->usr_name, name, sizeof(current_usr->usr_name) - 1);
    strncpy(current_usr->quote, quote, sizeof(current_usr->quote) - 1);
    
    current_usr->usr_name[sizeof(current_usr->usr_name) - 1] = '\0';
    current_usr->quote[sizeof(current_usr->quote) - 1] = '\0'; 

    current_usr->age = age; 
    current_usr->id = manager->count + 1; 
    manager->count++; 
}

int delete_usr(UserManager *manager, int id) {
    if (manager->count == 0) 
        return -1; 

    int target_index = -1; 
    for (int i = 0; i < manager->count; i++) {
        if (manager->users[i].id == id) {
            target_index = i; 
            break;
        }
    }

    if (target_index == -1) return -1; 
    int elements_to_move = manager->count - target_index - 1; 
    if (elements_to_move > 0) {
        memmove(&manager->users[target_index], 
            &manager->users[target_index+1], 
            elements_to_move * sizeof(User)
        ); 
    }

    manager->count--; 
    return 0; 
}

void save_to_file(const UserManager *manager, const char *filename) {
    FILE* file = fopen(filename, "wb"); 
    if (file == NULL) return; 

    fwrite(&manager->count, sizeof(int), 1, file); 
    if (manager->count > 0) 
        fwrite(manager->users, sizeof(User), manager->count, file); 

    fclose(file); 
}

void load_from_file(UserManager *manager, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
        return;

    int saved_count = 0;
    if (fread(&saved_count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return;
    }

    if (saved_count < 0) {
        fclose(file);
        return;
    }

    if (saved_count > 0) {
        User *temp_usrs = realloc(manager->users, saved_count * sizeof(User));

        if (temp_usrs == NULL) {
            fclose(file);
            return;
        }

        manager->users = temp_usrs;
        manager->count = saved_count;
        manager->capacity = saved_count;

        fread(
            manager->users, sizeof(User),
            manager->count,file
        );
    } else {
        manager->count = 0;
    }

    fclose(file);
}

void free_manager(UserManager *manager) {
    free(manager->users); 
    manager->users = NULL; 
    manager->capacity = 0; 
    manager->count = 0; 
}