#ifndef LIST_H
#define LIST_H

typedef enum {
    ITEM_STRING,
    ITEM_LIST
} ItemType;

typedef struct List List;

typedef struct {
    ItemType type;
    union {
        char *string;
        List *list;
    } value;
} Item;

struct List {
    Item *items;
    int count;
    int capacity;
};

List *create_list(void);
void add_item(List *list, Item item);
void add_string(List *list, const char *text);
void add_list(List *list, List *other);
void print_list(List *list, int indent);
void free_list(List *list);

#endif