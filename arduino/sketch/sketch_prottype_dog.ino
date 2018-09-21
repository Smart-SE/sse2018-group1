int time_m =0;
int startedMs = 0;
bool isStarted = false;
const int STARTING_MESURING_MS = 2000;
int standardX, standardY, standardZ;
const long ANALOG_PIN_X = 0;
const long ANALOG_PIN_Y = 1;
const long ANALOG_PIN_Z = 2;
const long EXCEED_RANGE = 10;
int mesuringStartedMs = 0;
int mesuringMs = 0;
const long INIT_STARTED_TIME = 0;
int exceededCount = 0;
const int OUT_PIN_NUM = 13;
const bool IS_DEBUG_MODE = true;
const int MESURING_INTERVAL_MS = 3000;
const int EXCEEDED_JUDGE_COUNT = 10;

void setup() {
  Serial.begin(9600);
  standardX = standardY = standardZ = 0;
  pinMode(OUT_PIN_NUM, OUTPUT); 
}

void loop() {
  startedMs = millis();

  // 始まりから2000msは傾きの基準を取得する時間.
  if (!isStarted) {
    standardX = analogRead(ANALOG_PIN_X);
    standardY = analogRead(ANALOG_PIN_Y);
    standardZ = analogRead(ANALOG_PIN_Z);
    
    if (IS_DEBUG_MODE) {
      Serial.print("standardX:");
      Serial.print(standardX);
      Serial.print(" standardY:");
      Serial.print(standardY);
      Serial.print(" standardZ:");
      Serial.println(standardZ);
    }

    if (startedMs > STARTING_MESURING_MS) isStarted = true;
    return;
  }

  long x, y, z;
  x = y = z = 0;
  x = analogRead(0);
  y = analogRead(1);
  z = analogRead(2);

  // 傾きが一定値を超えたか判定.
  bool isExeededX = standardX < x - EXCEED_RANGE || standardX > x + EXCEED_RANGE; 

  // millis()は0-32000msの間で変動するためマイナスの場合を考慮.
  bool isMinusMs = millis() < 0;

  // 一定時間内に何回傾きが基準を超えたかを計測する.
  if (isExeededX && (mesuringStartedMs == INIT_STARTED_TIME)) mesuringStartedMs = millis();  
  int mesuringMs = millis() - mesuringStartedMs;
  if ((!isMinusMs && mesuringMs < MESURING_INTERVAL_MS) || (isMinusMs && mesuringMs > MESURING_INTERVAL_MS)) {
    if (isExeededX) exceededCount++;
  } else {
    exceededCount = 0;
    mesuringStartedMs = 0;
  }

  // 一定回数を超えたら犬が動いていることにする.
  if (exceededCount > EXCEEDED_JUDGE_COUNT) {
    digitalWrite(OUT_PIN_NUM, HIGH);
  } else {
    digitalWrite(OUT_PIN_NUM, LOW);
  }

  if (IS_DEBUG_MODE) {
    Serial.print("X:");
    Serial.print(x);
    Serial.print(" Y:");
    Serial.print(y);
    Serial.print(" Z:");
    Serial.print(z);
    Serial.print(" exceededCount:");
    Serial.print(exceededCount);
    Serial.print(" startedMs:");
    Serial.println(startedMs);
  }
  
  delay(100);
}

