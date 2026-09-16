#include <WiFi.h>
#include <esp_now.h>


#define BTN_ARM 25
#define BTN_DISARM 27
#define BTN_MODE 26

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



}

void loop() {
  //esp_now_send(enderecoRecetor,(uint8_t *)&numero,sizeof(numero));//enderecoRecetor é o mac, (uint8_t *)&numero Pega no número e envia os bytes dele, sizeof(numero) Quantos bytes tem este número?
  //esp_now_send(enderecoRecetor,(uint8_t *)&dados,sizeof(dados));
  //Serial.println("Enviado");
 


  if(digitalRead(BTN_ARM) == LOW){
    dados.armado = true;
    Serial.println("ARMADO");
    //esp_now_send(enderecoRecetor,(uint8_t *)&dados,sizeof(dados)); assim teria menos trafego, pois só enviaria quando carrega se
  }

  if(digitalRead(BTN_DISARM) == LOW){
    dados.armado = false;
    Serial.println("DESARMADO");
    //esp_now_send(enderecoRecetor,(uint8_t *)&dados,sizeof(dados));
  }

  bool estadoMode = digitalRead(BTN_MODE);

  if(ultimoMode == HIGH & estadoMode == LOW){
    dados.modo++;
    if(dados.modo > 2){
      dados.modo = 0;
    }
    Serial.print("Modo:");
    Serial.println(dados.modo);
    //esp_now_send(enderecoRecetor,(uint8_t *)&dados,sizeof(dados));
  }

  esp_now_send(enderecoRecetor,(uint8_t *)&dados,sizeof(dados));//Lê os dados e agora envia

  delay(500);
}