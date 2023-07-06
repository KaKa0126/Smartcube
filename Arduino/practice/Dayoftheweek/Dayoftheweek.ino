int upload_year = 2022;
int upload_month = 10;
int upload_day = 11;
int upload_hour = 17; //24시 기준
int upload_minute = 12;
int upload_second = 0;
String dayofweek;

//https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
void Dayofweek()
{
    int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if ( upload_month < 3 )
    {
        upload_year -= 1;
    }
    int calc = (upload_year + upload_year/4 - upload_year/100 + upload_year/400 + t[upload_month-1] + upload_day) % 7;
    
    switch(calc){
    case 0 :
      dayofweek = "SUNDAY";
      break;
    case 1 :
      dayofweek = "MONDAY";
      break;
    case 2 :
      dayofweek = "TUESDAY";
      break;
    case 3 :
      dayofweek = "WEDNESDAY";
      break;
    case 4 :
      dayofweek = "THURSDAY";
      break;
    case 5 :
      dayofweek = "FRIDAY";
      break;
    case 6 :
      dayofweek = "SATURDAY";
      break;
  }
}

void setup() {
  Serial.begin(9600);
  Dayofweek();
  Serial.println(dayofweek);
}

void loop() {  
}
