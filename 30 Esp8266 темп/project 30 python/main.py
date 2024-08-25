import time

import paho.mqtt.client as mqtt
import pandas as pd

BROKER = ''
USERNAME = ''
PASSWORD = ''
PORT = 1883
DATA_TOPIC = ''
locations = {}

with open('locations.txt', 'r', encoding='utf-8') as file:
    data_txt = file.readlines()

    for line in data_txt:
        line = line.replace('\n', '')
        line = line.replace(' ', '')
        line = line.split('=')
        mac_address = line[0]
        address = line[1]
        locations[mac_address] = address

with open('settings.txt', 'r', encoding='utf-8') as file:
    data_txt = file.readlines()
    data = []

    for line in data_txt:
        line = line.replace('\n', '')
        line = line.replace(' ', '')
        line = line.split('=')
        data.append(line)

    BROKER = data[0][1]
    USERNAME = data[1][1]
    PASSWORD = data[2][1]
    PORT = int(data[3][1])
    DATA_TOPIC = data[4][1]


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print('[MQTT] Подключено успешно')
    else:
        print(f'[MQTT ERROR] Ошибка со статусом: {rc}')


def on_message(client, userdata, msg):
    msg_topic = msg.topic
    payload = msg.payload.decode()
    data = payload.replace(' ', '')
    data = data.split('-')
    # print(data)

    location = locations[data[0]]
    temp = data[1]

    print(location, temp, sep=' | ')
    # print(f'[MQTT MESSAGE] {msg_topic}: {payload}')

    write_to_excel(location, temp)


def write_to_excel(location, temp):
    file_path = f'G:/Мой диск/Документы/{location}.xlsx'

    df_new = pd.DataFrame({
        'Время': [time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())],
        'Температура': [temp],
    })

    try:
        # Попытка загрузки существующего файла
        df_existing = pd.read_excel(file_path)
        df_updated = pd.concat([df_existing, df_new], ignore_index=True)
        df_updated.to_excel(file_path, sheet_name=location, index=False)

    except FileNotFoundError:
        # Запись новых данных в новый файл Excel
        df_new.to_excel(file_path, index=False)
        return


if __name__ == '__main__':
    client = mqtt.Client()

    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set(USERNAME, PASSWORD)
    client.connect(BROKER, PORT, 60)
    client.subscribe(DATA_TOPIC)

    client.loop_forever()
