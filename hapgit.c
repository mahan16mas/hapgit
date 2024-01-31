//code
// #define _XOPEN_SOURCE
// #define __USE_XOPEN
// #define  _GNU_SOURCE
// #define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdint.h>
#define existance_error printf("Hapgit has not been initialized yet!\n")
#define MAX_FILE_NAME 500
#define MAX_COMMAND 100
#define MAX_MES_NAME 100 
#define MAX_HASH 1024
#define MAX_ADD 1024
void undo();
void status();
void where_are_we(char* addres);
int if_exist(char* addres);
void git();
void global_name(char* nameof);
void global_email(char* nameof);
void local_name(char* nameof);
void local_email(char* nameof);
void add(char* name, char* folder);
void reset(char* name, char* addres);
void get_hash(char* hash, char* name);
void add_to_staging(char* name, char* string);
void add_n();
void reset(char* name, char* folder);
void remove_line(char* text_name,int khat);
void redo();
void mkdfolder(char* add);
void remove_non_dir();
int exist(char* addres);
int compare_times(char* first, char* last);
int compare_times(char* first, char* last)
{
    int y1, y2;
    int m1, m2;
    int d1, d2;
    int  h1, min1, h2, min2;
    sscanf(first,"%d/%d/%d %d:%d", &y1, &m1, &d1, &h1, &min1);
    sscanf(last,"%d/%d/%d %d:%d", &y2, &m2, &d2, &h2, &min2);
    // printf("%d %d %d %d %d\n",  y1, m1, d1, h1, min1);
        // printf("%d %d %d %d %d\n",  y2, m2, d2, h2, min2);
    if(y1 > y2)
    {
        return 1;
    }
    if(y2 > y1)
    {
        return -1;
    }
    if(m1 > m2)
    {
        return 1;
    }
    if(m2 > m1)
    {
        return -1;
    }
    if(d1 > d2)
    {
        return 1;
    }
    if(d2 > d1)
    {
        return -1;
    }
    // printf("h1:%d h2:%d", h1, h2);
    if(h1 > h2)
    {
        return 1;
    }
    if(h2 > h1)
    {
        return -1;
    }
    if(min1 > min2)
    {
        return 1;
    }
    if(min2 > min1)
    {
        return -1;
    }
    return -1;
} 
int exist(char* addres)
{
    char folder[MAX_ADD] = ".";
    char current[MAX_ADD];
    char name[MAX_FILE_NAME];
    getcwd(current, sizeof(current));
    for (int i = strlen(addres) - 1; i >= 0; i--)
    {
        if(addres[i] == '/')
            {
                    // printf("folder: %s\nname: %s\n", folder, name);
                for (int j = 0; j < i; j++)
                {
                    folder[j] = addres[j];
                }
                folder[i] = '\0';
                strcpy(name, addres + i + 1);
                if(chdir(folder) != 0)
                {
                    // system("pwd");
                    // printf("wrong address!\n");e
                    return 0;
                }
                break;
            }
    }
    DIR* dir = opendir(".");
    struct dirent* entry;
    while(entry = readdir(dir))
    {
        if(!strcmp(entry->d_name, name) && entry->d_type != 4)
        {
            chdir(current);
            return 1;
        }
    }
    chdir(current);
    return 0;
}   
void remove_non_dir()
{
    // system("pwd");
    DIR* dir = opendir(".");
    struct dirent* entry;
    while(entry = readdir(dir))
    {
        if(entry->d_type == 4 && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, ".hapgit"))
        {
            chdir(entry->d_name);
            remove_non_dir();
            chdir("..");
        }
        else 
        if(entry->d_type != 4)
        {
            remove(entry->d_name);
        }
    }
}
void mkdfolder(char* add)
{
    char hapgit[1024];
    if_exist(hapgit);
    chdir(hapgit);
    chdir("..");
    DIR* dir = opendir(".");
    struct dirent* entry;
    while (entry = readdir(dir))
    {
        if(entry->d_type == 4 && strcmp(entry->d_name, ".")  && strcmp(entry->d_name, "..") && strcmp(entry->d_name, ".hapgit"))
        {
            char command[1024] = "cp -r ";
            strcat(command, entry->d_name);
            strcat(command, " ");
            strcat(command, add);
            // printf("command: %s\n", command);
            system(command);
        }
    }
    chdir(add);
    closedir(dir);
    remove_non_dir();
    
}
void commit(char* message)
{
    //cp
    // system("pwd");
    // exist
    int id;
    char addres[MAX_ADD], current[1024];
    if_exist(addres);
    chdir(addres);
    getcwd(current, sizeof(current));
    char last_commit[1024];

    char current_cum[1024];
    strcpy(last_commit, addres);
    strcpy(current_cum, addres);
    strcat(current_cum, "/current_commit.txt");
    strcat(last_commit, "/last_commit.txt");
    // printf("l commit:%s\n", last_commit);
    FILE* com_id = fopen(last_commit, "r");
    fscanf(com_id, "%d", &id);
    fclose(com_id);
    FILE* curr = fopen(last_commit, "w");
    fprintf(curr, "%d", id + 1);
    FILE* ff = fopen(current_cum, "w");
    fprintf(ff, "%d", id + 1);
    char com_folder[1024];
    strcpy(com_folder, addres);
    strcat(com_folder, "/commits");
    chdir(com_folder);
    char name[10];
    sprintf(name, "%d", id + 1);
    mkdir(name, 0777);
    int flag = 0;
    char last[1024];
    char reallast[1024];
    FILE* cpy;
        char commit_last_commit[1024];
    if(id > 0)
    {
        sprintf(last, "%d", id);
        realpath(last, reallast);
        flag = 1;
        strcpy(commit_last_commit, reallast);
        strcat(commit_last_commit, "/commited.txt");
        // printf("last commit:\t%s\n", commit_last_commit);
        cpy = fopen(commit_last_commit, "r");
    }
    char path[MAX_ADD];
    realpath(name, path);
    mkdfolder(path);
    
    chdir(path);
    // system("pwd");
    FILE* file = fopen("commited.txt", "w");
    if(flag)
    {
        char line3[1024];
        while (fgets(line3, 1024, cpy))
        {
            fprintf(file, "%s", line3);
        }
        fclose(cpy);
        
    }
    fclose(file);
    char laststaged[1024];
    strcpy(laststaged, addres);
    strcat(laststaged, "/staged_files.txt");
    // printf("last staged:%s\n", laststaged);
    FILE* staged = fopen(laststaged, "r");
    char line[1024];
        if(flag){
    while (fgets(line, 1024, staged))
    {
        // printf("kps");
        int i = 0;
        char line2[1024];
        char where[1024] = "commited.txt";
        FILE* read = fopen(where, "r");
        char name[1024];
        char hash[1024];
        sscanf(line, "%s %s\n", name, hash);
        while(fgets(line2, sizeof(line2), read))
        {
            char name2[1024];
            char h[1024];
            sscanf(line2, "%s %s\n", name2, h);
            if(!strcmp(name2, name))
            {
                remove_line(where, i);
                // fclose(file);
                // file = fopen(where, "a");
                break;
            }
            i++;
        }
        // fprintf(file, "%s", line);

    }
        }
    rewind(staged);
    file = fopen("commited.txt", "a");
    char line4[1024];
    int number_of_commited = 0;
    while(fgets(line4, sizeof(line4), staged))
    {
        
        fprintf(file, "%s", line4);
        number_of_commited++;
        
    }
    fclose(staged);
    FILE* fucked_up = fopen(laststaged, "w");
    fclose(fucked_up);
    fclose(file);
    FILE* rd = fopen("commited.txt", "r");
    char line10[MAX_ADD];
    rewind(rd);
    // system("pwd");
    // printf("lso");
    int i = 0;
    while(fgets(line10, sizeof(line10), rd))
    {
    // printf("ki\n");
        char nam[MAX_ADD];
        char has[MAX_HASH];
        sscanf(line10, "%s %s\n", nam, has);
        if(exist(nam))
        {
            char hapgit[MAX_ADD];
            strcpy(hapgit, addres);
            int length = strlen(hapgit);
            char commit_id[MAX_ADD];
            // printf("addres: %s\n", addres);
            strcpy(commit_id, addres);
            char idd[MAX_ADD];
            sprintf(idd, "%d", id + 1);
            strcat(commit_id, "/commits/");
            strcat(commit_id, idd);
            int place = (int)(strstr(hapgit, ".hapgit") - hapgit);
            for (int i = place - 1; i < length; i++)
            {
                *(hapgit + i) = '\0';
            }
            strcat(commit_id, nam + strlen(hapgit));
            for (int i = strlen(commit_id) - 1; i >= 0; i--)
            {
                if(commit_id[i] == '/')
                {
                    for (int j = i; i < strlen(commit_id); j++)
                    {
                        commit_id[j] = '\0';
                    }
                    break;
                }
            }
            char sys[1024] = "cp ";
            strcat(sys, nam);
            strcat(sys," ");
            strcat(sys, commit_id);
            system(sys);
            // printf("%s %s\n", commit_id, nam);
            
        }
        else remove_line("commited.txt", i);
        i++;
    }
    chdir(addres);
    FILE* write = fopen("temp.txt", "w");
    char author[MAX_MES_NAME];
    FILE* name_txt = fopen("name.txt", "r");
    fscanf(name_txt, "%s", author);
    fclose(name_txt);
    time_t rawtime;
     time_t now = time(NULL);
    struct tm *ptm = localtime(&now);

    char buf[256];
    strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M", ptm);
    // printf("%s\n", buf);
    // printf("Current time: %s", asctime(localTime));
    char branch[100];
    FILE* branches = fopen("current_branch.txt", "r");
    fscanf(branches, "%s", branch);
    FILE* list_of_cums = fopen("list_of_commits.txt", "r");
    fprintf(write, "commit id: %d\tauthor: %s\tbranch: %s\tnumber of files commited: %d\tmessage: %s\ttime commited: %s\n", id + 1, author, branch, number_of_commited, message, buf);
    printf("commit id: %d\tmessage: \"%s\"\ttime commited: %s\n", id + 1, message, buf);
    fclose(write);
    write = fopen("temp.txt", "a");
    char liin[MAX_ADD];
    while(fgets(liin, sizeof(liin), list_of_cums))
    {
        // printf("liin: %s\n", liin);
        fprintf(write, "%s", liin);
    }
    fclose(write);
    fclose(list_of_cums);
    remove("list_of_commits.txt");
    rename("temp.txt", "list_of_commits.txt");
    
    chdir(current);

}
/// //
void status()
{
    char addres[MAX_ADD];
    if_exist(addres);
    chdir(addres);
    // system("pwd");
    FILE* tracked = fopen("tracked.txt", "r");
    char tra_line[MAX_ADD];
    int id;
    FILE* file = fopen("current_commit.txt", "r");
    fscanf(file, "%d", &id);
    // printf("id:%d\n", id);
    // FILE* last_commit = fopen("current_commit.txt", "r");
    while(fgets(tra_line, sizeof(tra_line), tracked))
    {
        char name[MAX_ADD];
        char hash[MAX_ADD];
        sscanf(tra_line, "%s %s\n", name, hash);
        // printf("%s", name);
        printf("%s:\t", name);
        FILE* staged = fopen("staged_files.txt", "r");
        int flag = 1;
        char stage_line[MAX_ADD];
        while(fgets(stage_line, sizeof(stage_line), staged))
        {
            char name2[MAX_ADD];
            char hash2[MAX_ADD];
            sscanf(stage_line, "%s %s\n", name2, hash2);
            if(!strcmp(name, name2))
            {
                flag = 0;
                if(exist(name))
                {
                    get_hash(hash, name);
                    if(!strcmp(hash, hash2))
                    {
                        printf("+");
                    }
                    else printf("-");
                }
                else {printf("-");
                }
                break;
            }
            
        }
        if(flag)
            printf("-");
        // fclose(staged);
        flag = 1;
        char idd[100];
        sprintf(idd, "%d", id);
        // printf("Id: %s\n", idd);
        if(id > 0){
        char committxt[MAX_ADD];
        strcpy(committxt,addres);
        strcat(committxt, "/commits/");
        strcat(committxt, idd);
        strcat(committxt, "/commited.txt");
        // printf("%s\n", committxt);
        FILE* commited = fopen(committxt, "r");
        char commitline[MAX_ADD];
        while(fgets(commitline, sizeof(commitline), commited))
        {
            char name2[MAX_ADD];
            char hash2[MAX_ADD];
            sscanf(commitline, "%s %s\n", name2, hash2);
            if(!strcmp(name, name2))
            {
                flag = 0;
                if(exist(name))
                {
                    char hash3[MAX_HASH];
                    get_hash(hash3, name);
                    if(!strcmp(hash3, hash2))
                    {
                        printf("\n");
                    }
                    else printf("M\n");
                }
                else
                {
                    printf("D\n");
                }
                break;
            }
        }
        if(flag)
        {
            if(exist(name))
                printf("A");
            printf("\n");
        }
        }
        else{
            rewind(staged);
            // int flag = 1;
        char stage[MAX_ADD];
        while(fgets(stage, sizeof(stage), staged))
        {
            // printf("%s", stage);
            char name3[MAX_ADD];
            char hash3[MAX_ADD];
            sscanf(stage_line, "%s %s\n", name3, hash3);
            if(!strcmp(name, name3))
            {
                flag = 0;
                if(exist(name))
                {
                    get_hash(hash, name);
                    if(!strcmp(hash, hash3))
                    {
                        printf("\n");
                    }
                    else printf("M\n");
                }
                else {printf("D\n");
                }
                break;
            }
            
        }
        if(flag)
            printf("A\n");
        }
    }
}
void reset(char* name, char* folder)
{
    static int all = 0;
    char current[1024];
    getcwd(current, sizeof(current));
    char where[1024];
    if(if_exist(where))
    {
        strcat(where, "/staged_files.txt");
        for (int i = strlen(name) - 1; i >= 0; i--)
        {
            if(name[i] == '/')
            {
                for (int j = 0; j < i; j++)
                {
                    folder[j] = name[j];
                }
                folder[i] = '\0';
                strcpy(name, name + i + 1);
                if(chdir(folder) != 0)
                {
                    printf("wrong address!\n");
                    return;
                }
                break;

            }
        }
        DIR* dir = opendir(".");
        struct dirent* entry;
        while(entry = readdir(dir))
        {
            if(all == 0)
            {
                if(!strcmp(name, entry->d_name))
                {
                    if(entry->d_type == 4)
                    {
                        all = 1;
                        chdir(entry->d_name);
                        reset("", ".");
                        all = 0;
                        chdir("..");
                    }
                    else 
                    {
                        FILE* file = fopen(where, "r");
                        char line[MAX_ADD];
                        char hash[MAX_HASH];
                        char path[MAX_ADD];
                        realpath(name, path);
                        char namestage[MAX_ADD];
                        int i = 0;
                        while (fgets(line, sizeof(line), file))
                        {
                            sscanf(line, "%s %s\n", namestage, hash);
                            if(!strcmp(path, namestage))
                            {
                                remove_line(where, i);
                                
                            }
                            i++;
                        }
                        fclose(file);
                    }
                    chdir(current);
                    return;
                }
            }
            else
            {
                if(entry->d_type == 4)
                {
                    if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
                    {
                        all = 1;
                        chdir(entry->d_name);
                        reset("", ".");
                        chdir("..");
                    }
                }
                else 
                {
                    FILE* file = fopen(where, "r");
                    char line[MAX_ADD];
                    char hash[MAX_HASH];
                    char path[MAX_ADD];
                    realpath(entry->d_name, path);
                    char namestage[MAX_ADD];
                    int i = 0;
                    while (fgets(line, sizeof(line), file))
                    {
                        sscanf(line, "%s %s\n", namestage, hash);
                        if(!strcmp(path, namestage))
                        {
                            remove_line(where, i);
                            
                        }
                        i++;
                    }
                    fclose(file);
                }
            }
        }
    }
    chdir(current);
}
void undo()
{
    char addres[MAX_ADD];
    if_exist(addres);
    char staged[MAX_ADD];
    strcpy(staged, addres);
    strcat(addres, "/last_staged.txt");
    strcat(staged, "/staged_files.txt");
    FILE* last = fopen(addres, "r");
    char line[MAX_ADD];
    // int i = 0;
    // printf("lps");
    while(fgets(line, sizeof(line), last))
    {
        char path[MAX_ADD];
        char hash[MAX_HASH];
        sscanf(line, "%s %s\n", path, hash);
        FILE* st = fopen(staged, "r");
        char line2[MAX_ADD];
        int i = 0;
        while(fgets(line2, sizeof(line2), st))
        {
            char hs[MAX_HASH];
            char name[MAX_ADD];
            sscanf(line2, "%s %s\n", name, hs);
            // printf("name:%s path:%s\n", name, path);
            if(!strcmp(name, path))
            {
                // printf("%d", i);
                remove_line(staged, i);
                break;
            }
            i++;
        }
        fclose(st);
    }
    fclose(last);
    FILE* fuck = fopen(addres, "w");
}
void redo()
{
    char addres[MAX_ADD];
    char tra[MAX_ADD];
    if_exist(addres);
    strcpy(tra, addres);
    strcat(tra, "/tracked.txt");
    strcat(addres, "/staged_files.txt");
    // printf("%s\n", addres);
    FILE* st = fopen(addres, "r");
    char line[MAX_ADD];
            // printf("kos");
    int i = 0;
    while(fgets(line, sizeof(line), st))
    {
        char path[MAX_ADD];
        char hash[MAX_HASH];
        char real_hash[MAX_HASH];
        sscanf(line, "%s %s\n", path, hash);
        get_hash(real_hash, path);
        if(strcmp(hash, real_hash))
        {
            // fclose(st);
            remove_line(addres, i);
            // st = fopen(addres, "r");
            FILE* write = fopen(addres, "a");
            fprintf(write, "%s %s\n", path, real_hash);
            fclose(write);
        }
        i++;
    }
    fclose(st);
    FILE* trac = fopen(tra, "r");
    char line1[1024];
    int j = 0;
    while(fgets(line1, sizeof(line1), trac))
    {
        char path[MAX_ADD];
        char hash[MAX_HASH];
        char real_hash[MAX_HASH];
        sscanf(line1, "%s %s\n", path, hash);
        get_hash(real_hash, path);
        if(strcmp(hash, real_hash))
        {
            // fclose(st);
            remove_line(tra, j);
            // st = fopen(addres, "r");
            FILE* write = fopen(tra, "a");
            fprintf(write, "%s %s\n", path, real_hash);
            fclose(write);
        }
        j++;
    }
}
void remove_line(char* text_name, int khat)
{
    FILE* file = fopen(text_name, "r");
    char str[1024];
    // strcpy(str, text_name);
    // printf("text: %s\n", text_name);
    for (int i = strlen(text_name) - 1; i >= 0; i--)
    {
        if(text_name[i] == '/')
        {
            for (int j = 0; j <= i; j++)
            {
                str[j] = text_name[j];
            }
            
            
        break;
        }
    }
    strcat(str, "please_dont.txt");
    // printf("str: %s\n", str);
    FILE* copy = fopen(str, "w");
    int i = 0;
    char line[1024];

    while (fgets(line, sizeof(line), file))
    {
        // printf("%s", line);
        if(i == khat)
        {
            i++;
            continue;
        }
        fprintf(copy, "%s", line);
        i++;
    }
    fclose(file);
    fclose(copy);
    remove(text_name);
    rename(str, text_name);
}
void add_n()
{
    char where[1024];
    if(if_exist(where))
    {
        strcat(where, "/staged_files.txt");
        DIR* dir = opendir(".");
        struct dirent* entry;
        while(entry = readdir(dir))
        {
            if(entry->d_type != 4)
            {
                int flag = 1;
                printf("%s: ", entry->d_name);
                char path[1024], hash[1024];
                realpath(entry->d_name, path);
                FILE* file = fopen(where, "r");
                char line[1024], name[MAX_FILE_NAME], has[MAX_HASH];
                get_hash(hash, path);
                while(fgets(line, sizeof(line), file))
                {
                    sscanf(line, "%s %s\n", name, has);
                    if(!strcmp(has, hash), !strcmp(name, path))
                    {
                        flag = 0;
                        break;
                    }
                }
                if(flag)
                {
                    printf("un");
                }
                printf("staged\n");
            }
        }
        closedir(dir);
    }
    else existance_error;
    return;
}
void get_hash(char* hash, char *name)
{
    char command[100] = "md5sum ";
    system("touch hash.txt");
    strcat(command, name);
    strcat(command, " > hash.txt");
    system(command);
    char add[1024];
    FILE* file = fopen("hash.txt", "r");
    fscanf(file, "%s %s", hash, add);
    fclose(file);
    system("rm hash.txt");

}
void add(char* name, char* folder)
{
    static int all = 0;
    char current[1024];
    getcwd(current, sizeof(current));
    char where[1024];
    char traked[1024];
    char last[1024];
    if(if_exist(where))
    {
        strcpy(last, where);
        strcpy(traked, where);
        strcat(last, "/last_staged.txt");
        strcat(traked, "/tracked.txt");
        strcat(where, "/staged_files.txt");
        
        for (int i = strlen(name) - 1; i >= 0; i--)
        {
            if(name[i] == '/')
            {
                    // printf("folder: %s\nname: %s\n", folder, name);
                for (int j = 0; j < i; j++)
                {
                    folder[j] = name[j];
                }
                folder[i] = '\0';
                strcpy(name, name + i + 1);
                if(chdir(folder) != 0)
                {
                    // system("pwd");
                    printf("wrong address!\n");
                    return;
                }
                break;
            }
        }
        DIR* dir = opendir(".");
        struct dirent* entry;
        while(entry = readdir(dir))
        {
            if(all == 0)
            {
                if(!strcmp(entry->d_name, name))
                {
                    if(entry->d_type == 4)
                    {
                        all = 1;
                        chdir(entry->d_name);
                        add("", ".");
                        all = 0;
                        chdir("..");
                    }
                    else
                    {
                        int flag = 1;
                        char name2[MAX_ADD];
                        char hash[MAX_HASH];
                        realpath(entry->d_name, name2);
                        get_hash(hash, name2);
                        FILE* read = fopen(where, "r");
                        FILE* trak = fopen(traked, "r");
                        char line[2024];
                        int i = 0;
                        while(fgets(line, sizeof(line), read))
                        {
                            char path[1024];
                            char h[MAX_HASH];
                            sscanf(line, "%s %s\n", path, h);
                            if(!strcmp(path, name2) && !strcmp(hash, h))
                            {
                                flag = 0;
                    
                            }
                            else if(!strcmp(path, name2))
                            {
                                fclose(read);
                                remove_line(where, i);
                                break;
                                // read = fopen(where, "r");
                            }
                            i++;
                        }
                        i = 0;
                        char line1[1024];
                        while(fgets(line1, sizeof(line1), trak))
                        {
                            char path[1024];
                            char h[MAX_HASH];
                            sscanf(line1, "%s %s\n", path, h);
                            if(!strcmp(path, name2) && strcmp(hash, h) != 0)
                            {
                                // printf("kos");
                                fclose(trak);
                                remove_line(traked, i);
                                break;
                            }
                            
                            i++;
                        }
                        // fclose(read);
                        if(flag){
                         FILE* add = fopen(where, "a");
                        FILE* tracked = fopen(traked, "a");
                        fprintf(tracked, "%s %s\n", name2, hash);
                        fclose(tracked);
                        FILE* lstage = fopen(last, "a");
                        fprintf(lstage, "%s %s\n", name2, hash);
                        fclose(lstage);
                        fprintf(add, "%s %s\n", name2, hash);
                        fclose(add);
                        }
                    }
                    chdir(current);
                    return;
                }
            }
            else
            {
                if(entry->d_type == 4 && strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
                {
                    all = 1;
                    chdir(entry->d_name);
                    add("", ".");
                        chdir("..");
                }
                else if(entry->d_type != 4)
                {
                    int flag = 1;
                    char hash[MAX_HASH];
                    char addr[1024] = {0};
                    realpath(entry->d_name, addr);
                    get_hash(hash, addr);
                    FILE* read = fopen(where, "r");
                    FILE* trak = fopen(traked, "r");
                        char line[2024];
                        int i = 0;
                        while(fgets(line, sizeof(line), read))
                        {
                            char path[1024];
                            char h[MAX_HASH];
                            sscanf(line, "%s %s\n", path, h);
                            if(!strcmp(path, addr) && !strcmp(hash, h))
                            {
                                flag = 0;
                            }
                            else if(!strcmp(path, addr))
                            {
                                fclose(read);
                                remove_line(where, i);
                                break;
                                // read = fopen(where, "r");
                            }
                            i++;
                        }
                        i = 0;
                        while(fgets(line, sizeof(line), trak))
                        {
                            char path[1024];
                            char h[MAX_HASH];
                            sscanf(line, "%s %s\n", path, h);
                            if(!strcmp(path, addr) && strcmp(hash, h))
                            {
                                fclose(trak);
                                remove_line(traked, i);
                                break;
                                // read = fopen(where, "r");
                            }
                            i++;
                        }
                        // fclose(read);
                        if(flag){
                    FILE* add = fopen(where, "a");
                    
                    fprintf(add, "%s %s\n", addr, hash);
                    fclose(add);
                    FILE* tracked = fopen(traked, "a");
                        fprintf(tracked, "%s %s\n", addr, hash);
                        fclose(tracked);
                        FILE* lstage = fopen(last, "a");
                        fprintf(lstage, "%s %s\n", addr, hash);
                        fclose(lstage);
                        }
                }
            }
        }
    }
    if(all == 0)
    {
        printf("%s doesn't exist\n", name);
    }
    chdir(current);
    return;
}

void where_are_we(char* addres)
{
    system("touch pwd.txt");
    system("pwd > pwd.txt");
    FILE* pwd = fopen("pwd.txt", "r");
    fgets(addres, 1000, pwd);
    addres[strlen(addres) - 1] = '\0';
}
int if_exist(char* addres)
{
    char current[1024];
    getcwd(current, sizeof(current));
    while(1)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        DIR* dir = opendir(".");
        struct dirent* entry;
        if(strcmp(cwd, "/") == 0)
        {
            chdir(current);
            closedir(dir);
            return 0;
            break;
        }
        while(entry = readdir(dir))
        {
            if(!strcmp(entry->d_name, ".hapgit"))
            {
                closedir(dir);
                chdir(".hapgit");
                getcwd(addres, 1024);
                chdir(current);
                // printf("%s\n", addres);
                return 1;

            }
        }
        chdir("..");
    }
}
void git()
{
    char *current_place = (char*)calloc(1024, sizeof(char));
    getcwd(current_place, 1024);
    // where_are_we(current_place);
    char string[1024];
    if(if_exist(string))
    {
        printf("Repository already exists\n");
    }
    else
    { 
        chdir(current_place);
        mkdir(".hapgit", 0777);
        chdir(".hapgit");
        //directories:
        system("touch staged_files.txt");
        system("touch tracked.txt");
        system("touch last_staged.txt");
        mkdir("commits", 0777);
        char name_tt[MAX_ADD] = "/home/mahan/hapgit/name.txt";
        if(exist(name_tt))
        {
            FILE* nametxt = fopen(name_tt, "r");
            char globname[MAX_FILE_NAME];
            fscanf(nametxt, "%s", globname);
            FILE* locname = fopen("name.txt", "w");
            fprintf(locname, "%s", globname);
            fclose(locname);
        }
        char email_tt[MAX_ADD] = "/home/mahan/hapgit/email.txt";
        if(exist(email_tt))
        {
            FILE* emailtxt = fopen(email_tt, "r");
            char globemail[MAX_FILE_NAME];
            fscanf(emailtxt, "%s", globemail);
            FILE* locemail = fopen("email.txt", "w");
            fprintf(locemail, "%s", globemail);
            fclose(locemail);
        }
        FILE* com = fopen("last_commit.txt", "w");
        fprintf(com, "0");
        fclose(com);
        FILE* branch = fopen("branches.txt", "w");
        fprintf(branch, "master");
        FILE* c_brach = fopen("current_branch.txt", "w");
        fprintf(c_brach, "master");
        fclose(branch);
        fclose(c_brach);
        FILE* list_of_commits = fopen("list_of_commits.txt", "w");
        fclose(list_of_commits);
        FILE* current_cum = fopen("current_commit.txt", "w");
        fprintf(current_cum, "0");
        //
        char current[MAX_ADD];
        getcwd(current, sizeof(current));
        chdir("/home/mahan/hapgit");
        FILE* addresses = fopen("addres.txt", "a");
        fprintf(addresses, "%s/.hapgit\n", current_place);
        




        chdir(current);
        printf("Initialized successfully!\n");
    }        
}
void global_name(char* nameof)
{
    char* current_addres = (char*)malloc(100 * sizeof(char));
    where_are_we(current_addres);
    chdir("/home/mahan/hapgit");
    FILE* f = fopen("name.txt", "w");
    fprintf(f, "%s", nameof);
    fclose(f);
    char path[100];
    int how = 0;
    FILE* addresses = fopen("addres.txt", "r");
    while(fgets(path, 100, addresses))
    {
        path[strlen(path) - 1] = '\0';
        char command[MAX_COMMAND] = "cp name.txt ";
        strcat(command, path);
        system(command);

    }

}
void global_email(char* nameof)
{
    char* current_addres = (char*)malloc(100 * sizeof(char));
    where_are_we(current_addres);
    chdir("/home/mahan/hapgit");
    FILE* f = fopen("email.txt", "w");
    fprintf(f, "%s", nameof);
    fclose(f);
    char path[100];
    int how = 0;
    FILE* addresses = fopen("addres.txt", "r");
    while(fgets(path, 100, addresses))
    {
        path[strlen(path) - 1] = '\0';
        char command[MAX_COMMAND] = "cp email.txt ";
        strcat(command, path);
        system(command);
    }
}
void local_name(char* nameof)
{
    chdir(".hapgit");
    FILE* name = fopen("name.txt", "w");
    fprintf(name, "%s", nameof);
}
void local_email(char* nameof)
{
    chdir(".hapgit");
    FILE* email = fopen("email.txt", "w");
    fprintf(email, "%s", nameof);
}
int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("invalid command\n");
    }
    
    if(!strcmp(argv[1], "init"))
    {
        git();
    }
    else if(!strcmp(argv[1], "config"))
    {

        if(!strcmp(argv[2], "user.name"))
        {
            local_name(argv[3]);
        }
        else if(!strcmp(argv[2], "user.email"))
        {
            local_email(argv[3]);
        }
        else if(!strcmp(argv[2], "-global"))
        {
            if(!strcmp(argv[3], "user.name"))
            {
                global_name(argv[4]);
            }
            if(!strcmp(argv[3], "user.email"))
            {
                global_email(argv[4]);
            }
        }
    }
    else if(!strcmp(argv[1], "add"))
    {

        char folder[1024] = ".";
        char string[1024];
        if(if_exist(string)){
            if(strcmp(argv[2], "-n") && strcmp(argv[2], "-redo")){
        char ad[1024];
        strcpy(ad, string);
        strcat(ad, "/last_staged.txt");
        FILE* file = fopen(ad, "w");
        fclose(file);
            }
       if(!strcmp("-f", argv[2]))
        {
            for (int i = 3; i < argc; i++){
                add(argv[i], folder);
            }
            
        }
        else if(!strcmp("-n", argv[2]))
        {
            add_n();
        } 
        else if(!strcmp(argv[2], "-redo"))
        {
            redo();
        } 
        else {
        for (int i = 2; i < argc; i++)
        {
            add(argv[i], folder);
        }
        // system("pwd");
        
    }
        }
        else printf("Hapgit has not been initialized yet!\n");
    }
        
    else if(!strcmp(argv[1], "reset"))
    {
        char folder[1024] = ".";
        char string[50];
        if(if_exist(string)){
            if(!strcmp(argv[2], "-undo"))
            {
                undo();
            }
            else
        if(!strcmp(argv[2], "-f"))
        {
            for (int i = 3; i < argc; i++)
            {
                reset(argv[i], folder);                
            }
            
        }
        else 
        {
            for (int i = 2; i < argc; i++)
            {
                reset(argv[i], folder);
            }
            
        }
        }
        else existance_error;
    }
    else if(!strcmp(argv[1], "status"))
    {
        char string[MAX_ADD];
        if(if_exist(string))
        {
            status();
        }
        else existance_error;
    }
    else if(!strcmp(argv[1], "commit"))
    {
        char string[MAX_ADD];
        if(if_exist(string)){
        if(!strcmp(argv[2], "-m"))
        {
            if(argc == 4)
            {
                if(strlen(argv[3]) > 72)
                {
                    printf("Your commit message is too long!\n");
                }
                else commit(argv[3]);
            }
            else if(argc < 4)
            {
                printf("Enter a commit message");
            }
            else{
                    printf("Put your message between \"\"");
            }
        }
        else if(!strcmp(argv[2], "-s"))
        {
            chdir(string);
            char path[MAX_ADD];
            realpath(argv[3], path);
            if(exist(path))
            {
                FILE* shortc = fopen(path, "r");
                char mess[MAX_MES_NAME];
                // fscanf(shortc, "%s", mess);
                fgets(mess, sizeof(mess), shortc);
                if(mess[strlen(mess) - 1] == '\n')
                {
                    mess[strlen(mess) - 1] = 0;
                }
                commit(mess);
            }
            else printf("You have to set the shortcut first!\n"); 
        }
        }
        else existance_error;
    }
    else if(!strcmp(argv[1], "log"))
    {
        char string[MAX_ADD];
        if(if_exist(string))
        {
            chdir(string);
        FILE* file = fopen("list_of_commits.txt", "r");
        if(argc == 2)
        {
            char line[MAX_ADD];
            while(fgets(line, sizeof(line), file))
            {
                printf("%s", line);
            }
        }
        else if(strcmp(argv[2],"-n") == 0)
        {
            int i = 0;
            int number;
            sscanf(argv[3], "%d", &number);
            char line[MAX_ADD];
            while(fgets(line, sizeof(line), file))
            {
                if(i == number)
                    break;
                printf("%s", line);
                i++;
            }
        }
        else if(strcmp(argv[2], "-branch") == 0)
        {
            char line[MAX_ADD];
            int flag = 0;
            while(fgets(line, sizeof(line), file))
            {
                char mess[80], date[200], branch[100],  author[100];
                int id , number_of_commited;
                sscanf(line, "commit id: %d\tauthor: %s\tbranch: %s\tnumber of files commited: %d\tmessage: %s\ttime commited: %s", &id, author, branch, &number_of_commited, mess, date);
                if(!strcmp(argv[3], branch))
                {
                printf("%s", line);
                flag = 1;
                }
            }
            if(flag == 0)
            {
                printf("Branch doesn't exist\n");
            }
        }
        else if(!strcmp(argv[2], "-since"))
        {
            char line[MAX_ADD];
            char sice_time[1024];
            strcpy(sice_time, argv[3]);
            strcat(sice_time, " ");
            strcat(sice_time, argv[4]);
            
            while(fgets(line, sizeof(line), file))
            {
                int flag = 0;
                char mess[100], date[300], branch[100],  author[100];
                int id , number_of_commited;
                sscanf(line, "commit id: %d\tauthor: %s\tbranch: %s\tnumber of files commited: %d\tmessage: %s\ttime commited: %[^\n]s", &id, author, branch, &number_of_commited, mess, date);
                if(compare_times(date, sice_time) > 0)
                {
                    printf("%s", line);
                }

            }
        }
        else if(!strcmp(argv[2], "-before"))
        {
            char line[MAX_ADD];
            char sice_time[1024];
            strcpy(sice_time, argv[3]);
            strcat(sice_time, " ");
            strcat(sice_time, argv[4]);
            
            while(fgets(line, sizeof(line), file))
            {
                int flag = 0;
                char mess[100], date[300], branch[100],  author[100];
                int id , number_of_commited;
                sscanf(line, "commit id: %d\tauthor: %s\tbranch: %s\tnumber of files commited: %d\tmessage: %s\ttime commited: %[^\n]s", &id, author, branch, &number_of_commited, mess, date);
                if(compare_times(date, sice_time) < 0)
                {
                    printf("%s", line);
                }

            }
        }
        else if(!strcmp(argv[2], "-search"))
        {
            char line[MAX_ADD];
            while(fgets(line, sizeof(line), file))
            {
                int flag = 0;
                char mess[100], date[200], branch[100],  author[100];
                int id , number_of_commited;
                sscanf(line, "commit id: %d\tauthor: %s\tbranch: %s\tnumber of files commited: %d\tmessage: %[^\t]s\ttime commited: %s", &id, author, branch, &number_of_commited, mess, date);
                // printf("mess: %s\n", mess);
                for(int i = 3; i < argc; i++)
                {
                    // char word[200] = " ";
                    // strcat(word, argv[i]);
                    // strcat(word, "");
                    // char temp_word[200];
                    
                    if(strstr(mess, argv[i]))
                    {

                        printf("%s", line);
                        break;
                    }
                }
            }
        }
        else if(!strcmp(argv[2], "-author"))
        {
            char line[MAX_ADD];
            int flag = 0;
            while(fgets(line, sizeof(line), file))
            {
                char mess[80], date[200], branch[100],  author[100];
                int id , number_of_commited;
                sscanf(line, "commit id: %d\tauthor: %s\tbranch: %s\tnumber of files commited: %d\tmessage: %s\ttime commited: %s", &id, author, branch, &number_of_commited, mess, date);
                if(!strcmp(argv[3], author))
                {
                printf("%s", line);
                flag = 1;
                }
            }
        }
    }
        else existance_error;
    }
    else if(!strcmp(argv[1], "branch"))
    {
        char string[1024];
        if(if_exist(string))
        {
            char current[1024];
            getcwd(current, sizeof(current));
            chdir(string);
            FILE *file = fopen("branches.txt", "r");
            if(argc == 2){
            char line[1024];
            while (fgets(line, sizeof(line), file))
            {
                printf("%s", line);
            }
            

            }
            else
            {

            }
        
        }
        else existance_error;
    }
    else if(!strcmp(argv[1], "set"))
    {
        char string[MAX_ADD];
        if(if_exist(string))
        {
        if(argc < 6)
        {
            printf("Enter a shortcut!\n");
        }
        else {
            chdir(string);
            FILE* shortcut = fopen(argv[5], "w");
            fprintf(shortcut, "%s", argv[3]);
            fclose(shortcut);
        }
        }
        else existance_error;
    }
    else if(!strcmp(argv[1], "replace"))
    {
        char string[MAX_ADD];
        if(if_exist(string))
        {
        if(argc < 6)
        {
            printf("Enter a shortcut!\n");
        }
        else {
            chdir(string);
            char path[MAX_ADD];
            realpath(argv[5], path);
            if(exist(path)){
            FILE* shortcut = fopen(argv[5], "w");
            fprintf(shortcut, "%s", argv[3]);
            fclose(shortcut);
            }
            else printf("You have to set the shortcut first! 5:%s\n" ,argv[5]);
        }
        }
        else existance_error;
    }
    else if(!strcmp(argv[1], "remove"))
    {
        char string[MAX_ADD];
        if(if_exist(string))
        {
            chdir(string);
            remove(argv[3]);
        
        }
        else existance_error;
    }
}
