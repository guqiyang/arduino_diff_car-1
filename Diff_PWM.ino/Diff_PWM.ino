#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>

ros::NodeHandle nh;

int vel_l = 0;       //
int vel_r = 0;
float m = 0;       // (0~100%) Percentage of PWM
float n = 0;       // (0~100%) Percentage of PWM
float T = 100;     // PWM at 10kHz

int ENA_L = 3;     // MOTOR_ENA_L at D3
int ENA_R = 7;     // MOTOR_ENA_R at D7
int FR_L = 4;      // MOTOR_FR_L  at D4
int FR_R = 8;      // MOTOR_FR_R  at D8
int MOTOR_L = 5;   // MOTOR_PWM_L at D5
int MOTOR_R = 6;   // MOTOR_PWM_R at D6

std_msgs::String str_msg;
std_msgs::Int16 int_msg;

void set_l_backwards()
{
  digitalWrite(FR_L, HIGH);
}

void set_l_forwards()
{
  digitalWrite(FR_L, LOW);
}

void set_r_backwards()
{
    digitalWrite(FR_R, LOW);
}

void set_r_forwards()
{
  digitalWrite(FR_R, HIGH);
}

void set_ena_lr()
{
  digitalWrite(ENA_L, LOW);
  digitalWrite(ENA_R, LOW);
}

void set_pwm()
{
  if (vel_l>=0)
  {
    m = vel_l;
    set_l_forwards();
  }
  else
  {
    m = -vel_l;
    set_l_backwards();
  }

  if (vel_r>=0)
  {
    n = vel_r;
    set_r_forwards();
  }
  else
  {
    n = -vel_r;
    set_r_backwards();
  }

  if (m == n)
  {
    if ((m==0||m==-0)&&(n==0||n==-0))
    {
      set_ena_lr();
      digitalWrite(MOTOR_R, LOW);
      digitalWrite(MOTOR_L, LOW);
      delayMicroseconds(T);
    }
    else
    {
      set_ena_lr();
      digitalWrite(MOTOR_L, HIGH);
      digitalWrite(MOTOR_R, HIGH);
      delayMicroseconds(m);
      digitalWrite(MOTOR_L, LOW);
      digitalWrite(MOTOR_R, LOW);
      delayMicroseconds(T-m);
    }

  }
  else if (m>n)
  {
    set_ena_lr();
    if (n==0)
    {
      digitalWrite(MOTOR_L, HIGH);
      delayMicroseconds(m);
      digitalWrite(MOTOR_L, LOW);
      delayMicroseconds(T-m);
    }
    else
    {
      digitalWrite(MOTOR_L, HIGH);
      digitalWrite(MOTOR_R, HIGH);
      delayMicroseconds(n);
      digitalWrite(MOTOR_R, LOW);
      delayMicroseconds(m-n);
      digitalWrite(MOTOR_L, LOW);
      delayMicroseconds(T-m);
    }

  }
  else if (m<n)
  {
    set_ena_lr();
    if (m==0)
    {
      digitalWrite(MOTOR_R, HIGH);
      delayMicroseconds(n);
      digitalWrite(MOTOR_R, LOW);
      delayMicroseconds(T-n);
    }
    else
    {
      digitalWrite(MOTOR_L, HIGH);
      digitalWrite(MOTOR_R, HIGH);
      delayMicroseconds(m);
      digitalWrite(MOTOR_L, LOW);
      delayMicroseconds(n-m);
      digitalWrite(MOTOR_R, LOW);
      delayMicroseconds(T-n);
    }
  }
  else
  {
    delayMicroseconds(T);
  }
}

void messageCb(const std_msgs::Int16 &msg)
{
  vel_l = msg.data;
  //int_msg.data = pwm;
}

void messageCb_(const std_msgs::Int16 &msg)
{
  vel_r = msg.data;
  //int_msg.data = pwm;
}

//ros::Publisher chatter("chatter", &str_msg);
//ros::Publisher pwm_("pwm_", &int_msg);
ros::Subscriber<std_msgs::Int16> sub("cmd_vel/left", &messageCb );
ros::Subscriber<std_msgs::Int16> sub_("cmd_vel/right", &messageCb_ );

char hello[13] = "Hello World!";

void setup()
{
  nh.initNode();

  pinMode(ENA_L,OUTPUT);
  pinMode(ENA_R,OUTPUT);
  pinMode(FR_L,OUTPUT);
  pinMode(FR_R,OUTPUT);
  pinMode(MOTOR_L,OUTPUT);
  pinMode(MOTOR_R,OUTPUT);

  //nh.advertise(chatter);
  //nh.advertise(pwm_);
  nh.subscribe(sub);
  nh.subscribe(sub_);
}

void loop()
{
  set_pwm();
  //pwm_.publish(&int_msg);
  nh.spinOnce();
}
