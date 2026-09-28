#include "cmds.h"
#include "usrmanager.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

void wait_for_enter() {
    int c;

    while ((c = getchar()) != '\n' && c != EOF);

    printf("Нажмите Enter чтобы продолжить...");
    getchar();
}

void read_line(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}

void Help_Act(struct Command* self) {
    (void)&self;
    system(CLEAR);
    printf("Список доступных комманд: \n");
    printf("new_usr -> Создание нового пользователя. \n");
    printf("usrs -> Посмотреть список пользователей \n");
    printf("remove_usr -> Удалить пользователя. \n");
    printf("exit - Выйти из программы. \n");
    printf("\n"); 
    wait_for_enter(); 
}

void NewUser_Act(struct Command* self) {
    NewUser* this_cmd = (NewUser*)self;

    char usr_name[100];
    char quote[100];
    int age;

    system(CLEAR);

    printf("Создание нового пользователя...\n\n");

    // Убираем \n, оставшийся после scanf("%s", command)
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Введите имя пользователя: ");
    read_line(usr_name, sizeof(usr_name));

    printf("Введите возраст: "); 
    scanf(" %d", &age);

    // Убираем \n после scanf("%d")
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Введите его цитату: ");
    read_line(quote, sizeof(quote));

    new_usr(this_cmd->manager, usr_name, age, quote);

    system(CLEAR);
    printf("Пользователь успешно создан.\n\n");
    printf("> "); getchar(); 
}

void ReadUsers_Act(struct Command* self) {
    ReadUsers* this_cmd = (ReadUsers*)self; 
    if (this_cmd->manager->count == 0) {
        system(CLEAR);
        printf("Список пользователей пуст -_- \n"); 
        printf("\n"); 
        printf("> "); wait_for_enter(); 
        return; 
    }

    while (1) {
        system(CLEAR);
        printf("Список пользователей: \n"); 
        for (int i = 0; i < this_cmd->manager->count; i++) {
            printf("%d. %s\n", this_cmd->manager->users[i].id, this_cmd->manager->users[i].usr_name);
        }
        printf("\n"); 

        int selected_id; 
        printf("Выберите пользователя (0, %d) > ", this_cmd->manager->count); 
        if (scanf("%d", &selected_id) == 1 && selected_id > 0 && selected_id <= this_cmd->manager->count){
            system(CLEAR);
            User selected_user = this_cmd->manager->users[selected_id-1]; 
            printf("|        Профиль пользователя №%d       |\n", selected_id);
            printf("-----------------------------------------\n"); 
            printf("Имя: %s\n", selected_user.usr_name);
            printf("Возраст: %d\n", selected_user.age);
            printf("Цитата: %s\n", selected_user.quote); 

            printf("\n");
            printf("> "); wait_for_enter();
        } else {
            break; 
        }
    }
}

void DeleteUser_Act(struct Command* self) {
    DeleteUser* this_cmd = (DeleteUser*)self;

    if (this_cmd->manager->count == 0) {
        system(CLEAR);;

        printf("Некого удалять. Пользователей нет :^\n\n");
        printf("> ");

        wait_for_enter();
        return;
    }
    while (1) {
        system(CLEAR);

        printf("Кого удаляем?:\n");

        for (int i = 0; i < this_cmd->manager->count; i++) {
            printf(
                "%d. %s\n",
                this_cmd->manager->users[i].id,
                this_cmd->manager->users[i].usr_name
            );
        }

        printf("\n");

        int selected_id;

        printf(
            "Выберите пользователя (1-%d) или -1 чтобы выйти > ",
            this_cmd->manager->count
        );

        if (scanf("%d", &selected_id) != 1) {
            wait_for_enter();
            continue;
        }

        if (selected_id == -1) {
            break;
        }

        if (selected_id < 1 || selected_id > this_cmd->manager->count) {
            printf("Неверный ID.\n");
            wait_for_enter();
            continue;
        }

        delete_usr(this_cmd->manager, selected_id);

        system(CLEAR);
        printf("\nПользователь удалён.\n");
        printf("\n"); 
        wait_for_enter();
    }
}

Command** get_commands(UserManager *manager) {
    static Help help = {
        .base = {
            .trigger_token = "help",
            .Act = Help_Act
        }
    }; 

    static NewUser new_usr = {
        .base = {
            .trigger_token = "new_usr",
            .Act = NewUser_Act
        }
    };

    static ReadUsers read_urs = {
        .base = {
            .trigger_token = "usrs",
            .Act = ReadUsers_Act
        }
    };

    static DeleteUser delete_usr = {
        .base = {
            .trigger_token = "remove_usr",
            .Act = DeleteUser_Act
        }
    };

    new_usr.manager = manager;
    read_urs.manager = manager;
    delete_usr.manager = manager; 

    static Command* cmd_list[4]; 
    cmd_list[0] = (Command*)&help; 
    cmd_list[1] = (Command*)&new_usr;
    cmd_list[2] = (Command*)&read_urs; 
    cmd_list[3] = (Command*)&delete_usr; 

    return cmd_list;
}
