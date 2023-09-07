#include <stdio.h>
#include "SQLXParser.h"

int main() {
    char *text = "CREATE BUCKET IN encom WITH name=test";

    SQLXQuery *query = SQLXQuery_new(NULL);

    if (SQLXQuery_parser(query, text)) {

        printf("Operation: %s\n", query->operation);
        printf("Target: %s\n", query->target);
        printf("Selector: %s\n", query->selector);
        printf("Database: %s\n", query->database);
        printf("Table: %s\n", query->table);

        for (int i = 0; query->options.size; i++) {
            printf("Option key %d: %s\n", i, query->options.data[i].key);
            printf("Option value %d: %s\n", i, query->options.data[i].value);
        }

        SQLXQuery_free(query);
    }

    return 0;
}
