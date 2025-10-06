#include <stdio.h> // Только для main и printf

/*
 * Функция для нормализации пути.
 * Работает "на месте", модифицируя переданный буфер.
 * Не использует функции стандартной библиотеки.
 */
void normalize_path(char *buf) {
    char *writer = buf; // Указатель для записи результата
    char *reader = buf; // Указатель для чтения исходной строки

    // Путь абсолютный, поэтому первый символ всегда '/'
    // Пропускаем его, чтобы основной цикл был проще.
    // writer и reader будут указывать на позицию *после* начального слэша.
    if (*reader == '/') {
        writer++;
        reader++;
    }

    // Обрабатываем компоненты пути
    while (*reader) {
        // Находим начало и конец текущего компонента
        char *component_start = reader;
        while (*reader && *reader != '/') {
            reader++;
        }
        
        // component_len - длина текущего компонента ("a", ".", "..")
        int component_len = reader - component_start;

        if (component_len == 0) {
            // Это последовательность вроде "//", но по условию ее нет.
            // На всякий случай, если reader указывает на '/', пропускаем его.
        } else if (component_len == 1 && *component_start == '.') {
            // Компонент "." (текущий каталог), просто игнорируем
        } else if (component_len == 2 && *component_start == '.' && *(component_start + 1) == '.') {
            // Компонент ".." (родительский каталог)
            // Откатываем writer до предыдущего слэша
            if (writer > buf + 1) { // Убедимся, что мы не в корне "/"
                writer--; // Встаем на последний '/'
                while (writer > buf + 1 && *(writer - 1) != '/') {
                    writer--; // Идем назад до предыдущего '/'
                }
            }
        } else {
            // Обычный компонент имени
            // Копируем его в позицию writer
            char *p = component_start;
            while (p < reader) {
                *writer = *p;
                writer++;
                p++;
            }
            // Добавляем слэш после компонента
            *writer = '/';
            writer++;
        }

        // Если встретили '/', пропускаем его, чтобы reader указывал на начало след. компонента
        if (*reader) {
            reader++;
        }
    }

    // Если в результате что-то есть и это не просто "/", убираем последний слэш
    if (writer > buf + 1 && *(writer - 1) == '/') {
        writer--;
    }

    // Завершаем строку
    *writer = '\0';
}

// ---- Тестовая обвязка, чтобы проверить, как оно работает ----
// ее можно убрать при сдаче, оставив только саму функцию normalize_path

// Простая самописная strcpy для тестов
void my_strcpy(char *dest, const char *src) {
    while ((*dest++ = *src++));
}

int main() {
    char buffer[1024];

    // Тест 1
    my_strcpy(buffer, "/a/b/c/");
    normalize_path(buffer);
    printf("Input:  /a/b/c/\nOutput: %s\n\n", buffer);  // Ожидаем /a/b/c

    // Тест 2
    my_strcpy(buffer, "/a/b/../");
    normalize_path(buffer);
    printf("Input:  /a/b/../\nOutput: %s\n\n", buffer); // Ожидаем /a

    // Тест 3
    my_strcpy(buffer, "/a/b/../../..");
    normalize_path(buffer);
    printf("Input:  /a/b/../../../\nOutput: %s\n\n", buffer); // Ожидаем /

    // Тест 4
    my_strcpy(buffer, "/");
    normalize_path(buffer);
    printf("Input:  /\nOutput: %s\n\n", buffer); // Ожидаем /

    // Тест 5
    my_strcpy(buffer, "/a/./b/././c/d/../e/");
    normalize_path(buffer);
    printf("Input:  /a/./b/././c/d/../e/\nOutput: %s\n\n", buffer); // Ожидаем /a/b/c/e

    // Тест 6, подъем из корня
    my_strcpy(buffer, "/../a/b/c");
    normalize_path(buffer);
    printf("Input:  /../a/b/c\nOutput: %s\n\n", buffer); // Ожидаем /a/b/c

    return 0;
}

