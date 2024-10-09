/*
 * Nồng độ CO2 trong khí quyển..............400ppm
 * Nồng độ CO2 trung bình trong nhà.............350-450ppm
 * Nồng độ CO2 tối đa chấp nhận được...........1000ppm
 * Nồng độ CO2 nguy hiểm.............>2000ppm
 */

#define anInput   A0            // tín hiệu analog từ MQ135
#define co2Zero   55            // mức CO2 0 được hiệu chuẩn

void setup() 
{
 pinMode(anInput,INPUT);           // đặt nguồn cấp analog MQ135 cho đầu vào
 Serial.begin(9600);             // giao tiếp nối tiếp để gỡ lỗi
}
  
void loop() 
{
  
 int co2now[10];                // mảng int cho các giá trị đọc CO2
 int co2raw = 0;                // int cho giá trị thô của CO2
 int co2ppm = 0;                // int cho ppm được tính toán
 int zzz = 0;                 // int để tính trung bình


 for (int x = 0;x<10;x++) // lấy mẫu CO2 10 lần trong 2 giây
 {          
  co2now[x]=analogRead(A0);
  delay(200);
 }

 for (int x = 0;x<10;x++) // cộng các mẫu lại với nhau
 {           
  zzz=zzz + co2now[x];  
 }
  
 co2raw = zzz/10;              // chia các mẫu cho 10
 co2ppm = co2raw - co2Zero;         // lấy ppm được tính toán

 Serial.print("AirQuality=");
 Serial.print(co2ppm); // in ra giá trị đọc được
 Serial.println(" PPM");
 delay(50);       
}
