#include<Arduino.h>
#include <WMHead.h>
#include <WMBoard.h>
//Tham khảo từ http://mualinhkien.vn/san-pham/34/cam-bien-sieu-am-srf04.html
#define ECHOPIN 4
#define TRIGPIN 3
int a;
int count=0;
int x[2];
// @brief  Lây khoảng cách tới vật thể từ cảm biến siêu âm
// @remark   nên đợi một chút, tầm 100ms trước khi gọi hàm lần nữa
/// Sensor1 (Cam bien hong ngoai do line bên trai (11) va ben phai (12))
WMLineFollower lineFollower1(11,12);
/// Tạo hanlder điều khiển động cơ trái với 2 pin 8(hướng quay), và 6(công suất PWM)
WMDCMotor motor1(8);
/// Tạo hanlder điều khiển động cơ phải với 2 pin 7(hướng quay), và 5(công suất PWM)
WMDCMotor motor2(7); 
int get_distance(){
  digitalWrite(TRIGPIN, LOW);                    // Set chan Trig xuong muc thap 2uS: ~~__
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);                   //Gui mot xung 10uS muc cao vao chan Trig ~~__----------
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);                    // Gui lai muc thap vao chan Trig         ~~__----------____
  int distance = pulseIn(ECHOPIN, HIGH);        // Đếm thời gian (đơn vị us) cho tới khi chân ECHOPIN có sườn lên
  distance= distance/58;                        // Qui đổi thời gian ra khoảng cách
  return distance;
}


void setup() {
  Serial.begin(9600);
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
}

void loop() {
 
  
//  Serial.println(get_distance());
  
  count++;
  if(count==10){
    count = 0;
    a = get_distance();
  }
 
  Serial.println(a);
   // if(k==1){
   // if(((a[0]+a[1]+a[2])/3) > 10 && ((a[0]+a[1])) > 10 && ((a[1]+a[2])/2)> 10){
   if(a>10){
       if(lineFollower1.readSensor1Status()==0 && lineFollower1.readSensor2Status()==1 ){
      /// Tùy vào góc rẽ theo ý muốn, lập trình viên thiết lập tốc độ ở hai motor cho hợp lý
      x[0]=0;
      x[1]=1;
      motor1.reverseRun(20);
      motor2.reverseRun(50);
      //delay(10);
    }
   ///Nếu như đường line lệch về phía bên phải thì cho xe rẽ sang hướng bên phải 
    else if(lineFollower1.readSensor1Status()== 1 && lineFollower1.readSensor2Status()== 0){
      /// Tùy vào góc rẽ theo ý muốn, lập trình viên thiết lập tốc độ ở hai motor cho hợp lý
      x[0]=1;
      x[1]=0;
      motor1.reverseRun(50);
      motor2.reverseRun(20);
      //delay(10);
    }  
   ///Nếu không lệch thì đi thẳng 
    else if(lineFollower1.readSensor1Status()== 0&&lineFollower1.readSensor2Status()==0){
       x[0]=1;
       x[1]=1;
       motor1.reverseRun(50);
       motor2.reverseRun(20);
    }
   ///Nếu không tìm được đường thì đi lùi lại 
    else if(lineFollower1.readSensor1Status()==1 && lineFollower1.readSensor2Status()==1){
      motor1.reverseRun(50);
      motor2.reverseRun(50);
      if(x[0]==0&&x[1]==1){
        motor1.reverseRun(20);
        motor2.reverseRun(50);
      }
      else if(x[0]==1&&x[1]==0){
        motor1.reverseRun(50);
        motor2.reverseRun(20);
      }
      else if(x[0]==1 && x[1]==1){
        motor1.reverseRun(20);
        motor2.reverseRun(20);
      }
    }
   }
   
   
          //motor1.run(0);
          //motor2.run(0);
//          break; 
  
  else if(a<10){
     motor1.reverseRun(0);
     motor2.reverseRun(0);
  }
  
}
