//
// Created by Quintas on 9/7/2023.
//

#ifndef SQLXPARSER_SQLXPARSER_H
#define SQLXPARSER_SQLXPARSER_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

//
// Definitions
//

struct _SQLXOption {
    char *key;
    char *value;
};

typedef struct _SQLXOption SQLXOption;

void SQLXOption_free(SQLXOption *option);

struct _SQLXOptionList {
    int heap_size;
    int size;
    SQLXOption *data;
};
typedef struct _SQLXOptionList SQLXOptionList;

void SQLXOptionList_init(SQLXOptionList *list);

void SQLXOptionList_add(SQLXOptionList *list, SQLXOption *option);

struct _SQLXQuery {
    char *operation;
    char *target;
    char *selector;
    char *database;
    char *table;
    SQLXOptionList options;
};
typedef struct _SQLXQuery SQLXQuery;

SQLXQuery *SQLXQuery_new(SQLXQuery *query);

void SQLXQuery_free(SQLXQuery *query);

int SQLXQuery_parser(SQLXQuery *query, const char *text);

//
// Implementation
//

void SQLXOption_free(SQLXOption *option) {
    if (option->key)
        free(option->key);
    if (option->value)
        free(option->value);
    free(option);
}

void SQLXOptionList_init(SQLXOptionList *list) {
    list->heap_size = 1;
    list->size = 0;
    list->data = (SQLXOption *) malloc(sizeof(SQLXOption) * list->heap_size);
}

void SQLXOptionList_add(SQLXOptionList *list, SQLXOption *option) {
    while (list->size >= list->heap_size) {
        list->heap_size *= 2;
        list->data = (SQLXOption *) realloc(list->data, sizeof(SQLXOption) * list->heap_size);
    }

    list->data[list->size++] = *option;
}


SQLXQuery *SQLXQuery_new(SQLXQuery *query) {
    query = (SQLXQuery *) malloc(sizeof(SQLXQuery));
    query->operation = NULL;
    query->target = NULL;
    query->selector = NULL;
    query->database = NULL;
    query->table = NULL;
    SQLXOptionList_init(&query->options);
    return query;
}

void SQLXQuery_free(SQLXQuery *query) {
    if (query->operation)
        free(query->operation);
    if (query->target)
        free(query->target);
    if (query->selector)
        free(query->selector);
    if (query->database)
        free(query->database);
    if (query->table)
        free(query->table);
    for (int i = 0; i < query->options.size; i++) {
        SQLXOption_free(&query->options.data[i]);
        free(&query->options.data[i]);
    }
    free(query);
}

int SQLXQuery_parser(SQLXQuery *query, const char *text) {
    if (!text) {
        fprintf(stderr, "SQLXQuery_parse: text is NULL\n");
        return FALSE;
    }

    char lex[256];
    int lexIndex = 0;
    int i = 0;

    if (text[0] == ' ') {
        fprintf(stderr, "SQLXQuery_parse: text cannot start with a space\n");
        return FALSE;
    }

    // Operation
    while (text[i] != ' ' && text[i] != '\0') {
        lex[lexIndex++] = text[i++];
    }
    lex[lexIndex] = '\0';
    query->operation = strdup(lex);

    lexIndex = 0;
    i++;

    // Target
    while (text[i] != ' ' && text[i] != '\0') {
        lex[lexIndex++] = text[i++];
    }
    lex[lexIndex] = '\0';
    query->target = strdup(lex);

    lexIndex = 0;
    i++;

    // Selector
    while (text[i] != ' ' && text[i] != '\0') {
        lex[lexIndex++] = text[i++];
    }
    lex[lexIndex] = '\0';
    query->selector = strdup(lex);

    lexIndex = 0;
    i++;

    // Database and table
    while (text[i] != ' ' && text[i] != '\0') {
        lex[lexIndex++] = text[i++];
    }
    lex[lexIndex] = '\0';

    char *dot = strchr(lex, '.');
    if (dot) {
        *dot = '\0';
        query->database = strdup(lex);
        query->table = strdup(dot + 1);
    } else {
        query->database = strdup(lex);
        query->table = NULL;
    }

    lexIndex = 0;
    i++;

    // Is Option
    int isOption = FALSE;

    while (text[i] != ' ' && text[i] != '\0') {
        lex[lexIndex++] = text[i++];
    }
    lex[lexIndex] = '\0';

    if (strcmp(lex, "WHERE") == 0 || strcmp(lex, "WITH") == 0) {
        isOption = TRUE;
    } else return TRUE;

    lexIndex = 0;
    i++;

    // Options
    while (text[i] != '\0') {
        while (text[i] != ' ' && text[i] != '\0') {
            lex[lexIndex++] = text[i++];
        }
        lex[lexIndex] = '\0';

        if (lex[0] == ',') {
            lexIndex = 0;
            i++;
            continue;
        }

        char *equal = strchr(lex, '=');

        if (equal) {
            *equal = '\0';
            SQLXOption *option = (SQLXOption *) malloc(sizeof(SQLXOption));
            option->key = strdup(lex);
            option->value = strdup(equal + 1);
            SQLXOptionList_add(&query->options, option);
        }

        lexIndex = 0;
        i++;
    }

    return TRUE;
}

#endif //SQLXPARSER_SQLXPARSER_H
