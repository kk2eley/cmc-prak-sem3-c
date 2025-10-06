#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>

#define MAX_DEPTH 4

// Рекурсивная функция для обхода каталогов
void traverse(const char *base_path, const char *relative_path, int depth, long long max_size) {
    if (depth > MAX_DEPTH) {
        return;
    }

    DIR *d = opendir(base_path);
    if (!d) {
        // Не смогли открыть, возможно, это не каталог или нет прав.
        // По-хорошему, можно бы и perror, но в рекурсии это замусорит вывод.
        // Просто выходим.
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        // Игнорируем "." и ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Формируем полный и относительный пути для текущего элемента
        char new_base_path[PATH_MAX];
        char new_relative_path[PATH_MAX];

        snprintf(new_base_path, sizeof(new_base_path), "%s/%s", base_path, entry->d_name);
        
        // Относительный путь формируем аккуратно: без начального слэша
        if (strlen(relative_path) == 0) {
            snprintf(new_relative_path, sizeof(new_relative_path), "%s", entry->d_name);
        } else {
            snprintf(new_relative_path, sizeof(new_relative_path), "%s/%s", relative_path, entry->d_name);
        }

        struct stat st;
        // Используем lstat, чтобы не следовать по симлинкам и игнорировать их
        if (lstat(new_base_path, &st) == -1) {
            continue; // Не удалось получить информацию, пропускаем
        }

        if (S_ISDIR(st.st_mode)) {
            // Если это каталог, уходим в рекурсию
            traverse(new_base_path, new_relative_path, depth + 1, max_size);
        } else if (S_ISREG(st.st_mode)) {
            // Если это регулярный файл, проверяем остальные условия
            if (st.st_size <= max_size && access(new_base_path, R_OK) == 0) {
                printf("%s\n", new_relative_path);
            }
        }
        // Символические ссылки и другие типы файлов просто игнорируются
    }

    closedir(d);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <directory> <max_size>\n", argv[0]);
        return 1;
    }

    const char *dir_path = argv[1];
    char *endptr;
    long long max_size = strtoll(argv[2], &endptr, 10);

    // Проверяем, что второй аргумент - корректное число
    if (errno != 0 || *endptr != '\0' || max_size < 0) {
        fprintf(stderr, "Error: Invalid max_size value\n");
        return 1;
    }

    traverse(dir_path, "", 1, max_size);

    return 0;
}

