#define uen 4
#define mgf 5
#define mgb 3
#define mdb 9
#define mdf 6
#define an A2
#define j 8
int bouton = 12;
int setpoint = 35;
int kp = 15;
int ki = 0;
int PWM_min = 20;
int PWM_max = 255;
int BS = 150;
int PWM_RIGHT = 0;
int PWM_LEFT = 0;
int total_error = 0;
int error = 0;
int etat = 0;
int bloc = 1;
int coef[8] = {-8, -6, -4, -2, 2, 4, 6, 8};
bool first = false;
int count = 0;
bool car = false;
int a5ermaraIssar = 0;

int compt = 0;

long int mariem = 0;
// variables
char b = 0x03;
char rchar;
int etape = 0;

int values[8];

bool verif = true;
int vit1 = 120;

bool test = false;
int maxCompt = 1000;

void junction_width(char a)
{
    char adr = 0x02;
    char cmd = 0x4A;
    char data = a;
    char check = adr + cmd + data;
    Serial.write(adr);
    Serial.write(cmd);
    Serial.write(data);
    Serial.write(check);
}
// see sensor manual for different configuration

void sensor_config(char address, char command, char data)
{
    char checksum = address + command + data;
    Serial.write(address);
    Serial.write(command);
    Serial.write(data);
    Serial.write(checksum);
}

void forward()
{
    analogWrite(mdf, PWM_RIGHT);
    analogWrite(mdb, 0);
    analogWrite(mgf, PWM_LEFT);
    analogWrite(mgb, 0);
}

void backward()
{
    analogWrite(mdf, 0);
    analogWrite(mdb, 80);
    analogWrite(mgf, 0);
    analogWrite(mgb, 80);
}

void right()
{
    analogWrite(mdf, 0);
    analogWrite(mdb, 120);
    analogWrite(mgf, 120);
    analogWrite(mgb, 0);
}

void left()
{
    analogWrite(mdf, 120);
    analogWrite(mdb, 0);
    analogWrite(mgf, 0);
    analogWrite(mgb, 120);
}

void right2()
{
    analogWrite(mdf, 0);
    analogWrite(mdb, 0);
    analogWrite(mgf, 50);
    analogWrite(mgb, 0);
}

void left2()
{
    analogWrite(mdf, 50);
    analogWrite(mdb, 0);
    analogWrite(mgf, 0);
    analogWrite(mgb, 0);
}

void leftTaw5ir()
{
    analogWrite(mdf, 0);
    analogWrite(mdb, 0);
    analogWrite(mgf, 0);
    analogWrite(mgb, 50);
}

void pause()
{
    analogWrite(mdf, 0);
    analogWrite(mdb, 0);
    analogWrite(mgf, 0);
    analogWrite(mgb, 0);
}
void junction()
{

    if (digitalRead(j))
    {
        etape++;
        // if(rchar!=35) etape--;
        while (digitalRead(j))
        {

            if (rchar >= 30 && rchar <= 39)
            {
                forward();
            }
            else if (rchar >= 0 && rchar <= 29)
            {
                left();
            }
            else if (rchar >= 40 && rchar <= 70)
            {
                right();
            }
            else
            {
                pause();
            }
        } /*move forward*/;
    }
}
void jwchange()
{
    switch (etape)
    {
    case 1:
        junction_width(0x05) /*junction_width(char a)*/;
    case 2:
        junction_width(0x07) /*junction_width(char a)*/;
    case 3:
        junction_width(0x05) /*junction_width(char a)*/;
    case 4: /*junction_width(char a)*/;
    }
}
void junction2()
{
    if (digitalRead(j) && rchar > 5 && rchar <= 15)
        right();
    if (digitalRead(j) && rchar >= 0 && rchar <= 5)
        forward();
}
void readValues()
{
    for (int i = 0; i < 8; i++)
    {
        bool sensorState = rchar & (1 << i); // Check the state of sensor 'i'
        if (sensorState)
        {
            values[i] = 1;
        }
        else
        {
            values[i] = 0;
        }
    }
    // Serial.println(rchar,BIN);
    // mariem = values[0]*10000000+values[1]*1000000+values[2]*100000+values[3]*10000+values[4]*1000+values[5]*100+values[6]*10+values[7] ;
}

void PID()
{
    int error = 0;
    for (int i = 0; i < 8; i++)
    {
        error = error + values[i] * coef[i];
    }

    int PWM = kp * error;
    // Serial.println(error);
    PWM_RIGHT = BS - PWM;
    PWM_LEFT = BS + PWM;
    if (PWM_RIGHT > PWM_max)
        PWM_RIGHT = PWM_max;
    if (PWM_RIGHT < PWM_min)
        PWM_RIGHT = PWM_min;
    if (PWM_LEFT > PWM_max)
        PWM_LEFT = PWM_max;
    if (PWM_LEFT < PWM_min)
        PWM_LEFT = PWM_min;

    if (bloc == 3)
    {
        //  if(verif == false && values[0]==0 && values[7]==0  ){
        //    etat = 0;
        //  }
        vit1 = 80;
        PWM_RIGHT = 50;
        PWM_LEFT = 50;

        //  pause();
        //  delay(500);
        // leftTaw5ir();
        //  delay(400);
        //  forward();
        //  delay(400);

        // if(compt>maxCompt){
        //   if(values[6]==1 || values[7]==1){
        //     right();
        //   }
        // }

        //**********************************left**********************************
        if (values[2] == 1 && values[3] == 0 && values[4] == 0 && values[5] == 0)
        {
            left2();
        }
        else if (values[2] == 1 && values[3] == 1 && values[4] == 0 && values[5] == 0)
        {
            left2();
        }
        else if (values[2] == 0 && values[3] == 1 && values[4] == 0 && values[5] == 0)
        {
            left2();
        }
        else if (values[2] == 1 && values[3] == 1 && values[4] == 1 && values[5] == 0)
        {
            left2();
        }
        //**********************************right**********************************
        else if (values[2] == 0 && values[3] == 0 && values[4] == 0 && values[5] == 1)
        {
            right2();
        }
        else if (values[2] == 0 && values[3] == 0 && values[4] == 1 && values[5] == 1)
        {
            right2();
        }
        else if (values[2] == 0 && values[3] == 0 && values[4] == 1 && values[5] == 0)
        {
            right2();
        }
        else if (values[2] == 0 && values[3] == 1 && values[4] == 1 && values[5] == 1)
        {
            right2();
        }
        //**********************************forward**********************************
        else if (values[2] == 0 && values[3] == 1 && values[4] == 1 && values[5] == 0)
        {
            forward();
        }
        else if (values[2] == 1 && values[3] == 1 && values[4] == 1 && values[5] == 1)
        {
            forward();
        }

        /* else if(verif==true){
       forward();}*/

        else if (values[0] == 0 && values[1] == 0 && values[2] == 0 && values[3] == 0 && values[4] == 0 && values[5] == 0 && values[6] == 0 && values[7] == 0)
        {
            forward();
            // pause();
            // delay(1000);
        }

        // if(values[0]==0 && values[1]==0 && values[3]==1 && values[4]==1  && values[6]==0 && values[7]==0 ){
        if (values[7] == 0)
        {

            compt++;
        }
        else
        {
            if (compt <= maxCompt)
            {
                compt = 0;
            }
        }

        // for(int i =0 ; i<  8 ; i++){
        //    Serial.print(values[i]);
        // }
        //
        // Serial.println();

        //
        // if(error>0){
        //    left2();
        //  }
        //  else if(error<0){
        //    right2();
        //  }
        //
        //  else{
        //    forward();
        //  }
        //
        // pause();
        // delay(100);
        //  PWM_RIGHT = 50 ;
        //  PWM_LEFT = 50 ;
        coef[0] = 0;
        // coef[1]=0;
        // coef[6]=0;
        coef[7] = 0;
        //
        //   if(error>0){
        //    left2();
        //  }
        //  else if(error<0){
        //    right2();
        //  }
        //
        //  else{
        //    forward();
        //  }
    }
    else
    {
        forward();
    }
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(bouton, INPUT_PULLUP);
    pinMode(uen, OUTPUT);
    pinMode(mgf, OUTPUT);
    pinMode(mdf, OUTPUT);
    pinMode(mgb, OUTPUT);
    pinMode(mdb, OUTPUT);
    pinMode(j, INPUT);
    pinMode(an, INPUT);
    pinMode(uen, OUTPUT);

    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
}

void loop()
{

    while (digitalRead(bouton) == 0)
    {
    };

    digitalWrite(uen, LOW);
    while (Serial.available() <= 0)
        ;
    rchar = Serial.read();
    readValues();
    digitalWrite(uen, HIGH);
    // Serial.println(rchar,BIN);

    if (values[0] == 1)
    {
        etat = 1;
    }
    if (values[7] == 1)
    {
        etat = 2;
    }
    verif = true;
    for (int i = 0; i < 8; i++)
    {
        if (values[i] == 1)
        {
            verif = false;
        }
    }

    // if((compt >maxCompt && bloc==3)){
    //          pause();
    //          delay(1000);
    //        }
    if (verif == true)
    {
        // left2();
        coef[0] = -8;
        coef[1] = -6;

        if (first)
        {

            if (bloc == 1)
            {
                etat = 0;
                //          pause();
                //        delay(1000);
            }
            if (bloc == 1)
            {
                bloc = 2;
            }
        }

        if (true)
        {
            if ((compt > maxCompt && bloc == 3))
            {
                bloc = 1;
                // forward();
            }
            else
            {
                if (etat == 1)
                {
                    left();
                }
                else if (etat == 2)
                {
                    right();
                }
                else if (etat == 0)
                {
                    forward();
                }
            }
        }
    }
    else
    {
        PID();
    }

    /* if(values[0]==0 && values[7]==0 && verif == false){
       etat=0;}*/
    if (values[3] == 1 && values[4] == 1 && values[5] == 1 && values[6] == 1 && values[7] == 1)
    {
        if (!first)
        {
            coef[0] = -2;
            coef[1] = -2;
        }

        first = true;
    }

    if (bloc == 2)
    {
        if (values[0] == 1 && values[1] == 1 && values[2] == 1 && values[3] == 1 && values[4] == 0 && values[6] == 0 && values[5] == 0)
        {
            a5ermaraIssar = millis();
        }

        if (values[7] == 1 && (millis() - a5ermaraIssar) < 1000 && (count > 6))
        {

            bloc = 3;
            digitalWrite(13, HIGH);

            leftTaw5ir();
            delay(500);
        }
        if (values[2] == 1 && values[1] == 1 && values[3] == 1)
        {
            car = true;
        }
        if (car && values[1] == 0)
        {
            count += 1;
            car = false;
        }
        //         if(count == 6){
        //          pause();
        // 0          delay(1000);}
    }
}