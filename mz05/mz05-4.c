#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Структура для хранения информации о каждом валидном пути
typedef struct {
    const char *path; // Сам путь (строка из argv)
    dev_t dev_id;     // ID устройства
    ino_t inode;      // Номер inode
} FileInfo;

// Функция для qsort: группирует по файлам, а внутри группы сортирует по убыванию пути
int compare_for_grouping(const void *a, const void *b) {
    const FileInfo *file_a = (const FileInfo *)a;
    const FileInfo *file_b = (const FileInfo *)b;

    // Сначала сравниваем по ID устройства
    if (file_a->dev_id < file_b->dev_id) return -1;
    if (file_a->dev_id > file_b->dev_id) return 1;

    // Если устройства одинаковые, сравниваем по inode
    if (file_a->inode < file_b->inode) return -1;
    if (file_a->inode > file_b->inode) return 1;

    // Если это один и тот же файл, сортируем пути лексикографически по убыванию
    // чтобы "максимальный" путь был первым в группе.
    return strcmp(file_b->path, file_a->path);
}

// Стандартная функция для qsort: сортирует строки по возрастанию
int compare_for_final_sort(const void *a, const void *b) {
    const char **str_a = (const char **)a;
    const char **str_b = (const char **)b;
    return strcmp(*str_a, *str_b);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        return 0; // Нет путей для обработки
    }

    // Шаг 1: Сбор информации
    FileInfo *files = malloc((argc - 1) * sizeof(FileInfo));
    if (!files) {
        perror("malloc for files failed");
        return 1;
    }
    
    int valid_files_count = 0;
    for (int i = 1; i < argc; ++i) {
        struct stat st;
        // Используем lstat, чтобы не "проваливаться" в символические ссылки сразу,
        // но stat тоже будет работать, так как задача - найти конечный файл.
        // stat проще и надежнее для определения конечного inode.
        if (stat(argv[i], &st) == 0) {
            files[valid_files_count].path = argv[i];
            files[valid_files_count].dev_id = st.st_dev;
            files[valid_files_count].inode = st.st_ino;
            valid_files_count++;
        }
        // Если stat не удался, просто игнорируем путь
    }

    if (valid_files_count == 0) {
        free(files);
        return 0; // Нет валидных файлов
    }

    // Шаг 2: Первичная сортировка для группировки
    qsort(files, valid_files_count, sizeof(FileInfo), compare_for_grouping);

    // Шаг 3: Фильтрация и выборка лексикографически максимальных путей
    const char **final_paths = malloc(valid_files_count * sizeof(char *));
    if (!final_paths) {
        perror("malloc for final_paths failed");
        free(files);
        return 1;
    }
    
    int final_count = 0;
    if (valid_files_count > 0) {
        // Всегда берем первый элемент, он уникален и максимален для своей группы
        final_paths[final_count++] = files[0].path;
        
        // Проходим по остальным и берем только первые элементы из новых групп
        for (int i = 1; i < valid_files_count; ++i) {
            if (files[i].dev_id != files[i - 1].dev_id || files[i].inode != files[i - 1].inode) {
                final_paths[final_count++] = files[i].path;
            }
        }
    }

    // Шаг 4: Финальная сортировка по возрастанию
    qsort(final_paths, final_count, sizeof(char *), compare_for_final_sort);

    // Шаг 5: Вывод
    for (int i = 0; i < final_count; ++i) {
        printf("%s\n", final_paths[i]);
    }

    // Очистка
    free(files);
    free(final_paths);

    return 0;
}

