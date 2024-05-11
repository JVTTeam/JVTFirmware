// Firmware for JVT Virtual Touch
// Berkant Bakışlı & Eren Taşdemir

#define serial Serial

#define DIN 3
#define CLK 4
#define LE 5
#define CLR 8

#define ZONE_COUNT 15

#define SW_CH1 0b0000100000000000
#define SW_CH2 0b0000010000000000
#define SW_CH3 0b0000001000000000
#define SW_CH4 0b0000000100000000
#define SW_CH5 0b0000000010000000
#define SW_CH6 0b0000000001000000
#define SW_CH7 0b0000000000100000
#define SW_CH8 0b0000000000010000
#define SW_CH9 0b0000000000001000
#define SW_CH10 0b0000000000000100
#define SW_BASE1 0b0000000000000010
#define SW_BASE2 0b0000000000000001
#define SW_CATHODIC 0b1010000000000000
#define SW_ANODIC 0b0101000000000000

#define THUMB_DISTAL (SW_CATHODIC | SW_CH1 | SW_BASE1)
#define THUMB_PROXIMAL (SW_ANODIC | SW_CH1 | SW_BASE1)
#define INDEX_DISTAL (SW_CATHODIC | SW_CH2 | SW_BASE1)
#define INDEX_MIDDLE (SW_ANODIC | SW_CH2 | SW_BASE1)
#define INDEX_PROXIMAL (SW_ANODIC | SW_CH3 | SW_BASE1)
#define MIDDLE_DISTAL (SW_CATHODIC | SW_CH4 | SW_BASE1)
#define MIDDLE_MIDDLE (SW_ANODIC | SW_CH4 | SW_BASE1)
#define MIDDLE_PROXIMAL (SW_ANODIC | SW_CH5 | SW_BASE1)
#define RING_DISTAL (SW_CATHODIC | SW_CH6 | SW_BASE2)
#define RING_MIDDLE (SW_ANODIC | SW_CH6 | SW_BASE2)
#define RING_PROXIMAL (SW_ANODIC | SW_CH7 | SW_BASE2)
#define PINKY_DISTAL (SW_CATHODIC | SW_CH8 | SW_BASE2)
#define PINKY_MIDDLE (SW_ANODIC | SW_CH8 | SW_BASE2)
#define PINKY_PROXIMAL (SW_ANODIC | SW_CH9 | SW_BASE2)
#define PALM1 (SW_CATHODIC | SW_CH10 | SW_BASE1 | SW_BASE2)
#define PALM2 (SW_ANODIC | SW_CH10 | SW_BASE1 | SW_BASE2)

#define PERIOD(f) ((unsigned long)((1.0f / f) * 1000000))  // Period in microseconds

uint16_t frequency = 100;
uint16_t pulse_width = 400;
uint16_t strengths[ZONE_COUNT];
const uint16_t switch_configurations[ZONE_COUNT] = {
  THUMB_DISTAL,
  THUMB_PROXIMAL,
  INDEX_DISTAL,
  INDEX_MIDDLE,
  INDEX_PROXIMAL,
  MIDDLE_DISTAL,
  MIDDLE_MIDDLE,
  MIDDLE_PROXIMAL,
  RING_DISTAL,
  RING_MIDDLE,
  RING_PROXIMAL,
  PINKY_DISTAL,
  PINKY_MIDDLE,
  PINKY_PROXIMAL,
  PALM1
};
unsigned long last_loop_time;

void configureSwitch(uint16_t config) {
  //Ensure timings are correct
  digitalWrite(CLK, LOW);
  shiftOut(DIN, CLK, LSBFIRST, lowByte(config));
  shiftOut(DIN, CLK, LSBFIRST, highByte(config));

  digitalWrite(LE, LOW);
  delayMicroseconds(1);  //t_WLE 12ns (probably can be removed)
  digitalWrite(LE, HIGH);
  digitalWrite(CLK, LOW);
}

void setup() {
  pinMode(DIN, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(LE, OUTPUT);
  pinMode(CLR, OUTPUT);

  // Turn all switches off
  digitalWrite(CLR, HIGH);

  // Hold previous state
  digitalWrite(LE, HIGH);
  digitalWrite(CLR, LOW);

  serial.begin(115200);
  while (!serial)
    ;
}

void loop() {
  if (serial.available() > 0)  // Command received
  {
    char serial_buffer[150];
    size_t size = serial.readBytesUntil('\n', serial_buffer, 149);
    serial_buffer[size] = '\0';

    memset(strengths, 0, ZONE_COUNT * sizeof(uint16_t));

    char *p = serial_buffer;
    uint16_t strength = 0;
    while (*p != '\0') {
      switch (*p) {
        case 'A':  //Compatibility mode (IJ)
          strength = atoi(p + 1);
          strength = constrain(strength, 0, 1023);
          strengths['I' - 'I'] = strength;
          strengths['J' - 'I'] = strength;
          break;
        case 'B':  //Compatibility mode (KLM)
          strength = atoi(p + 1);
          strength = constrain(strength, 0, 1023);
          strengths['K' - 'I'] = strength;
          strengths['L' - 'I'] = strength;
          strengths['M' - 'I'] = strength;
          break;
        case 'C':  //Compatibility mode (NOP)
          strength = atoi(p + 1);
          strength = constrain(strength, 0, 1023);
          strengths['N' - 'I'] = strength;
          strengths['O' - 'I'] = strength;
          strengths['P' - 'I'] = strength;
          break;
        case 'D':  //Compatibility mode (QRS)
          strength = atoi(p + 1);
          strength = constrain(strength, 0, 1023);
          strengths['Q' - 'I'] = strength;
          strengths['R' - 'I'] = strength;
          strengths['S' - 'I'] = strength;
          break;
        case 'E':  //Compatibility mode (TUV)
          strength = atoi(p + 1);
          strength = constrain(strength, 0, 1023);
          strengths['T' - 'I'] = strength;
          strengths['U' - 'I'] = strength;
          strengths['V' - 'I'] = strength;
          break;
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
          strength = atoi(p + 1);
          strengths[*p - 'I'] = constrain(strength, 0, 1023);
          break;
        case 'X':
          frequency = atoi(p + 1);
          frequency = constrain(atoi(p + 1), 1, 1000);
          break;
        case 'Y':
          pulse_width = atoi(p + 1);
          pulse_width = constrain(atoi(p + 1), 0, 800);
          break;
      }
      p++;
    }
  }

  // Stimulation cycle
  if (micros() < last_loop_time + PERIOD(frequency))
    return;

  last_loop_time = micros();

  analogWrite(A0, 0);
  for (int i = 0; i < ZONE_COUNT; i++) {
    if (strengths[i] > 0) {
      configureSwitch(switch_configurations[i]);
      analogWrite(A0, strengths[i]);
      delayMicroseconds(pulse_width);
      analogWrite(A0, 0);
    }
  }
}
