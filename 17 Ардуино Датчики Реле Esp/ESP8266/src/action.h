void action(GyverPortal &p)
{
  if (portal.click("relay6"))
  {
    relay_state[5] = !relay_state[5];
    Serial.println(relay_state[5]);
  }

  if (portal.form("/temp_settings_form"))
  {
    temp_max[0] = portal.getString("temp1_max").toInt();
    temp_max[1] = portal.getString("temp2_max").toInt();
    temp_max[2] = portal.getString("temp3_max").toInt();
    temp_max[3] = portal.getString("temp4_max").toInt();
    temp_max[4] = portal.getString("temp5_max").toInt();
    temp_max[5] = portal.getString("temp6_max").toInt();

    mem1.update();

    Serial.println(temp_max[0]);
    Serial.println(temp_max[1]);
    Serial.println(temp_max[2]);
    Serial.println(temp_max[3]);
    Serial.println(temp_max[4]);
    Serial.println(temp_max[5]);
  }

  // if (portal.form("/wifi_settings_form"))
  // {
  //   WIFI_SSID = portal.getString("inp_ssid");
  //   WIFI_PASSWORD = portal.getString("inp_pass");

  //   mem2.update();
  //   mem3.update();

  //   Serial.println(WIFI_SSID);
  //   Serial.println(WIFI_PASSWORD);
  // }

  if (portal.update())
  {
    portal.updateBool("relay_led1", relay_state[0]);
    portal.updateBool("relay_led2", relay_state[1]);
    portal.updateBool("relay_led3", relay_state[2]);
    portal.updateBool("relay_led4", relay_state[3]);
    portal.updateBool("relay_led5", relay_state[4]);
    portal.updateBool("relay_led6", relay_state[5]);

    portal.updateBool("led3", led_state[2]);

    portal.updateInt("temp1", temp[0]);
    portal.updateInt("temp2", temp[1]);
    portal.updateInt("temp3", temp[2]);
    portal.updateInt("temp4", temp[3]);
  }
}