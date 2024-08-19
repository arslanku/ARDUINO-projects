import pandas as pd
import time


def write_to_excel(location, temp):
    existing_excel_file_path = 'data.xlsx'

    xl = pd.ExcelFile(existing_excel_file_path)
    print(xl.sheet_names)  # see all sheet names
    print(xl.parse('Sheet1'))  # read a specific sheet to DataFrame)  # see all sheet names

    try:
        # Попытка загрузки существующего файла
        book = pd.ExcelFile(existing_excel_file_path)
    except FileNotFoundError:
        # Если файл не найден, создаем новый с листом для текущего адреса
        df_new = pd.DataFrame({
            'Время': [time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())],
            'Адрес': [location],
            'Температура': [temp],
        })

        # Запись новых данных в новый файл Excel
        df_new.to_excel(existing_excel_file_path, sheet_name=location, index=False)
        return

    # Попытка чтения существующих данных по адресу
    try:
        df_existing = pd.read_excel(existing_excel_file_path, sheet_name=location)
        df_new = pd.DataFrame({
            'Время': [time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())],
            'Адрес': [location],
            'Температура': [temp],
        })
        df_updated = pd.concat([df_existing, df_new], ignore_index=True)

        # Запись обновленных данных обратно в файл Excel
        with pd.ExcelWriter(existing_excel_file_path, engine='openpyxl') as writer:
            writer.book = book.book
            writer.sheets = dict((ws.title, ws) for ws in book.book.worksheets)
            df_updated.to_excel(writer, sheet_name=location, index=False)

    except KeyError:
        # Если лист с таким именем не найден, добавим данные в новый лист
        df_new = pd.DataFrame({
            'Время': [time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())],
            'Адрес': [location],
            'Температура': [temp],
        })

        # Запись новых данных в существующий файл Excel
        with pd.ExcelWriter(existing_excel_file_path, engine='openpyxl', mode='a') as writer:
            writer.book = book.book
            writer.sheets = dict((ws.title, ws) for ws in book.book.worksheets)
            df_new.to_excel(writer, sheet_name=location, index=False)

    finally:
        # Закрываем файл Excel
        book.close()


# Пример использования функции
write_to_excel("ул.Пушкина 10", 26)
write_to_excel("ул.Пушкина 20", 26)
