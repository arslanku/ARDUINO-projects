void build()
{
    GP.BUILD_BEGIN(700);
    GP.ONLINE_CHECK();
    GP.UPDATE("relay_led1,relay_led2,relay_led3,relay_led4,relay_led5,relay_led6,temp1,temp2,temp3,temp4,led3", 1000);
    GP.THEME(GP_DARK);

    // GP.BREAK();
    // GP.TITLE("Охлаждение");
    // GP.BREAK();
    // GP.HR();
    // GP.BREAK();

    GP.BLOCK_BEGIN(GP_TAB, "80%", "Ручное управление");
    // GP.BREAK();
    // GP.BREAK();

    GP.LABEL("Реле 6");
    GP.SWITCH("relay6", relay_state[5]);
    GP.BREAK();
    // GP.BREAK();

    // GP.BREAK();
    // GP.BREAK();

    GP.BLOCK_END();
    GP.BLOCK_BEGIN(GP_TAB, "80%", "Состояние реле");
    // GP.BREAK();

    GP.LABEL("Реле 1");
    GP.LED("relay_led1", relay_state[0]);
    // GP.BREAK();

    GP.LABEL("Реле 2");
    GP.LED("relay_led2", relay_state[1]);
    GP.BREAK();

    GP.LABEL("Реле 3");
    GP.LED("relay_led3", relay_state[2]);
    // GP.BREAK();

    GP.LABEL("Реле 4");
    GP.LED("relay_led4", relay_state[3]);
    GP.BREAK();

    GP.LABEL("Реле 5");
    GP.LED("relay_led5", relay_state[4]);
    // GP.BREAK();

    GP.LABEL("Реле 6");
    GP.LED("relay_led6", relay_state[5]);
    GP.BREAK();

    GP.BLOCK_END();

    // GP.BREAK();
    // GP.HR();
    // GP.BREAK();

    GP.BLOCK_BEGIN(GP_TAB, "80%", "Данные с датчиков");
    // GP.BREAK();

    GP.LABEL("Температура 1");
    GP.NUMBER("temp1", "", temp[0], "15%", 1);
    GP.BREAK();

    GP.LABEL("Температура 2");
    GP.NUMBER("temp2", "", temp[1], "15%", 1);
    GP.BREAK();

    GP.LABEL("Температура 3");
    GP.NUMBER("temp3", "", temp[2], "15%", 1);
    GP.LED("led3", !led_state[2]);
    GP.BREAK();

    GP.LABEL("Температура 4");
    GP.NUMBER("temp4", "", temp[3], "15%", 1);
    GP.BREAK();

    GP.BLOCK_END();

    GP.BREAK();
    GP.HR();
    GP.BREAK();

    GP.BLOCK_BEGIN(GP_TAB, "80%", "Настройка пороговых значений");
    GP.FORM_BEGIN("/temp_settings_form");
    GP.BREAK();

    GP.LABEL("Темп макс 1");
    GP.NUMBER("temp1_max", "", temp_max[0], "15%", 0);
    GP.BREAK();

    GP.LABEL("Темп макс 2");
    GP.NUMBER("temp2_max", "", temp_max[1], "15%", 0);
    GP.BREAK();

    GP.LABEL("Темп макс 3");
    GP.NUMBER("temp3_max", "", temp_max[2], "15%", 0);
    GP.BREAK();

    GP.LABEL("Темп макс 4");
    GP.NUMBER("temp4_max", "", temp_max[3], "15%", 0);
    GP.BREAK();

    GP.LABEL("Темп макс 5");
    GP.NUMBER("temp5_max", "", temp_max[4], "15%", 0);
    GP.BREAK();

    GP.LABEL("Темп макс 6");
    GP.NUMBER("temp6_max", "", temp_max[5], "15%", 0);
    GP.BREAK();
    GP.BREAK();

    GP.SUBMIT_MINI("Сохранить");

    GP.FORM_END();
    GP.BLOCK_END();

    // GP.BREAK();
    // GP.HR();
    // GP.BREAK();

    GP.BUILD_END();
}
