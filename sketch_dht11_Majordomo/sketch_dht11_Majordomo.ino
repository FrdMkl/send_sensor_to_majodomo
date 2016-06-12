#include <dht.h>
#include <Ethernet.h>
#include <SPI.h>

// MAC-адрес нашего устройства
byte mac[] = { 0x00, 0x2A, 0xF6, 0x12, 0x68, 0xFC };
// ip-адрес устройства
IPAddress ip(192,168,1,72);
// ip-адрес удалённого сервера
IPAddress server(192,168,1,12); // измените на свой
EthernetClient rclient;

int old_temperature=0;
int old_hyd=0;

char msg[128];

DHT sensor = DHT();

// Функция отправки HTTP-запроса на сервер
void sendHTTPRequest() {
  Serial.println(msg); 
  if (rclient.connect(server, 80)) { 
   Serial.println("OK"); 
   rclient.println(msg);
   rclient.println("Host: 192.168.1.12");
   rclient.println();   
   delay(2000);
   rclient.stop();
  } else {
   Serial.println("FAILED");     
  }
}
void setup()
{
 Ethernet.begin(mac, ip); // Инициализируем Ethernet Shield
 sensor.attach(A0);
 delay(1000);
 Serial.begin(9600);
}

void loop()
{
    // метод update заставляет сенсор выдать текущие измерения
    sensor.update();
     int current_temp=0;
     int current_hyd=0;
    
    switch (sensor.getLastError())
    {
        case DHT_ERROR_OK:
            current_temp=sensor.getTemperatureInt(); // получаем температуру
            current_hyd=sensor.getHumidityInt();
            // данные последнего измерения можно считать соответствующими
            // методами
            sprintf(msg, "Temperature = %dC, Humidity = %d%%", current_temp, current_hyd);
            Serial.println(msg);
            if ((old_temperature!=(int)current_temp) && (current_temp<79) && (current_temp>5)) {
            sprintf(msg, "GET /objects/?object=humSensor01&op=m&m=humChanged&h=%0d HTTP/1.0", current_hyd);
            sendHTTPRequest();
            sprintf(msg, "GET /objects/?object=tempSensor01&op=m&m=tempChanged&t=%0d HTTP/1.0", current_temp);   
            sendHTTPRequest();
            }
            break;
        case DHT_ERROR_START_FAILED_1:
            Serial.println("Error: start failed (stage 1)");
            break;
        case DHT_ERROR_START_FAILED_2:
            Serial.println("Error: start failed (stage 2)");
            break;
        case DHT_ERROR_READ_TIMEOUT:
            Serial.println("Error: read timeout");
            break;
        case DHT_ERROR_CHECKSUM_FAILURE:
            Serial.println("Error: checksum error");
            break;
    }
 
    delay(2000);
}
