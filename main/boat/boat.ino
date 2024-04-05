#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define A_1A 2 // A_1A控制A馬達的前進（HIGH）、後退（LOW）
#define A_1B 3 // A_1B控制A馬達的速度 0~255 ，LOW表示停止
#define B_1A 4 // B_1A控制B馬達的前進（HIGH）、後退（LOW）
#define B_1B 5 // B_1B控制B馬達的速度 0~255，LOW表示停止

int lw=0,ls=0,rw=0,rs=0;

RF24 rf24(7, 8); // CE腳, CSN腳

const byte addr[] = "boat1";
const byte pipe = 1;  // 指定通道編號

void setup() {
  Serial.begin(9600);
  rf24.begin();
  rf24.setChannel(83);  // 設定頻道編號
  rf24.setPALevel(RF24_PA_MIN);
  rf24.setDataRate(RF24_250KBPS);
  rf24.openReadingPipe(pipe, addr);  // 開啟通道和位址
  rf24.startListening();  // 開始監聽無線廣播
  Serial.println("nRF24L01 ready!");
  pinMode(A_1A,OUTPUT);
  pinMode(A_1B,OUTPUT);
  pinMode(B_1A,OUTPUT);
  pinMode(B_1B,OUTPUT);
}

void loop(){
  if (rf24.available(&pipe)) {
    int data[4];
    rf24.read(&data, sizeof(data));
    for(int i=0;i<3;i++){
      Serial.print(data[i]); // 顯示訊息內容 
      Serial.print(",");
    }
    Serial.println(data[3]);
    digitalWrite(A_1A,data[0]);
    analogWrite(A_1B,data[1]);
    digitalWrite(B_1A,data[2]);
    analogWrite(B_1B,data[3]);
  }

}
