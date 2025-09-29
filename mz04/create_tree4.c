#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Та же структура, что и в задаче
struct Node {
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

// Функция для конвертации 32-битного числа из формата хоста в Big-Endian (массив байт)
// Это обратная операция к той, что в вашей основной программе
void convert_to_be(uint32_t val, unsigned char *buf) {
    buf[0] = (val >> 24) & 0xFF; // Самый старший байт
    buf[1] = (val >> 16) & 0xFF;
    buf[2] = (val >> 8)  & 0xFF;
    buf[3] = val         & 0xFF; // Самый младший байт
}

int main() {
    const char *filename = "tree.bin";
    
    // Описываем наше дерево как массив структур
    struct Node tree[] = {
        // {key, left_idx, right_idx}
        {100, 1, 2}, // Индекс 0 (корень)
        {50,  0, 3}, // Индекс 1
        {200, 4, 0}, // Индекс 2
        {75,  0, 0}, // Индекс 3
        {150, 0, 0}  // Индекс 4
    };
    
    // Открываем файл для бинарной записи
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Не удалось создать файл");
        return 1;
    }

    // Буфер для одного узла в Big-Endian формате (12 байт)
    unsigned char node_buffer[12];
    size_t num_nodes = sizeof(tree) / sizeof(tree[0]);

    // Проходим по каждому узлу, конвертируем его и записываем в файл
    for (size_t i = 0; i < num_nodes; ++i) {
        convert_to_be(tree[i].key,       &node_buffer[0]);
        convert_to_be(tree[i].left_idx,  &node_buffer[4]);
        convert_to_be(tree[i].right_idx, &node_buffer[8]);
        
        fwrite(node_buffer, sizeof(node_buffer), 1, f);
    }
    
    fclose(f);
    printf("Файл '%s' успешно создан.\n", filename);

    return 0;
}

