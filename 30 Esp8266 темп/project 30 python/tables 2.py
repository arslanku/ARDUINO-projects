import pandas as pd
import time


def write_to_excel(location, temp):
    existing_excel_file_path = 'data_with_columns.xlsx'

    try:
        # Попытка загрузки существующего файла
        df_existing = pd.read_excel(existing_excel_file_path)
    except FileNotFoundError:
        # Если файл не найден, создаем новый с колонками для текущего адреса
        df_new = pd.DataFrame({
            'Адрес': [location],
            'Время': [time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())],
            'Температура': [temp],
        })

        # Запись новых данных в новый файл Excel
        df_new.to_excel(existing_excel_file_path, index=False)
        return

    # Проверяем, существует ли колонка для указанного адреса
    if location not in df_existing.columns:
        # Добавляем новую колонку для текущего адреса
        df_existing[location] = ''

    # Добавление новых данных
    new_data = {
        'Время': time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()),
        'Температура': temp,
    }

    # Добавление новых данных в соответствующую колонку
    df_existing.loc[len(df_existing), location] = f"{new_data['Время']} | {new_data['Температура']}"

    # Запись обновленных данных обратно в файл Excel
    df_existing.to_excel(existing_excel_file_path, index=False)


# Пример использования функции
write_to_excel("Пушкина 10", 25)
write_to_excel("Пушкина 20", 26)
write_to_excel("Пушкина 20", 26)
write_to_excel("Пушкина 20", 26)
write_to_excel("Пушкина 30", 26)
write_to_excel("Пушкина 30", 26)
write_to_excel("Пушкина 30", 26)
