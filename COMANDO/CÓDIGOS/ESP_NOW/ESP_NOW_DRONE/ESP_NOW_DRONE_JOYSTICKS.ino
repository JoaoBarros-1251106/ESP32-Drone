#include <WiFi.h>
#include <esp_now.h>


//struct que vai receber, tem de ser exatamente igual a do transmissor
struct Dados{
  bool armado;
  int modo;

  int JOYLX;
  int JOYLY;
  int JOYRX;
  int JOYRY;
};


//CALLBACK
void OnDataRecv(const esp_now_recv_info_t *info,const uint8_t *incomingData,int len){ //Função que corre quando chega uma mensagem 
  //Serial.println("Mensagem recebida!");

  //receber um valor int
  //int numRecebido;
  //memcpy(&numRecebido,incomingData,sizeof(numRecebido));//copia os bytes recebidos para o int 
  //Serial.print("recebi :");
  //Serial.println(numRecebido);

  //receber struct
  Dados dadosRecebido;
  memcpy(&dadosRecebido,incomingData,sizeof(dadosRecebido));
  Serial.print("armado :");
  Serial.println(dadosRecebido.armado);
  Serial.print("modo :");
  Serial.println(dadosRecebido.modo);
  Serial.println("LX:");
  Serial.println(dadosRecebido.JOYLX);
  Serial.println("LY:");
  Serial.println(dadosRecebido.JOYLY);
  Serial.println("RX:");
  Serial.println(dadosRecebido.JOYRX);
  Serial.println("RY:");
  Serial.println(dadosRecebido.JOYRY);
  Serial.println("----------------");

}



void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println("WiFi STA OK");

  if(esp_now_init() == ESP_OK){
    Serial.println("ESP_NOW OK");
  } else {
    Serial.println("ERRO ESP_NOW");
  }

  esp_now_register_recv_cb(OnDataRecv);//Quando recebe algo, chama OnDataRecv() e esta Le os bytes, Converter para variáveis, Guardar numa struct, Usar os dados, ou seja para rocessar/tratar os dados recebidos.
}

void loop() {
  // put your main code here, to run repeatedly:

}
