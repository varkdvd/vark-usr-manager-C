#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmds.h"
#include "usrmanager.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#define SAVE_DATA_PATH "usrs_data.bin"
#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

int read_command(UserManager *manager, char* command) {
    if (strcmp(command, "exit") == 0) {
        printf("Выход из программы...\n");
        return 1; 
    }
    
    Command** commands = get_commands(manager); 
    int count_of_commands = 4; 

    for (int i = 0; i < count_of_commands; i++) {
        if (strcmp(commands[i]->trigger_token, command) == 0) {
            commands[i]->Act(commands[i]);
            return 0;
        }
    }

    return 0; 
}

int command_menu(UserManager *manager) {
    bool loop = true;
    char command[20]; 

    while (loop) {
        system(CLEAR);
        printf("Добро пожаловать в Менеджер Пользователей v0.0.1 \n");
        printf("Эта программа используется для создания, изменения и чтения пользовательских учётных записей. \nНапишите help чтоб ознакомиться. \n");

        printf("\n");
        printf("> "); scanf("%19s", command); 
        if (read_command(manager, command) == 1) break;
    }

    return 0; 
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    UserManager manager; 
    init_manager(&manager); 
    load_from_file(&manager, SAVE_DATA_PATH); 
    command_menu(&manager);

    save_to_file(&manager, SAVE_DATA_PATH); 
    free_manager(&manager); 
    return 0; 
}