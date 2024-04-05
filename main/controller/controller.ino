#include<SPI.h>
#include <RF24.h>

RF24 rf24(7, 8); // 創建 RF24 物件，設定 CE 和 CSN 腳
const byte addr[] = "boat1";// 定義無線通信地址

const int lx = A5;// 左搖桿 X 軸腳
const int ly = A4;  // 左搖桿 Y 軸腳
const int rx = A2;  // 右搖桿 X 軸腳
const int ry = A1;  // 右搖桿 Y 軸腳
int ax,ay,bx,by,lw,ls,rw,rs;// 定義變數來存放搖桿數值和馬達狀態
int way = 1;// 方向變數

void lmotor(int a){ //判斷左馬達的速度和方向
  if(a<=320)  lw=0;
  else if(a>=350) lw=1;
  else{
    lw=0;
    ls=0;
  }
  if(lw)
    ls = map(a,345,691,255,0);//443
	else if(!lw)
    ls = map(a,325,0,0,255);
    
  if(ls < 0){
    lw=0;
    ls = 0;
  }
}

void rmotor(int a){ //判斷右馬達的速度和方向
  if(a<=320)  rw=0;
  else if(a>=350) rw=1;
  else{
    rw=0;
    rs=0;
  }
  if(rw)
    rs = map(a,345,691,255,0);//443
  else if(!rw)
    rs = map(a,325,0,0,255);
    
  if(rs < 0){
    rw=0;
    rs = 0;
  }
}

void setup(){
	pinMode(lx,INPUT);//腳位狀態設置
	pinMode(ly,INPUT);
	pinMode(rx,INPUT);
	pinMode(ry,INPUT);
	Serial.begin(115200);//打開監視序列
 
  rf24.begin();
  rf24.setChannel(83);       // 設定頻道編號
  rf24.openWritingPipe(addr); // 設定通道位址
  rf24.setPALevel(RF24_PA_MIN);   // 設定廣播功率
  rf24.setDataRate(RF24_250KBPS); // 設定傳輸速率
  rf24.stopListening();       // 停止偵聽；設定成發射模式
  
  Serial.println("Setup done");
}

void loop(){
	ax = analogRead(lx);
	ay = analogRead(ly);
	bx = analogRead(rx);
	by = analogRead(ry);

  lmotor(ay);
  rmotor(by);

  int data[4] = {lw , ls , rw ,rs};
  rf24.write(&data, sizeof(data));//開始通訊

  Serial.print("lw = ");//序列監視
  Serial.print(lw);
  Serial.print(" ls = ");
  Serial.print(ls);
  Serial.print(" rw = ");
  Serial.print(rw);
  Serial.print(" rs = ");
  Serial.println(rs);
  
	delay(100);//延遲0.1秒
  
}
