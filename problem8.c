#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DB_ADDR "localhost"
#define DB_ID "root"
#define DB_PWD "1234"
#define BUF_SIZE 256

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

void InsertName(char *query, char *name){
    memset(query, 0, BUF_SIZE);
    strcpy(query,"INSERT INTO name_book VALUES (default, ");
    strcat(query, "'");
    strcat(query, name);
    strcat(query, "'");
    strcat(query, ");");
}

void *ExecuteQuery(MYSQL *mysql, const char *query){
    int status;
    status = mysql_query(mysql, query);
    if(status != 0){
        fprintf(stderr, "Query Error\n");
        return NULL;
    }
    return 0;
}

int main(){
    MYSQL mysql;

    char query[BUF_SIZE] = "";
    char name[20];
    ConnectDB(&mysql);
    printf("input your name : ");
    scanf("%s", name);
    InsertName(query, name);
    ExecuteQuery(&mysql, query);
    CloseDB(&mysql);
    return 0;
}