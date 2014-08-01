/*GR-SAKURA Sketch Template Version: V1.08*/

#include <rxduino.h>
#include <sdmmc.h>
#include "JPEGCamera.h"

// CS Pin of SD card 
#define SDCS 22
SDMMC mySD;



////////////// JPEGCamera ///////////////

#define READ_BUFFER_SIZE 64

char filename[12] = "pic0.jpg";

//JPEGCameraインスタンスを作成
JPEGCamera camera;

//受信バッファ
unsigned char response[READ_BUFFER_SIZE];
//受信データサイズ
unsigned int count=0;
//撮影したJPGE画像サイズ
uint16_t size=0;
//次に読み込むJPEG画像のアドレス
unsigned int address=0;

//void dateTime(uint16_t* date, uint16_t* time);



void changeResolution(JPEGCamera::ImageSize size){

    Serial.println("change image size");
    if( camera.imageSize2( size ) == false ){
        Serial.println("change image size failed");
        while(1);
    }
}


void takepicture( void ){
    
    Serial.println("taking picture...");
    if( camera.takePicture() == false ){
        Serial.println("fail to take picture");
        while(1);
    }
    Serial.println("ok.");
    
    Serial.println("taking size...");
    if( camera.getSize(&size) == false ){
        Serial.println("getSize failed");
        while(1);
    }
    Serial.print(size);
    Serial.println();


    unsigned int count = 0;
    while(mySD.exists(filename)){
        count++;
        sprintf(filename, "pic%d.jpg", count);
    }
    File dataFile = mySD.open(filename, FILE_WRITE);
    Serial.print("filename is ");
    Serial.println(filename);

    //開始アドレスを0に、size分読み込むまで繰り返す
    address = 0;
    while(address < size)
    {
        //READ_BUFFER_SIZE byte読み込む
        count = camera.readData(response, READ_BUFFER_SIZE, address);
        if( count ){
            //SDに書き込む
            for (int i =0 ; i< READ_BUFFER_SIZE;i+=2){
                dataFile.write(response[i]);
                dataFile.write(response[i+1]);
                if((response[i] == 0xFF) && (response[i+1] == 0xD9)){//JPGの終わりを検出
                    break;
                }
            }

            //受信したデータサイズ分、アドレスを更新する
            address += count;
        } else {
            Serial.println("readData failed");
            Serial.flush();
			while(1);
        }
        delay(2); //これがないとread failする
    }
    //撮影停止（一旦止めないと新しい画像は撮影されない）
    camera.stopPictures();
    dataFile.close();
    Serial.println("Success to save picture");

}



void setup( void )
{

    pinMode(PIN_LED0, OUTPUT);
    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    pinMode(PIN_LED3, OUTPUT);
    
    digitalWrite(PIN_LED0, 0);
    digitalWrite(PIN_LED1, 0);
    digitalWrite(PIN_LED2, 0);
    digitalWrite(PIN_LED3, 0);
    
    
    // PC用
    Serial.begin(115200,SCI_USB0);
    while(!Serial.available()); // 入力を待つ
    Serial.read();


    // Camera
    Serial2.begin(38400, SCI_SCI0P2x); // to JPEG camera

    Serial.println("Camera Reset");
    if( camera.reset() == false ){
        Serial.println("reset failed");
        while(1);
    }


    Serial.println("Initializing SD card...");
    mySD.begin(SDCS);
    Serial.println("OK");


    //少しまってから撮影を開始する
    delay(2000);

    // 解像度の変更
    changeResolution(JPEGCamera::IMG_SZ_160x120);
    
    delay(1000);


}




void loop( void )
{
    
    takepicture();
  
    delay(3000);  // 少し待つ
}

