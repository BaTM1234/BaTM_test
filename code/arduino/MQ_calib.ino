/******************************************************************************
 * Author: BA.TM
 * Date: 8/2/2018
 * Project: Calibrate MQ ssensor and convert ADC input to PPM value 
 *****************************************************************************/
const int MQ_PIN=A0;                        /*define which analog input channel
                                             *you are going to use*/
int RL_VALUE=1;                             /*define the load resistance on the
                                             *board, in kilo ohms*/
/************************Hardware Related Macros******************************/
#define  MQ3_SENSOR                   0     /*define which analog input channel
                                             *you are going to use*/
#define  MQ5_SENSOR                   1
#define  MQ9_SENSOR                   2

#define  RO_CLEAN_AIR_FACTOR_MQ3      60    /*RO_CLEAR_AIR_FACTOR=(Sensor
                                             *resistance in clean air)/RO,*/
#define  RO_CLEAN_AIR_FACTOR_MQ5      6.5
#define  RO_CLEAN_AIR_FACTOR_MQ9      9.83

#define  RS_RO_RATIO_MIN_MQ3          0.15
#define  RS_RO_RATIO_MIN_MQ5          0.15
#define  RS_RO_RATIO_MIN_MQ9          0.15

/***********************Software Related Macros*******************************/
#define  CALIBRATION_SAMPLE_TIMES     50    /*define how many samples you are
                                             *going to take in the calibration 
                                             * phase*/
#define  CALIBRATION_SAMPLE_INTERVAL  50    /*define the time interal(in
                                             *milisecond) between each samples
                                             *in the*/
#define  READ_SAMPLE_INTERVAL         50    /*define how many samples you are
                                             *going to take in normal operation
                                             */
#define  READ_SAMPLE_TIMES            5     /*define the time interal(in
                                             *milisecond) between each samples 
                                             *in*/ 
                                             
/**********************Application Related Macros*****************************/
#define  GAS_LPG             0  
#define  GAS_CO              1  
#define  GAS_SMOKE           2
#define  GAS_C2H5OH          3      /*Alcohol, Ethanol*/
#define  GAS_C6H6            4      /*Benzene*/
#define  GAS_CH4             5
#define  GAS_NHEX            6      /*hexa*/
#define  GAS_H2              7

/*****************************Globals*****************************************/
float C2H5OH_Curve_MQ3[3] = {-1,0.36,-0.64};
float C6H6_Curve_MQ3[3]   = {-1,0.6,-0.35};
float CH4_Curve_MQ3[3]    = {-1,1.68,-0.05};
float NHEX_Curve_MQ3[3]   = {-1,1.73,-0.37};
float LPG_Curve_MQ3[3]    = {-1,1.72,-0.28};
float CO_Curve_MQ3[3]     = {-1,1.72,-0.26};

float H2_Curve_MQ5[3]     = {2.3,0.26,-0.26};
float LPG_Curve_MQ5[3]    = {2.3,-0.15,-0.39};
float CH4_Curve_MQ5[3]    = {2.3,-0.02,-0.39};
float CO_Curve_MQ5[3]     = {2.3,0.59,-0.12};
float C2H5OH_Curve_MQ5[3] = {2.3,0.54,-0.22};

float LPG_Curve_MQ9[3]    = {2.3,0.34,-0.48};
float CO_Curve_MQ9[3]     = {2.3,0.23,-0.48};
float CH4_Curve_MQ9[3]    = {2.3,0.5,-0.39};

float Ro = 10;             /*Ro is initialized to 10 kilo ohmss*/


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Calibrating...");
  Ro = MQCalibration(MQ_PIN, RO_CLEAN_AIR_FACTOR_MQ3); /*Calibrating the sensor. Please make sure
                                                        *the sensor is in clean air*/
  Serial.print("Ro=");
  Serial.println(Ro);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.print("ADC: ");
  Serial.println(analogRead(MQ_PIN));
  
  Serial.print("Ro=");
  Serial.println(Ro);

  Serial.print("Rs: ");
  Serial.println(MQRead(MQ_PIN));
  /*
  Serial.println("MQ5: ");
  Serial.print("H2: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_H2, MQ5_SENSOR));

  Serial.print("LPG: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_LPG, MQ5_SENSOR));

  Serial.print("CH4: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_CH4, MQ5_SENSOR));

  Serial.print("CO: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_CO, MQ5_SENSOR));

  Serial.print("C2H5OH: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_C2H5OH, MQ5_SENSOR));
  */
  Serial.println("MQ3: ");
  Serial.println("**********************************************************");

  
  
  Serial.print("C2H5OH: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_C2H5OH, MQ3_SENSOR));
/*
  Serial.print("C6H6: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_C6H6, MQ3_SENSOR));

  Serial.print("CH4: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_CH4, MQ3_SENSOR));

  Serial.print("NHEX: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_NHEX, MQ3_SENSOR));

  Serial.print("LPG: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_LPG, MQ3_SENSOR));
  
  Serial.print("CO: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_CO, MQ3_SENSOR));
  Serial.println("**********************************************************");
  */
/*
  Serial.println("MQ9: ");
  Serial.println("**********************************************************");
  Serial.print("LPG: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_LPG, MQ9_SENSOR));

  Serial.print("CO: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_CO, MQ9_SENSOR));

  Serial.print("CH4: ");
  Serial.println(MQGetGasPercentage(MQRead(MQ_PIN)/Ro, GAS_CH4, MQ9_SENSOR));
  Serial.println("**********************************************************");
  */
  delay(1000);
}

/****************** MQResistanceCalculation ***********************************
 * Input:   raw_adc - raw value read from adc, which represents the voltage
 * Output:  the calculated sensor resistance
 * Remarks: The sensor and the load resistor forms a voltage divider. Given
 *          the voltage across the load resistor and its resistance, the
 *          resistance of the sensor could be derived.
 ******************************************************************************/
float MQResistanceCalculation(int raw_adc)
{
  return (((float)RL_VALUE*(1024-raw_adc)/raw_adc));
}

/***************************** MQCalibration **********************************
 * Input:   mq_pin - analog channel
 * Output:  Ro of the sensor
 * Remarks: This function assumes that the sensor is in clean air. It use  
 *          MQResistanceCalculation to calculates the sensor resistance in 
 *          clean airand then divides it with RO_CLEAN_AIR_FACTOR.  
 *          RO_CLEAN_AIR_FACTOR is about 10, which differs slightly between
 *          different sensors.
 *****************************************************************************/ 
float MQCalibration(int mq_pin, int ro_clean_air_factor)
{
  int i;
  float val=0;

  for (i=0; i<CALIBRATION_SAMPLE_TIMES; i++) {            //take multiple samples
    val += MQResistanceCalculation(analogRead(mq_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/CALIBRATION_SAMPLE_TIMES;                   //calculate the average value
  val = val/ro_clean_air_factor;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro
                                         
  return val;                                           //according to the chart in the datasheet 
}

/*****************************  MQRead ****************************************
 * Input:   mq_pin - analog channel
 * Output:  Rs of the sensor
 * Remarks: This function use MQResistanceCalculation to caculate the sensor
 *          resistenc (Rs). The Rs changes as the sensor is in the different
 *          consentration of the target gas. The sample times and the time
 *          interval between samples could be configured by changing the
 *          definition of the macros.
 *****************************************************************************/
float MQRead(int mq_pin)
{
  int i;
  float rs=0;
 
  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }
  rs = rs/READ_SAMPLE_TIMES;
 
  return rs;
}

/*****************************  MQGetGasPercentage ****************************
 * Input:   rs_ro_ratio - Rs divided by Ro
 *          gas_id      - target gas type
 * Output:  ppm of the target gas
 * Remarks: This function passes different curves to the MQGetPercentage
 *          function which calculates the ppm (parts per million) of the target
 *          gas.
 *****************************************************************************/
long MQGetGasPercentage(float rs_ro_ratio, int gas_id, int sensor_id)
{
  if (sensor_id == MQ3_SENSOR ){
    if ( gas_id == GAS_C2H5OH ){
      return MQGetPercentage(rs_ro_ratio, C2H5OH_Curve_MQ3);
    } else if ( gas_id == GAS_C6H6 ){
      return MQGetPercentage(rs_ro_ratio, C6H6_Curve_MQ3);
    } else if ( gas_id == GAS_CH4 ){
     return MQGetPercentage(rs_ro_ratio, CH4_Curve_MQ3);
    } else if ( gas_id == GAS_NHEX ){
     return MQGetPercentage(rs_ro_ratio, NHEX_Curve_MQ3);
    } else if ( gas_id == GAS_LPG ){
     return MQGetPercentage(rs_ro_ratio, LPG_Curve_MQ3);
    } else if ( gas_id == GAS_CO ){
     return MQGetPercentage(rs_ro_ratio, CO_Curve_MQ3);
    }
  } else if (sensor_id == MQ5_SENSOR ){
    if (rs_ro_ratio < RS_RO_RATIO_MIN_MQ5){
      rs_ro_ratio = RS_RO_RATIO_MIN_MQ5;
    }
    if ( gas_id == GAS_H2 ){
      return MQGetPercentage(rs_ro_ratio, H2_Curve_MQ5);
    } else if ( gas_id == GAS_LPG ){
      return MQGetPercentage(rs_ro_ratio, LPG_Curve_MQ5);
    } else if ( gas_id == GAS_CH4 ){
     return MQGetPercentage(rs_ro_ratio, CH4_Curve_MQ5);
    } else if ( gas_id == GAS_CO ){
     return MQGetPercentage(rs_ro_ratio, CO_Curve_MQ5);
    } else if ( gas_id == GAS_C2H5OH ){
     return MQGetPercentage(rs_ro_ratio, C2H5OH_Curve_MQ5);
    }
  }  else if (sensor_id == MQ9_SENSOR ){
    if (rs_ro_ratio < RS_RO_RATIO_MIN_MQ9){
      rs_ro_ratio = RS_RO_RATIO_MIN_MQ9;
    }
    if ( gas_id == GAS_LPG ){
      return MQGetPercentage(rs_ro_ratio, LPG_Curve_MQ9);
    } else if ( gas_id == GAS_CH4 ){
     return MQGetPercentage(rs_ro_ratio, CH4_Curve_MQ9);
    } else if ( gas_id == GAS_CO ){
     return MQGetPercentage(rs_ro_ratio, CO_Curve_MQ9);
    }
  }

  return 0;
}

/*****************************  MQGetPercentage *******************************
 * Input:   rs_ro_ratio - Rs divided by Ro
 *          pcurve      - pointer to the curve of the target gas
 * Output:  ppm of the target gas
 * Remarks: By using the slope and a point of the line. The x(logarithmic value
 *          of ppm) of the line could be derived if y(rs_ro_ratio) is provided.
 *          As it is a logarithmic coordinate, power of 10 is used to convert
 *          the result to non-logarithmic value.
 *****************************************************************************/
long MQGetPercentage(float rs_ro_ratio, float *pcurve)
{
  return (pow(10,(((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}
