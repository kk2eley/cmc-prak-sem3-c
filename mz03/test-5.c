#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "mz03-5.h"

// Создает новый узел. Для тестов копирует строку.
struct Elem* create_node(const char* str) {
    struct Elem* node = (struct Elem*)malloc(sizeof(struct Elem));
    if (!node) return NULL;
    node->str = strdup(str); // strdup = malloc + strcpy
    if (!node->str) {
        free(node);
        return NULL;
    }
    node->next = NULL;
    return node;
}

// Печатает список
void print_list(struct Elem* head) {
    if (!head) {
        printf("List is empty.\n");
        return;
    }
    struct Elem* current = head;
    while(current) {
        printf("[\"%s\"] -> ", current->str);
        current = current->next;
    }
    printf("NULL\n");
}

// Освобождает всю память, выделенную под список
void free_list(struct Elem* head) {
    struct Elem* current = head;
    while(current != NULL) {
        struct Elem* next = current->next;
        free(current->str); // Освобождаем строку
        free(current);      // Освобождаем узел
        current = next;
    }
}

int main() {
    // 1. Создаем тестовый список
    struct Elem *head = create_node("10");
    head->next = create_node("hello");
    head->next->next = create_node("-5");
    head->next->next->next = create_node("99");
    head->next->next->next->next = create_node(""); // пустая строка
    head->next->next->next->next->next = create_node("world");
    head->next->next->next->next->next->next = create_node("123not_a_number");
    
    printf("Original list:\n");
    print_list(head);

    // 2. Вызываем тестируемую функцию
    head = dup_elem(head);

    // 3. Печатаем результат
    printf("\nModified list:\n");
    print_list(head);
    
    // 4. Очищаем память
    free_list(head);

    return 0;
}

