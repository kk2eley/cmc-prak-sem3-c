#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h> // Для sprintf

// Размер буфера, как указано в задании (4 КБ)
#define BUFFER_SIZE 4096

int copy_file(const char *srcpath, const char *dstpath) {
    struct stat src_st, dst_st;
    char *final_dst_path = NULL;
    int src_fd = -1;
    int dst_fd = -1;
    int result = -1; // По умолчанию - ошибка

    // 1. Проверка исходного файла
    if (stat(srcpath, &src_st) == -1) {
        // Исходный файл не существует или недоступен
        return -1;
    }

    if (!S_ISREG(src_st.st_mode)) {
        // Исходный путь не является обычным файлом (например, это каталог)
        return -1;
    }

    // 2. Определение конечного пути назначения
    int dst_exists = (stat(dstpath, &dst_st) == 0);

    if (dst_exists && S_ISDIR(dst_st.st_mode)) {
        // Случай А: dstpath - это каталог. Формируем новый путь.
        const char *src_basename = strrchr(srcpath, '/');
        if (src_basename == NULL) {
            src_basename = srcpath; // В пути нет '/', имя файла - это весь путь
        } else {
            src_basename++; // Пропускаем сам символ '/'
        }

        // Выделяем память под новый путь: "dstpath/basename\0"
        final_dst_path = malloc(strlen(dstpath) + strlen(src_basename) + 2);
        if (final_dst_path == NULL) {
            return -1; // Ошибка выделения памяти
        }
        sprintf(final_dst_path, "%s/%s", dstpath, src_basename);
    } else {
        // Случай Б: dstpath - не каталог или не существует. Используем как есть.
        final_dst_path = strdup(dstpath);
        if (final_dst_path == NULL) {
            return -1; // Ошибка выделения памяти
        }
    }

    // 3. Проверка на самокопирование
    // Перепроверяем stat для конечного пути, так как он мог измениться
    if (stat(final_dst_path, &dst_st) == 0) {
        if (src_st.st_ino == dst_st.st_ino && src_st.st_dev == dst_st.st_dev) {
            // Это один и тот же файл. Успешный выход без действий.
            result = 0;
            goto cleanup;
        }
    }

    // 4. Открытие файлов
    src_fd = open(srcpath, O_RDONLY);
    if (src_fd == -1) {
        goto cleanup;
    }

    // Открываем конечный файл с правами исходного
    dst_fd = open(final_dst_path, O_WRONLY | O_CREAT | O_TRUNC, src_st.st_mode);
    if (dst_fd == -1) {
        goto cleanup;
    }

    // 5. Цикл копирования
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        ssize_t bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            // Ошибка записи (например, закончилось место на диске)
            goto cleanup;
        }
    }

    if (bytes_read == -1) {
        // Ошибка чтения
        goto cleanup;
    }

    // Если мы дошли сюда, копирование прошло успешно
    result = 0;

cleanup:
    // 6. Освобождение ресурсов
    if (src_fd != -1) {
        close(src_fd);
    }
    if (dst_fd != -1) {
        close(dst_fd);
    }
    if (final_dst_path != NULL) {
        free(final_dst_path);
    }

    return result;
}

