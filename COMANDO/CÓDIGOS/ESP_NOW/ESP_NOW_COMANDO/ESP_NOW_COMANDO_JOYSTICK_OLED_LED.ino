#include <WiFi.h>
#include <esp_now.h>

//oled
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//LED
#define LED_R 32
#define LED_G 33
#define LED_B 4

//botoes
#define BTN_ARM 25
#define BTN_DISARM 27
#define BTN_MODE 26

//Joysticks
#define JOY_LX 34
#define JOY_LY 35
#define JOY_LZ 16
#define JOY_RX 36
#define JOY_RY 39
#define JOY_RZ 17

//oled
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64//dimensoes

#define OLED_MOSI 23
#define OLED_CLK 18
#define OLED_DC 22
#define OLED_CS 5
#define OLED_RESET 21//pinos


//funcao display
Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  OLED_MOSI,
  OLED_CLK,
  OLED_DC,
  OLED_RESET,
  OLED_CS
);

bool ultimoMode = HIGH;

uint8_t enderecoRecetor[] =//endereco mac do destinatario (peer), maiusculas ou minisculas são exatamente a mesma coisa em hexadecimal
{
  0x68,
  0x09,
  0x47,
  0x48,
  0x1c,
  0xac
};

//int numero = 123; //variavel int teste

//enviar struct 
struct Dados {
  bool armado;
  int modo;

  
  int JOYLX;
  int JOYLY;
  int JOYRX;
  int JOYRY;

};
Dados dados;


void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);//ativa rádio wifi

  Serial.println("WiFi STA OK");

  if(esp_now_init() == ESP_OK){ //ATIVA PROTOCOLO ESP NOW  E VERIFICA SE ESTA ATIVO
    Serial.println("ESP_NOW OK");
  } else {
    Serial.println("ERRO ESP_NOW");
  }

  esp_now_peer_info_t peerInfo;//Cria uma "ficha de contacto"
  memcpy(peerInfo.peer_addr,enderecoRecetor,6);//coloca o endereço dentro dessa ficha
  peerInfo.channel = 0; //diz ao esp "Usa o canal atual do Wi-Fi"
  peerInfo.encrypt = false; //Não quero encriptação
  peerInfo.ifidx = WIFI_IF_STA; //significa "Este peer vai comunicarpela interface STA(WiFi Station)"
  if (esp_now_add_peer(&peerInfo) == ESP_OK){ //Guarda este contactona lista de destinatários e verifica se funcionou
  Serial.println("Peer adicionado");
  } else {
  Serial.println("Erro Peer");
  }

        
  //dados da struct
  dados.armado = true;
  dados.modo = 0;

  pinMode(BTN_ARM, INPUT_PULLUP); // significa sempre high, quando pressionado fica low
  pinMode(BTN_DISARM, INPUT_PULLUP);
  pinMode(BTN_MODE, INPUT_PULLUP);

  dados.JOYLX = 2048;//analogRead(JOY_LX);
  dados.JOYLY = 2048;//analogRead(JOY_LY);
  dados.JOYRX = 2048;//analogRead(JOY_RX);
  dados.JOYRY = 2048;//analogRead(JOY_RY);
  //4096/2=2048, ou seja o centro


  //oled
  display.begin(SSD1306_SWITCHCAPVCC);

  display.clearDisplay();
  display.display();


  //LED
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);


}

void loop() {
  //esp_now_send(enderecoRecetor,(uint8_t *)&numero,sizeof(numero));//enderecoRecetor é o mac, (uint8_t *)&numero Pega no número e envia os bytes dele, sizeof(numero) Quantos bytes tem este número?
  //esp_now_send(enderecoRecetor,(uint8_t *)&dados,sizeof(dados));
  //Serial.println("Enviado");
 


  if(digitalRead(BTN_ARM) == LOW){
    dados.armado = true;
    Serial.println("ARMADO");
    esp_now_send(enderecoRecetor,(uint8_t *)&dados,sizeof(dados)); //assim tem menos trafego, pois só enviaria quando carrega se
  }

  if(digitalRead(BTN_DISARM) == LOW){
    dados.armado = false;
    Serial.println("DESARMADO");
    esp_now_send(enderecoRecetor,(uint8_t *)&dados,sizeof(dados));
  }

  bool estadoMode = digitalRead(BTN_MODE);

  if(ultimoMode == HIGH & estadoMode == LOW){
    dados.modo++;
    if(dados.modo > 2){
      dados.modo = 0;
    }
    Serial.print("Modo:");
    Serial.println(dados.modo);
    esp_now_send(enderecoRecetor,(uint8_t *)&dados,sizeof(dados));
  }

  //esp_now_send(enderecoRecetor,(uint8_t *)&dados,sizeof(dados));//Lê os dados e ta sempre a enviar sem para

  //OLED

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("CONTROLO DRONE");

  display.setCursor(0, 20);
  display.print("ARM: ");
  display.println(dados.armado ? "ON" : "OFF");

  display.setCursor(0, 35);
  display.print("MODE: ");

  switch (dados.modo) {

    case 0:
      display.println("SAFE");
      break;

    case 1:
      display.println("NORMAL");
      break;

    case 2:
      display.println("SPORT");
      break;
  }

  display.display();

  if (!dados.armado) {

    // Vermelho
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);

  } else {

    switch (dados.modo) {

      case 0: // SAFE
        digitalWrite(LED_R, LOW);
        digitalWrite(LED_G, HIGH);
        digitalWrite(LED_B, LOW);
        break;

      case 1: // NORMAL
        digitalWrite(LED_R, HIGH);
        digitalWrite(LED_G, HIGH);
        digitalWrite(LED_B, LOW);
        break;

      case 2: // SPORT
        digitalWrite(LED_R, LOW);
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_B, HIGH);
        break;
    }
  }

  delay(500);
}