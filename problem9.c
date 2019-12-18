#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DB_ADDR "localhost"
#define DB_ID "root"
#define DB_PWD "1234"
#define BUF_SIZE 256

void ConnectDB(MYSQL *mysql);
void CloseDB(MYSQL *mysql);
void InsertData(MYSQL *mysql, char *query);
void SearchData(MYSQL *mysql, char *query);
void ExecuteQuery(MYSQL *mysql, char *query);
void BuildQuery(char *query, char *type, char *arg);

void ConnectDB(MYSQL *mysql){
    mysql_init(mysql);
    if(!mysql_real_connect(mysql, DB_ADDR, DB_ID, DB_PWD, "phone_book", 0, (char *)NULL, 0)){
        fprintf(stderr, "MySql Connection Error\n");
        exit(1);
    }
}

void CloseDB(MYSQL *mysql){
    if(mysql != NULL){
        mysql_close(mysql);
    }
}

void BuildQuery(char *query, char *type, char *arg){
    memset(query, 0, BUF_SIZE);
    if(!strcmp(type, "insert")){
        char *ptr = strtok(arg, "/");
        sprintf(query, "%s (%s, '%s',", "INSERT INTO phone_book VALUES ", "default", ptr);
        ptr = strtok(NULL, "/");
        strcat(query, "'");
        strcat(query, ptr);
        strcat(query, "',");
        ptr = strtok(NULL, "/");
        strcat(query, "'");
        strcat(query, ptr);
        strcat(query, "')");
    }
    else if(!strcmp(type, "select")){
        sprintf(query, "%s", "SELECT * FROM phone_book");
    }
    else{
        fprintf(stderr, "invalid query type\n");
    }
}

void InsertData(MYSQL *mysql, char *query){
    ExecuteQuery(mysql, query);
}

void SearchData(MYSQL *mysql, char *query){
    MYSQL_RES *res;
    MYSQL_ROW row;
    int fields;
    int results;
    ExecuteQuery(mysql, query);
    res = mysql_store_result(mysql);
    results = mysql_num_rows(res);
    fields = mysql_num_fields(res);
    for(int i = 0; i < results; i++){
        row = mysql_fetch_row(res);
        for(int i = 0; i < fields; i++){
            printf("[ %s ]", row[i]);
        }
        printf("\n");
    }
}

void ExecuteQuery(MYSQL *mysql, char *query){
    if(mysql_query(mysql, query) != 0){
        fprintf(stderr, "Query failed\n");
    }
}

int main(){
    char *query = NULL;
    char name[BUF_SIZE];
    char phone[BUF_SIZE];
    char address[BUF_SIZE];
    char data[BUF_SIZE * 3];
    MYSQL mysql;
    int select_menu;
    query = (char *)malloc(sizeof(char)*BUF_SIZE);
    ConnectDB(&mysql);

    while(1){
        printf("ADD DATA : 1, SEARCH DATA : 2 EXIT : 3 >> ");
        scanf("%d", &select_menu);
        if(select_menu == 1){
            printf("input : <name> <phone_number> <address>\n");
            printf(">> ");
            scanf(("%s %s %s"), name, phone, address);
            sprintf(data, "%s/%s/%s", name, phone, address);
            BuildQuery(query, "insert", data);
            InsertData(&mysql, query);
            continue;
        }

        if(select_menu == 2){
            BuildQuery(query, "select", NULL);
            SearchData(&mysql, query);
            continue;
        }

        if(select_menu == 3){
            break;
        }
        else{
            fprintf(stderr, "invalid menu number input 1 or 2 or 3\n");
        }
    }
    CloseDB(&mysql);
    return 0;
}