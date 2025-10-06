#include <stdio.h>
#include <time.h>

// В задачах ВМК иногда любят, чтобы константы были именованными,
// это делает код читаемее.
#define THURSDAY 4 // в struct tm: Sunday=0, ..., Thursday=4, ...

int main() {
    int year;
    scanf("%d", &year);

    // Проходим по всем месяцам года. В struct tm месяцы нумеруются с 0.
    for (int month_idx = 0; month_idx < 12; ++month_idx) {
        
        // 1. Находим первый четверг месяца.
        // Для этого сначала узнаем день недели 1-го числа.
        struct tm date_spec = {0}; // Важно обнулить, чтобы не было мусора
        date_spec.tm_year = year - 1900; // Годы в tm считаются от 1900
        date_spec.tm_mon = month_idx;
        date_spec.tm_mday = 1;
        
        // mktime нормализует структуру и заполняет tm_wday
        mktime(&date_spec);
        
        int wday_of_first_day = date_spec.tm_wday;
        
        // Вычисляем, на какое число месяца приходится первый четверг.
        // Формула (target_wday - current_wday + 7) % 7 дает смещение в днях.
        int first_thursday_date = 1 + (THURSDAY - wday_of_first_day + 7) % 7;
        
        // 2. Находим 2-й четверг и проверяем условие
        int second_thursday_date = first_thursday_date + 7;
        if (second_thursday_date % 3 != 0) {
            printf("%d %d\n", month_idx + 1, second_thursday_date);
        }
        
        // 3. Находим 4-й четверг и проверяем его
        int fourth_thursday_date = first_thursday_date + 21;
        
        // Нужно убедиться, что эта дата все еще в пределах текущего месяца.
        // Снова используем mktime для проверки.
        struct tm check_date = {0};
        check_date.tm_year = year - 1900;
        check_date.tm_mon = month_idx;
        check_date.tm_mday = fourth_thursday_date;
        
        mktime(&check_date);
        
        // Если после нормализации месяц не изменился, значит, дата корректна
        if (check_date.tm_mon == month_idx) {
            if (fourth_thursday_date % 3 != 0) {
                printf("%d %d\n", month_idx + 1, fourth_thursday_date);
            }
        }
    }
    
    return 0;
}

