#include <string.h>
#include <stdlib.h>

// Объявление функции, как требуется в задаче
char *relativize_path(const char *path1, const char *path2);

char *relativize_path(const char *path1, const char *path2) {
    // 1. Получаем директорию первого пути (dir1)
    char *dir1_dup = strdup(path1);
    if (!dir1_dup) {
        return NULL; // Ошибка выделения памяти
    }
    
    char *last_slash = strrchr(dir1_dup, '/');
    if (last_slash) {
        if (last_slash == dir1_dup) { 
            // Это корень, например, путь "/a". Директория - "/"
            *(last_slash + 1) = '\0';
        } else {
            // Обычный случай, например, "/a/b". Директория - "/a"
            *last_slash = '\0';
        }
    }
    // Если path1 был "/", dir1_dup уже является "/" и ничего менять не надо.
    const char *dir1 = dir1_dup;

    // 2. Находим общий базовый каталог, сравнивая компоненты
    const char *p1 = dir1;
    const char *p2 = path2;
    const char *common_p1 = dir1;
    const char *common_p2 = path2;

    while (1) {
        // Указатели на следующий '/' в каждом из путей
        const char *next_slash1 = strchr(p1 + 1, '/');
        const char *next_slash2 = strchr(p2 + 1, '/');

        // Определяем длину текущего компонента пути
        size_t len1 = next_slash1 ? (size_t)(next_slash1 - p1) : strlen(p1);
        size_t len2 = next_slash2 ? (size_t)(next_slash2 - p2) : strlen(p2);

        // Сравниваем компоненты
        if (len1 != len2 || strncmp(p1, p2, len1) != 0) {
            break; // Нашли первое различие, останавливаемся
        }

        // Компоненты совпали, обновляем указатели на конец общего пути
        common_p1 = p1 + len1;
        common_p2 = p2 + len2;

        // Если хотя бы один из путей закончился, поиск общего предка завершен
        if (!next_slash1 || !next_slash2) {
            break;
        }

        // Переходим к следующим компонентам
        p1 = next_slash1;
        p2 = next_slash2;
    }

    // 3. Считаем, сколько раз нужно подняться вверх ("../")
    int up_count = 0;
    const char *rem_dir1 = common_p1;
    // Пропускаем возможный `/` в начале остатка
    if (*rem_dir1 == '/') {
        rem_dir1++;
    }
    if (*rem_dir1) { // Если в остатке dir1 есть хоть что-то
        up_count = 1;
        const char *p = rem_dir1;
        while ((p = strchr(p, '/'))) {
            p++;
            if (*p) { // Убедимся, что это не просто конечный слэш
                up_count++;
            }
        }
    }
    
    // 4. Определяем путь "вниз"
    const char *down_path = common_p2;
    if (*down_path == '/') {
        down_path++;
    }

    // 5. Особый случай: dir1 и path2 - это один и тот же каталог
    if (up_count == 0 && *down_path == '\0') {
        free(dir1_dup);
        return strdup(".");
    }

    // 6. Вычисляем итоговую длину и выделяем память
    size_t up_len = up_count * 3; // Для каждого "../"
    size_t down_len = strlen(down_path);
    
    size_t total_len = up_len + down_len + 1; // +1 для '\0'
    char *result = malloc(total_len);
    if (!result) {
        free(dir1_dup);
        return NULL;
    }

    // 7. Собираем итоговую строку
    char *current = result;
    for (int i = 0; i < up_count; ++i) {
        memcpy(current, "../", 3);
        current += 3;
    }

    if (down_len > 0) {
        memcpy(current, down_path, down_len);
        current += down_len;
    }
    *current = '\0';

    free(dir1_dup); // Освобождаем память, выделенную strdup
    return result;
}

/*
// Пример использования функции main для проверки
#include <stdio.h>

int main() {
    // Пример из задачи
    char* r1 = relativize_path("/a/b/c/d", "/a/e/f");
    printf("Test 1: /a/b/c/d -> /a/e/f  ==> %s\n", r1); // Ожидаем ../../e/f
    free(r1);

    // Случай, когда path2 является потомком
    char* r2 = relativize_path("/a/b", "/a/b/c/d");
    printf("Test 2: /a/b -> /a/b/c/d    ==> %s\n", r2); // Ожидаем c/d
    free(r2);

    // Случай, когда dir1 является потомком
    char* r3 = relativize_path("/a/b/c/d", "/a/b");
    printf("Test 3: /a/b/c/d -> /a/b      ==> %s\n", r3); // Ожидаем ../..
    free(r3);

    // Случай идентичных путей
    char* r4 = relativize_path("/a/b/c", "/a/b/c");
     printf("Test 4: /a/b/c -> /a/b/c      ==> %s\n", r4); // Ожидаем .
    free(r4);
    
    // Пути расходятся от корня
    char* r5 = relativize_path("/a", "/b");
    printf("Test 5: /a -> /b              ==> %s\n", r5); // Ожидаем ../b
    free(r5);
    
    // Работа с корнем
    char* r6 = relativize_path("/", "/a/b");
    printf("Test 6: / -> /a/b             ==> %s\n", r6); // Ожидаем a/b
    free(r6);

    return 0;
}
*/

