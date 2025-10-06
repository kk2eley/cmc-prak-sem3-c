#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

ssize_t getcwd2(int fd, char *buf, size_t size) {
    // 1. Сохраняем исходный CWD, чтобы вернуться в него в конце.
    int original_cwd_fd = open(".", O_RDONLY);
    if (original_cwd_fd == -1) {
        return -1;
    }

    // Буфер для сборки пути в обратном порядке.
    char path_rev[PATH_MAX];
    char *p = path_rev + sizeof(path_rev) - 1;
    *p = '\0';

    ssize_t total_len = 0;
    int error_occurred = 0;

    // 2. Переходим в целевой каталог.
    if (fchdir(fd) == -1) {
        error_occurred = 1;
        goto cleanup;
    }

    struct stat current_stat, parent_stat;
    // Получаем inode/dev_id стартового каталога
    if (stat(".", &current_stat) == -1) {
        error_occurred = 1;
        goto cleanup;
    }

    // 3. Главный цикл: поднимаемся вверх по дереву.
    while (1) {
        // Проверяем, не достигли ли мы корня.
        // В корне "." и ".." указывают на один и тот же inode.
        if (stat("..", &parent_stat) == -1) {
            error_occurred = 1;
            break;
        }
        if (current_stat.st_ino == parent_stat.st_ino && current_stat.st_dev == parent_stat.st_dev) {
            break; // Мы в корне, выходим из цикла.
        }

        // Переходим в родительский каталог.
        if (chdir("..") == -1) {
            error_occurred = 1;
            break;
        }

        DIR *parent_dir = opendir(".");
        if (!parent_dir) {
            error_occurred = 1;
            break;
        }

        struct dirent *entry;
        int found_entry = 0;
        // Ищем в родительском каталоге запись с нашим Inode.
        while ((entry = readdir(parent_dir)) != NULL) {
            struct stat entry_stat;
            // lstat, чтобы не следовать по символическим ссылкам.
            if (lstat(entry->d_name, &entry_stat) == -1) {
                continue;
            }

            if (entry_stat.st_ino == current_stat.st_ino && entry_stat.st_dev == parent_stat.st_dev) {
                size_t name_len = strlen(entry->d_name);
                // Проверяем, хватит ли места в нашем временном буфере.
                if ((p - path_rev) < (ssize_t)(name_len + 1)) {
                    error_occurred = 1; // Путь слишком длинный
                    break;
                }
                
                // Копируем имя компонента и слэш перед ним.
                p -= name_len;
                memcpy(p, entry->d_name, name_len);
                *--p = '/';
                
                total_len += name_len + 1;
                found_entry = 1;
                break;
            }
        }
        closedir(parent_dir);

        if (!found_entry || error_occurred) {
            if (!error_occurred) errno = ENOENT; // Устанавливаем ошибку, если запись не найдена
            error_occurred = 1;
            break;
        }

        // Обновляем inode для следущей итерации - теперь "текущим" стал родительский.
        if (stat(".", &current_stat) == -1) {
            error_occurred = 1;
            break;
        }
    }

    // 4. Финальная сборка и копирование результата.
    if (!error_occurred) {
        if (total_len == 0) { // Если цикл ни разу не сработал - мы в корне.
            if ((p - path_rev) < 1) {
                error_occurred = 1;
            } else {
                *--p = '/';
                total_len = 1;
            }
        }
        
        if (!error_occurred && size > 0) {
            size_t copy_len = (total_len < size) ? total_len : (size - 1);
            memcpy(buf, p, copy_len);
            buf[copy_len] = '\0';
        }
    }

cleanup:
    // 5. Восстанавливаем исходный CWD.
    // Эта часть выполняется всегда, и при успехе, и при ошибке.
    fchdir(original_cwd_fd);
    close(original_cwd_fd);

    return error_occurred ? -1 : total_len;
}

// ------ Тестовая обвязка для проверки ------
int main(int argc, char *argv[]) {
    char path_buffer[PATH_MAX];
    const char *test_path = "."; // По умолчанию тестируем на текущем каталоге

    // Позволяем задать каталог для теста через аргумент командной строки
    if (argc > 1) {
        test_path = argv[1];
    }
    
    printf("Testing getcwd2 on directory: '%s'\n", test_path);

    int fd = open(test_path, O_RDONLY | O_DIRECTORY);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    // Вызываем нашу функцию
    ssize_t len = getcwd2(fd, path_buffer, sizeof(path_buffer));

    // Анализируем результат
    if (len != -1) {
        printf("Success!\n");
        printf("Returned length: %zd\n", len);
        printf("Recovered path: %s\n", path_buffer);
        
        // Дополнительная проверка, если size=0
        ssize_t len_only = getcwd2(fd, NULL, 0);
        printf("Length-only check returned: %zd\n", len_only);

    } else {
        perror("getcwd2 failed");
    }

    close(fd);
    return 0;
}

