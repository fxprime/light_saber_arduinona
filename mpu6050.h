#define XAXIS 0
#define YAXIS 1
#define ZAXIS 2
float gyro[3];
float accel[3];
float gyroScaleFactor = radians(2000.0/32668.0);//32768.0 32730 , +-32756  32768.0
float accelScaleFactor = 9.80665;//9.81/8192.09 9.81 / 8205    

uint8_t gyroSamples = 0;
uint8_t gyroSamples2 = 0;
int16_t gyroRaw[3];
float gyroSum[3];
int16_t accelRaw[3];
float accelSum[3];

float AccXf,AccYf,AccZf;
float Accel[3] = {0.0,0.0,0.0};
float filteredAccel[3] = {0.0,0.0,0.0};
float c_acc_x,c_acc_y,c_acc_z;

float GyroXf,GyroYf,GyroZf;
float gyro_offsetX,gyro_offsetY,gyro_offsetZ,acc_offsetX,acc_offsetY,acc_offsetZ;
float GyroX,GyroY,GyroZ,GyroTemp,AccX,AccY,AccZ;
float GyroX2,GyroY2,GyroZ2;

#define HMC5883_Address 0x1E
int MagX,MagY,MagZ;
float MagXf,MagYf,MagZf;
float c_magnetom_x;
float c_magnetom_y;
float c_magnetom_z;

//sensor MPU6050 -------------------------------------
// MPU 6050 Registers
#define MPU6050_ADDRESS         0x68
#define MPUREG_WHOAMI           0x75
#define MPUREG_SMPLRT_DIV       0x19
#define MPUREG_CONFIG           0x1A
#define MPUREG_GYRO_CONFIG      0x1B
#define MPUREG_ACCEL_CONFIG     0x1C
#define MPUREG_FIFO_EN          0x23
#define MPUREG_INT_PIN_CFG      0x37
#define MPUREG_INT_ENABLE       0x38
#define MPUREG_INT_STATUS       0x3A
#define MPUREG_ACCEL_XOUT_H     0x3B
#define MPUREG_ACCEL_XOUT_L     0x3C
#define MPUREG_ACCEL_YOUT_H     0x3D
#define MPUREG_ACCEL_YOUT_L     0x3E
#define MPUREG_ACCEL_ZOUT_H     0x3F
#define MPUREG_ACCEL_ZOUT_L     0x40
#define MPUREG_TEMP_OUT_H       0x41
#define MPUREG_TEMP_OUT_L       0x42
#define MPUREG_GYRO_XOUT_H      0x43
#define MPUREG_GYRO_XOUT_L      0x44
#define MPUREG_GYRO_YOUT_H      0x45
#define MPUREG_GYRO_YOUT_L      0x46
#define MPUREG_GYRO_ZOUT_H      0x47
#define MPUREG_GYRO_ZOUT_L      0x48
#define MPUREG_USER_CTRL        0x6A
#define MPUREG_PWR_MGMT_1       0x6B
#define MPUREG_PWR_MGMT_2       0x6C
#define MPUREG_FIFO_COUNTH      0x72
#define MPUREG_FIFO_COUNTL      0x73
#define MPUREG_FIFO_R_W         0x74
// Configuration bits
#define BIT_SLEEP               0x40
#define BIT_H_RESET             0x80
#define BITS_CLKSEL             0x07
#define MPU_CLK_SEL_PLLGYROX    0x01
#define MPU_CLK_SEL_PLLGYROZ    0x03
#define MPU_EXT_SYNC_GYROX      0x02
#define BITS_FS_250DPS          0x00
#define BITS_FS_500DPS          0x08
#define BITS_FS_1000DPS         0x10
#define BITS_FS_2000DPS         0x18
#define BITS_FS_MASK            0x18
#define BITS_DLPF_CFG_256HZ  0x00// //Default settings LPF 256Hz/8000Hz sample
#define BITS_DLPF_CFG_188HZ         0x01
#define BITS_DLPF_CFG_98HZ          0x02
#define BITS_DLPF_CFG_42HZ          0x03
#define BITS_DLPF_CFG_20HZ          0x04
#define BITS_DLPF_CFG_10HZ          0x05
#define BITS_DLPF_CFG_5HZ           0x06
#define BITS_DLPF_CFG_2100HZ_NOLPF  0x07
#define BITS_DLPF_CFG_MASK          0x07
#define BIT_INT_ANYRD_2CLEAR    0x10
#define BIT_RAW_RDY_EN          0x01
#define BIT_I2C_IF_DIS          0x10
#define BIT_INT_STATUS_DATA     0x01
//sensor ---------------------

int A_X_MIN = -4000;    //
int A_X_MAX = 4000;     //
int A_Y_MIN = -4000;    //
int A_Y_MAX = 4000;     //
int A_Z_MIN = -4000;    //
int A_Z_MAX = 4000;     //

#define applyDeadband(value, deadband)  \
  if(fabs(value) < deadband) {          \
    value = 0.0;                        \
  } else if(value > 0.0){               \
    value -= deadband;                  \
  } else if(value < 0.0){               \
    value += deadband;                  \
  }
 
void mpu6050_initialize()
{
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPUREG_PWR_MGMT_1);    // Chip reset DEVICE_RESET 1
    Wire.write(BIT_H_RESET);//DEVICE_RESET
    Wire.endTransmission();  
    delay(10);// Startup delay      
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPUREG_PWR_MGMT_1);
    Wire.write(MPU_CLK_SEL_PLLGYROZ);//CLKSEL 3 (PLL with Z Gyro reference)
    Wire.endTransmission();    
    delay(5);
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPUREG_SMPLRT_DIV);// SAMPLE RATE
    Wire.write(0x00);//// Sample rate = 1kHz
    Wire.endTransmission();  
    delay(5);
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPUREG_CONFIG);
    Wire.write(BITS_DLPF_CFG_42HZ);//98 BITS_DLPF_CFG_188HZ, BITS_DLPF_CFG_42HZ, default DLPF_CFG = 0 => ACC bandwidth = 260Hz  GYRO bandwidth = 256Hz)
    Wire.endTransmission();  
    delay(5);   
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPUREG_GYRO_CONFIG);
    Wire.write(BITS_FS_2000DPS);//BITS_FS_1000DPS FS_SEL = 3: Full scale set to 2000 deg/sec,  BITS_FS_2000DPS
    Wire.endTransmission(); 
    delay(5);  
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPUREG_ACCEL_CONFIG);
    Wire.write(0x10);//0x10 = 1G=256, AFS_SEL=2 (0x00 = +/-2G)  1G = 16,384 //0x10 = 1G = 4096 ,//0x08 = +-4g
    Wire.endTransmission();
    delay(5);
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPUREG_INT_PIN_CFG);// enable I2C bypass for AUX I2C
    Wire.write(0x02);//I2C_BYPASS_EN=1
    Wire.endTransmission();     
}

void mpu6050_Gyro_Values()
{
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPUREG_GYRO_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(MPU6050_ADDRESS, 6);
     int i = 0;
     byte result[6];
  while(Wire.available())    
  { 
    result[i] = Wire.read(); 
    i++;
  }
  Wire.endTransmission();
    gyroRaw[XAXIS] = ((result[0] << 8) | result[1]);//-12     -3
    gyroRaw[YAXIS] = ((result[2] << 8) | result[3])*-1;//37      15
    gyroRaw[ZAXIS] = ((result[4] << 8) | result[5])*-1;//11    5
}  
void mpu6050_Accel_Values()
{
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPUREG_ACCEL_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(MPU6050_ADDRESS, 6);
    int i = 0;
    byte result[6];
  while(Wire.available())    
  { 
    result[i] = Wire.read(); 
    i++;
  }
  Wire.endTransmission();
    accelRaw[XAXIS] = ((result[0] << 8) | result[1])*-1;//+ 105 + 100 max=4054.46, min=-4149.48
    accelRaw[YAXIS] = ((result[2] << 8) | result[3]);// - 45 - 35 max=4129.57, min=-4070.64
    accelRaw[ZAXIS] = ((result[4] << 8) | result[5]);// + 150 + 170 max=4014.73 , min=-4165.13
  // adjust for  compass axis offsets/sensitivity differences by scaling to +/-1 range
  c_acc_x = ((float)(accelRaw[XAXIS] - A_X_MIN) / (A_X_MAX - A_X_MIN))*2.0 - 1.0;
  c_acc_y = ((float)(accelRaw[YAXIS] - A_Y_MIN) / (A_Y_MAX - A_Y_MIN))*2.0 - 1.0;
  c_acc_z = ((float)(accelRaw[ZAXIS] - A_Z_MIN) / (A_Z_MAX - A_Z_MIN))*2.0 - 1.0;
}

void mpu6050_readGyroSum() {
    mpu6050_Gyro_Values();
    gyroSum[XAXIS] += gyroRaw[XAXIS];
    gyroSum[YAXIS] += gyroRaw[YAXIS];
    gyroSum[ZAXIS] += gyroRaw[ZAXIS];
    gyroSamples++;
}
void mpu6050_Get_gyro()
{       
    // Calculate average
    GyroX = (gyroSum[XAXIS] / gyroSamples)*gyroScaleFactor - gyro_offsetX;
    GyroY = (gyroSum[YAXIS] / gyroSamples)*gyroScaleFactor - gyro_offsetY;
    GyroZ = (gyroSum[ZAXIS] / gyroSamples)*gyroScaleFactor - gyro_offsetZ;            
    // Reset SUM variables
    gyroSum[XAXIS] = 0;
    gyroSum[YAXIS] = 0;
    gyroSum[ZAXIS] = 0;
    gyroSamples2 = gyroSamples;
    gyroSamples = 0;            
}

void mpu6050_readAccelSum() {
    mpu6050_Accel_Values();
    accelSum[XAXIS] += c_acc_x;
    accelSum[YAXIS] += c_acc_y;
    accelSum[ZAXIS] += c_acc_z;  
}
void mpu6050_Get_accel()
{
    // Calculate average
     if(gyroSamples == 0){
      gyroSamples = 1;
    }
    AccX = (accelSum[XAXIS] / gyroSamples)*accelScaleFactor - acc_offsetX;
    AccY = (accelSum[YAXIS] / gyroSamples)*accelScaleFactor - acc_offsetY;
    AccZ = (accelSum[ZAXIS] / gyroSamples)*accelScaleFactor;          
    // Apply correct scaling (at this point accel reprensents +- 1g = 9.81 m/s^2)
    // Reset SUM variables
    accelSum[XAXIS] = 0;
    accelSum[YAXIS] = 0;
    accelSum[ZAXIS] = 0;        
}
