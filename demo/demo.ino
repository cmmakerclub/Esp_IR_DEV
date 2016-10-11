#include <EEPROM.h>

#define IR_commanf_time 500000
#define DHTTYPE       DHT22
#define DHT_pin       13
#define oled_sck_pin  4
#define oled_sda_pin  5
#define IR_rx_pin     14
#define IR_tx_pin     12
#define User_pin      0

uint16_t pointer_x = 0;
typedef struct {
  float ir_freq = 0;
  float data_period = 0;
  uint32_t data[32] = {0};
} protocol_pattern; 

protocol_pattern tmp;

int8_t ir_state;
int8_t data_bit;
void dataIROut(void) {
  if (data_bit == 1) {
    digitalWrite(IR_tx_pin, ir_state);
    ir_state = 1 - ir_state;
  } else {
    ir_state = 0;
    digitalWrite(IR_tx_pin, ir_state);
  }
}

void IR_transmit(void) {
  pointer_x = 0;
  uint32_t  bit_data = 0x80000000;
      timer1_isr_init();
      timer1_attachInterrupt(dataIROut);
      timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);
      timer1_write((float)clockCyclesPerMicrosecond()  * (1000000.0f / tmp.ir_freq) * 0.5f);
      uint32_t rec_time1 = micros();
      uint32_t rec_time_back = rec_time1;
      float rec_time2 = 0;
      float rec_time3 = 0;
      uint32_t pointer_x = 0;
      uint8_t toggle = 1;
      while (pointer_x <   32 * 32) {
        rec_time1 = micros() - rec_time_back;
        if (rec_time1 - rec_time3 >= tmp.data_period / 2.0f) {
          rec_time3 += tmp.data_period / 2.0f;
          if (tmp.data[pointer_x / 32] & bit_data >> (pointer_x % 32)) {
            data_bit = 1;
          } else {
            data_bit = 0;
          }
          pointer_x++;
        }
      }
      timer1_detachInterrupt();
}


void eep_ir_load(int16_t command_NO) {
  int size_data = sizeof(protocol_pattern);
  uint8_t buffer_data[sizeof(protocol_pattern)] = {0};
  EEPROM.begin(512);
  for (int x = 0; x < size_data; x++)  buffer_data[x] = EEPROM.read(1 + (size_data * command_NO) + x);
  EEPROM.end();
  memcpy(&tmp, buffer_data, size_data);
}


void setup() {
  delay(100);

  pinMode(IR_rx_pin, INPUT_PULLUP);
  pinMode(IR_tx_pin, OUTPUT);
  pinMode(User_pin, INPUT_PULLUP);

  delay(100);
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Start");

}

void loop() {

  eep_ir_load(0);  // turn off 
  IR_transmit();
  delay(5000);
  

  eep_ir_load(1);  // turn on
  IR_transmit();
  delay(5000);

}
