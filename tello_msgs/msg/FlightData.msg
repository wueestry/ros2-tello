std_msgs/Header header

#######
# Raw string
#######

string raw

#######
# SDK version
#######

uint8 SDK_UNKNOWN=0
uint8 SDK_1_3=1
uint8 SDK_2_0=2
uint8 sdk

#######
# SDK 1.3+ fields
#######

int32 pitch   # pitch in degrees
int32 roll    # roll in degrees
int32 yaw     # yaw in degrees relative to startup orientation

int32 vx     # x velocity in m/s, +forward
int32 vy     # y velocity in m/s, +right
int32 vz     # z velocity in m/s, +down

int32 temp_low   # lowest temp in C
int32 temp_high   # highest temp in C

int32 tof     # time of flight distance in cm
int32 h       # height in cm

int32 battery     # battery %

float32 baro  # barometric altitude in m
int32 t    # time used by motor in seconds

# Acceleration includes gravity
float32 ax   # x acceleration in mm/s^2, +forward
float32 ay   # y acceleration in mm/s^2, +right
float32 az   # z acceleration in mm/s^2, +down

int32 mission_pad_id     # mission pad id, or -1 if not found
int32 x       # x coord relative to mission pad in cm, or 0 if not found
int32 y       # y coord relative to mission pad in cm, or 0 if not found
int32 z       # z coord relative to mission pad in cm, or 0 if not found
