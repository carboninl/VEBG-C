#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

List *create_list(){
    List *list = malloc(sizeof(List));
    if (!list) return NULL;
    list->count = 0;
    list->capacity = 4; // arbitrary min capacity of 4
    list->items = malloc(sizeof(Item) * list->capacity);
    if (!list->items){ // if malloc failed, free list
        free(list);
        return NULL;
    }
    return list;
}

void add_item(List *list, Item item){
    if (list->count == list->capacity){
        list->capacity *= 2;
        list->items = realloc(list->items, sizeof(Item) * list->capacity);
        
    }
    list->items[list->count++] = item;
}

void add_string(List* list, const char *text){
    Item item;
    item.type = ITEM_STRING;
    item.value.string = strdup(text);

    add_item(list, item);
}

void add_list(List* list, List* other){
    Item item;
    item.type = ITEM_LIST;
    item.value.list = other;

    add_item(list, item);
}

static void print_indent(int indent){
    for (int i = 0; i < indent; i++){
        printf("  ");
    }
}

void print_list(List *list, int indent){
    print_indent(indent);
    printf("[\n");
    for(int i = 0; i < list->count; i++){
        if (list->items[i].type == ITEM_STRING){
            print_indent(indent + 1);
            printf("%s\n", list->items[i].value.string);
        } else if (list->items[i].type == ITEM_LIST){
            print_indent(indent);
            print_list(list->items[i].value.list, indent + 1);
        }
    }
    print_indent(indent);
    printf("]\n");
}

void free_list(List *list) {
    if (!list) return;

    for (int i = 0; i < list->count; i++) {
        if (list->items[i].type == ITEM_STRING) {
            free(list->items[i].value.string);
        } else if (list->items[i].type == ITEM_LIST) {
            free_list(list->items[i].value.list);
        }
    }

    free(list->items);
    free(list);
}

