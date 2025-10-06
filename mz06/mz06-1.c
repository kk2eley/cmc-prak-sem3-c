#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

// Для PATH_MAX, хотя можно было бы и своей константой обойтись
#include <limits.h>

int main(int argc, char *argv[]) {
    // Проверяем, что нам передали ровно один аргумент
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    const char *dir_path = argv[1];
    DIR *d;

    // Пытаемся открыть каталог
    d = opendir(dir_path);
    if (d == NULL) {
        // perror выведет наше сообщение и системное описание ошибки
        perror(dir_path);
        return 1;
    }

    int count = 0;
    struct dirent *dir_entry;
    const char *suffix = ".exe";
    const size_t suffix_len = strlen(suffix);

    // Читаем содержимое каталога в цикле
    while ((dir_entry = readdir(d)) != NULL) {
        const char *name = dir_entry->d_name;
        size_t name_len = strlen(name);

        // 1. Проверка на суффикс .exe. Самая быстрая, делаем ее первой.
        if (name_len < suffix_len || strcmp(name + name_len - suffix_len, suffix) != 0) {
            continue;
        }

        // Для системных вызовов stat/access нужен полный путь к файлу.
        // Собираем его, не меняя текущий каталог процесса (как в условии).
        char full_path[PATH_MAX];
        int res = snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, name);
        if (res < 0 || res >= sizeof(full_path)) {
            // Ошибка при формировании пути или буфер слишком мал
            // В рамках задачи просто пропустим такой файл
            continue;
        }

        struct stat st;
        // Используем stat, а не lstat, чтобы следовать по символическим ссылкам
        if (stat(full_path, &st) == -1) {
            // Возможно, битая ссылка или файл удалили, пока мы работали. Игнорируем.
            continue;
        }

        // 2. Проверка, что файл регулярный
        if (!S_ISREG(st.st_mode)) {
            continue;
        }
        
        // 3. Проверка на доступность для выполнения текущему пользователю
        if (access(full_path, X_OK) != 0) {
            continue;
        }
        
        // Если все проверки пройдены, то это наш файл
        count++;
    }

    // Важно не забывать закрывать то, что открыли
    closedir(d);

    printf("%d\n", count);
    
    return 0;
}

