#include "HT_SSD1306Wire.h"
#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "esp_sleep.h" 

#define RF_FREQUENCY 915000000  // Hz
#define TX_OUTPUT_POWER 5       // dBm
#define LORA_BANDWIDTH 0        // 125 kHz
#define LORA_SPREADING_FACTOR 12
#define LORA_CODINGRATE 1
#define LORA_PREAMBLE_LENGTH 8
#define LORA_SYMBOL_TIMEOUT 0
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false
#define AOUT_PIN 36
#define AOUT_PIN_2 37
#define RX_TIMEOUT_VALUE 1000
#define BUFFER_SIZE 30

char txpacket[BUFFER_SIZE];
bool lora_idle = true;
int dry = 3100;
int wet = 1400;
static SSD1306Wire display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);
static RadioEvents_t RadioEvents;

#define SLEEP_DURATION (60 * 1000000ULL)  

void OnTxDone(void);
void OnTxTimeout(void);

#define LORA_RST 9 

void showMessage(String message, int delayMs = 2000) {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 32, message);
    display.display();
    delay(delayMs);
}

void reset_lora() {
    showMessage("Resetando LoRa...");
    pinMode(LORA_RST, OUTPUT);
    digitalWrite(LORA_RST, LOW);
    delay(10);
    digitalWrite(LORA_RST, HIGH);
    delay(50);
    showMessage("Reset Concluído!");
}

void VextON(void) {
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void) {
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, HIGH);
}

void setup() {
    VextON();
    delay(100);

    // Inicializa o display OLED
    display.init();
    display.clear();
    display.display();
    display.setContrast(255);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    showMessage("Inicializando...");

    SPI.begin();

    RadioEvents.TxDone = OnTxDone;
    RadioEvents.TxTimeout = OnTxTimeout;

    Radio.Init(&RadioEvents);
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                      LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                      LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                      true, 0, 0, LORA_IQ_INVERSION_ON, 3000);

    showMessage("Configuração LoRa OK!");
    showMessage("LoRa Pronto!");
    analogSetAttenuation(ADC_11db);
}

void loop() {
    if (lora_idle) {
      char sensor1char[10];
      char sensor2char[10];
      char mediaChar[10];

      int sensor1 = analogRead(AOUT_PIN);
      int sensor2 = analogRead(AOUT_PIN_2);
      sprintf(sensor1char, "%d", sensor1);
      sprintf(sensor2char, "%d", sensor2);
      showMessage("Sensor 1");

      showMessage(sensor1char);
      showMessage("Sensor 2");

      showMessage(sensor2char);

      int media = (sensor1 + sensor2) / 2;
      if(media > dry){
        wet = wet + (media - dry);
        dry = media;
      }
      else if(media < wet){
        dry = dry - (wet - media);
        wet = media;
      }

      sprintf(mediaChar, "%d", media);
      showMessage("Media");

      showMessage(mediaChar);
      int resultado = map(media, dry, wet, 0, 100);
      if(resultado < 0)
        resultado = 0;
      if(resultado > 100)
        resultado = 100;
      delay(1000);
      sprintf(txpacket, "%d", resultado);


      // Envia o pacote LoRa
      int i = 0;
      while(i < 5){
        showMessage("Enviando...");
        showMessage(txpacket, 1000);

        Radio.Send((uint8_t *)txpacket, strlen(txpacket));
        delay(12000);
        i++;
      }
      lora_idle = false;
    }

    Radio.IrqProcess();
    showMessage("delay...");
    // delay(60000); // Para testes apenas, 1min de timing
    delay(3600000);
    // showMessage("Reiniciando ESP...");
    // delay(5000);  

    // esp_restart();  

}

void OnTxDone(void) {
    lora_idle = true;
    showMessage("TX DONE!", 2000);
    // entrarDeepSleep();
}

void OnTxTimeout(void) {
    lora_idle = true;
    showMessage("TX ERROR", 2000);
    // entrarDeepSleep();
}

void entrarDeepSleep() {
    showMessage("Entrando em Deep Sleep...");

    esp_sleep_enable_timer_wakeup(SLEEP_DURATION);
  esp_sleep_enable_touchpad_wakeup();
    VextOFF();

    esp_light_sleep_start();
}
