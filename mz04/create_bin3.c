#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // для LLONG_MIN

int main() {
    const char *filename = "input.bin";

    // --- Данные для записи ---
    // Можете менять этот массив, чтобы создавать разные тестовые файлы.
    // Пример 1: Основной случай
    long long data[] = {3, -2, 4};

    // Пример 2: Тест с LLONG_MIN (раскомментируйте, чтобы использовать)
    // long long data[] = {10, -50, LLONG_MIN, 20};

    // Пример 3: Тест с несколькими одинаковыми минимумами
    // long long data[] = {5, 8, -100, 9, -100};

    // Вычисляем количество элементов в массиве
    size_t count = sizeof(data) / sizeof(data[0]);

    // Открываем файл в режиме бинарной записи ("wb")
    // 'w' - write (запись), 'b' - binary (бинарный режим)
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Не удалось создать файл");
        return 1;
    }
    
    // Записываем весь массив в файл за один раз
    fwrite(data, sizeof(long long), count, f);

    // Закрываем файл
    fclose(f);

    printf("Бинарный файл '%s' успешно создан с %zu элементами.\n", filename, count);

    return 0;
}

