#define sensorTA12 A5           // назначаем пин для подключения датчика

float nVPP;                     // напряжение на резисторе
float nCurrThruResistorPP;      // пиковый ток на резисторе
float nCurrThruResistorRMS;     // среднеквадратичное значение тока на резисторе
float nCurrentThruWire;         // актуальное среднекватратичное значение тока

// следующая функция узнает пиковое значение за одну секунду
float getVPP() {
  
    float result;
    int readValue;
    int maxValue = 0;
    uint32_t start_time = millis();
    while ((millis() - start_time) < 1000)
    {
        readValue = analogRead(sensorTA12);

        if (readValue > maxValue) {
            maxValue = readValue;
        }
   }
    result = (maxValue * 5.0) / 1024.0;

    return result;
}


void setup_TA12_100() {
    Serial.begin(9600);
    pinMode(sensorTA12, INPUT);
}

void func() {
    // узнаем напряжение на резисторе с помощью функции getVPP()
    nVPP = getVPP();

    // используем закон Ома для расчета тока на резисторе
    nCurrThruResistorPP = (nVPP / 200.0) * 1000.0;

    // преобразуем значения тока в среднекватичное значение
    nCurrThruResistorRMS = nCurrThruResistorPP * 0.707;

    // коэффициент трансформатора 1000:1, поэтому ток умножается на 1000
    nCurrentThruWire = nCurrThruResistorRMS * 1000;

    // выводим все данные на мониторе порта
    Serial.print("Volts Peak : ");
    Serial.println(nVPP, 3);

    Serial.print("Current Through Resistor (Peak) : ");
    Serial.print(nCurrThruResistorPP, 3);
    Serial.println(" mA Peak to Peak");

    Serial.print("Current Through Resistor (RMS) : ");
    Serial.print(nCurrThruResistorRMS, 3);
    Serial.println(" mA RMS");

    Serial.print("Current Through Wire : ");
    Serial.print(nCurrentThruWire, 3);
    Serial.println(" mA RMS");

    Serial.println();
}
