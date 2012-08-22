#ifndef ITG3200_h
#define ITG3200_h

class ITG3200{

public:
  void begin(char i2c_address);
  char update(int timeElapsed);

private:
  char _i2c_address;
};

#endif


