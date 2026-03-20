#include <Arduino.h>
#include <Adafruit_NeoPixel.h>  
#include <math.h>   

#define LED_NUM     2   
#define LED_PIN     8   

// GPIO pin connected to the gas sensor analog output
#define GAS_ANALOG_PIN    17      

// Number of samples used when reading voltage
#define VOLTAGE_SAMPLE_COUNT  2    

// Number of samples used when calibrating R0
#define CALIBRATION_SAMPLE_COUNT 10 

// Sensor supply voltage (usually 5V)
#define VCC             5.0f      

// Load resistor RL value (kΩ), must match the hardware module
#define LOAD_RESISTOR    4.7f      

// Gas concentration formula coefficients (depend on sensor type and target gas)
#define COEFF_A          11.5428f
#define COEFF_B          0.6549f
#define COEFF_C          100.0f

// Gas concentration alarm threshold (ppm)
#define ALARM_THRESHOLD_PPM  5000      // Set threshold to 10000 ppm

// Global structure storing sensor parameters and latest data
struct {
  float gas_voltage;   // Latest measured sensor voltage (V)
  float RL;            // Load resistor value (kΩ)
  float R0;            // Baseline resistance in clean air (kΩ)
} gas_data;

// NeoPixel LED strip object
Adafruit_NeoPixel strip(LED_NUM, LED_PIN, NEO_GRB + NEO_KHZ800);

// Initialize gas sensor settings
void gas_init() {
  
  // Set ADC attenuation to measure up to ~3.3V
  analogSetPinAttenuation(GAS_ANALOG_PIN, ADC_11db);
  
  // Initialize sensor parameters
  gas_data.RL = LOAD_RESISTOR;
  gas_data.R0 = 0.0;          
  gas_data.gas_voltage = 0.0;
}

/*
 * Read gas sensor voltage with multiple sampling and averaging
 * Return 1 when a full sample cycle is completed, otherwise 0
 */
int get_gas_voltage() {
  static int vol_sum = 0;           
  static uint8_t vol_reading_cnt = 0;
  
  // Read calibrated millivolt value from ADC
  int raw_mv = analogReadMilliVolts(GAS_ANALOG_PIN);
  
  vol_sum += raw_mv;
  vol_reading_cnt++;
  
  if (vol_reading_cnt == VOLTAGE_SAMPLE_COUNT) {
    
    // Compute average voltage and convert to volts
    float avg_mv = (float)vol_sum / vol_reading_cnt;
    gas_data.gas_voltage = avg_mv / 1000.0f;   
    
    // Reset sampling counters
    vol_sum = 0;
    vol_reading_cnt = 0;
    
    return 1;   
  }
  
  // Sampling still in progress
  return 0;
}

// Perform R0 calibration in clean air
float get_r0_calibration() {
  static uint8_t vol_reading_cnt = 0;
  static float vol_sum = 0;
  
  while (1) {
    
    // Wait until one voltage sample cycle finishes
    if (get_gas_voltage()) {   
      vol_sum += gas_data.gas_voltage;   
      vol_reading_cnt++;
    }
    
    if (vol_reading_cnt >= CALIBRATION_SAMPLE_COUNT) {
      
      // Calculate average voltage
      float Vavg = vol_sum / vol_reading_cnt;
      
      // Compute R0 value
      // R0 = ((Vcc - Vavg) * RL) / Vavg
      gas_data.R0 = ((VCC - Vavg) * gas_data.RL) / Vavg;
      
      // Reset counters
      vol_reading_cnt = 0;
      vol_sum = 0;
      
      break;
    }
    
    delay(100);   
  }
  
  return gas_data.R0;
}

// Calculate gas concentration in ppm
float get_gas_data() {
  float RS = 0;
  float ppm = 0;
  
  // Wait until voltage sampling completes
  while (!get_gas_voltage()) {
    delay(10);
  }
  
  // Calculate sensor resistance RS (kΩ)
  // RS = ((Vcc - Vout) * RL) / Vout
  RS = ((VCC - gas_data.gas_voltage) * gas_data.RL) / gas_data.gas_voltage;
  
  // Calculate gas concentration
  // ppm = ((A * R0) / RS) ^ B * C
  ppm = pow(((COEFF_A * gas_data.R0) / RS), COEFF_B) * COEFF_C;
  
  return ppm;
}


void led_on() {
  strip.setPixelColor(0, strip.Color(255, 255, 255)); 
  strip.setPixelColor(1, strip.Color(255, 255, 255)); 
  strip.show();
}

void led_off() {
  strip.setPixelColor(0, strip.Color(0, 0, 0));
  strip.setPixelColor(1, strip.Color(0, 0, 0));
  strip.show();
}


void setup() {
  Serial.begin(115200);

  // Initialize sensor
  gas_init();
  
  // Perform baseline calibration
  Serial.println("Calibrating R0 in clean air...");
  float r0 = get_r0_calibration();
  
  Serial.print("R0 = ");
  Serial.print(r0);
  Serial.println(" kOhm");
}

void loop() {
  
  static float last_ppm_data = 0;       
  static bool led_state = false;        
  static unsigned long last_toggle = 0; 

  unsigned long now = millis();


  // Get gas concentration
  float ppm_data = get_gas_data();

  // Alarm logic: Turn on LED when gas level exceeds threshold (10000 ppm)
  if (ppm_data >= ALARM_THRESHOLD_PPM) {
    led_on();  // Turn on LED
  } else {
    led_off();  // Turn off LED
  }

  // Print gas concentration
  Serial.print("Gas ppm = ");
  Serial.println(ppm_data, 2);

  // Sampling interval
  delay(20);
}