# 1. Переменные
# --------------------
# Компилятор
CC = gcc
# Имя выходного исполняемого файла
TARGET = mz5

# Файлы с исходным кодом (.c)
SOURCES = test-5.c mz03-5.c
# Заголовочные файлы (.h), от которых зависит сборка
HEADERS = 

# Файлы для форматирования
FORMAT_FILES = $(SOURCES) $(HEADERS)

# Флаги компилятора: -std=gnu23 из вашей заготовки является аналогом c2x/c23
# Используем флаги из вашей заготовки, так как они более полные и строгие
CFLAGS = -Wall -Werror -std=gnu23 -O2 -Werror=vla

# 2. Цели сборки
# --------------------

# .PHONY указывает, что цель не является файлом. Это стандартная практика.
# 'all' - цель по умолчанию. Вызов 'make' без аргументов выполнит 'make all'.
.PHONY: all
all: $(TARGET)

# Основное правило сборки:
# Цель '$(TARGET)' зависит от '$(SOURCES)' и '$(HEADERS)'.
# Это означает, что если любой из .c или .h файлов изменится,
# цель будет пересобрана.
$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

# Цель для форматирования кода
.PHONY: format
format:
	@echo "Formatting files: $(FORMAT_FILES)..."
	@clang-format -i $(FORMAT_FILES)
	@echo "Formatting complete."

# Цель для очистки проекта
.PHONY: clean
clean:
	@echo "Cleaning up..."
	@rm -f $(TARGET)
	@echo "Clean complete."


