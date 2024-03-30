#define BLYNK_TEMPLATE_ID "Seu_ID"
#define BLYNK_TEMPLATE_NAME "Casa"
#define BLYNK_AUTH_TOKEN "Seu_token"

#include "WiFi.h"
#include "BlynkSimpleEsp32.h"
#include "DHT.h"
#include "ESP32Servo.h"

#define sensorDHT 22
#define TipoDHT DHT11
#define sensorLumi 33
#define LED_quarto1 2
#define LED_sala 5
#define LED_banheiro 15
#define Lumi_PIN 33
#define Solar 32
#define Lampadas 19
#define servo 18

Servo portao;
DHT dht(sensorDHT, TipoDHT);

const char* server = "Nome_da_sua_rede";
const char* senha = "Senha_da_sua_rede";

float energia = 0;


BLYNK_WRITE(V4){
  int abrir = param.asInt();
  if(abrir == 1){
    portao.attach(servo);
    portao.write(180);
  }else{
    portao.attach(servo);
    portao.write(90);
  }
}

BLYNK_WRITE(V5){
  digitalWrite(LED_sala, param.asInt());
}

BLYNK_WRITE(V6){
  digitalWrite(LED_quarto1, param.asInt());
}

BLYNK_WRITE(V7){
  digitalWrite(LED_banheiro, param.asInt());
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_quarto1, OUTPUT);
  pinMode(LED_sala, OUTPUT);
  pinMode(LED_banheiro, OUTPUT);
  pinMode(Solar, INPUT);
  WiFi.begin(server, senha);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print("\Conectando ao WIFI: ");
    Serial.println(server);
  }
  Blynk.begin(BLYNK_AUTH_TOKEN, server, senha, "blynk.cloud", 80);
  dht.begin();
}

void loop() {
  bool fogo = 0;
  energia = (analogRead(Solar)*5.00)/4095;
  energia = energia/(10000.0/(100000.0+10000.0));
  int Luminosidade = analogRead(Lumi_PIN);
  float umidade = dht.readHumidity();
  float temperatura= dht.readTemperature();
  // R1 = 100000.0 e R2 = 10000.0
  Serial.println(analogRead(Solar));
  Serial.println(Luminosidade);
  if(Luminosidade < 1000){
    digitalWrite(Lampadas, HIGH);
  }else{
    digitalWrite(Lampadas, LOW);
  }
  Blynk.virtualWrite(V0, energia);
  Blynk.virtualWrite(V1, temperatura);
  Blynk.virtualWrite(V2, umidade);
  delay(1000);
}
