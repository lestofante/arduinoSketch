/*byte retry, phase=0;


byte MyFastwire::readBuf(byte device,byte address,byte *data,byte num)
{
  switch(phase){
    case 0:
      retry=2;
      TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO)  | (1<<TWSTA);
      phase++;
      break;
    case 1:
      byte twst=TWSR & 0xF8;
      if (twst != TW_START && twst != TW_REP_START)
        return 17;
      phase++;
      break;
    case 2:
      TWDR = device & 0xfe; // send device address to write
      TWCR = (1<<TWINT) | (1<<TWEN);
      phase++;
      break;
  }
  do
  {
    
    if (!waitInt()) return 16;
    twst=TWSR & 0xF8;
    if (twst != TW_START && twst != TW_REP_START) return 17;


    
    if (!waitInt()) return 18;
    twst=TWSR & 0xF8;
  } 
  while (twst == TW_MT_SLA_NACK && retry-- > 0);
  if (twst != TW_MT_SLA_ACK) return 19;

  TWDR = address; // send data to the previously addressed device
  TWCR = (1<<TWINT) | (1<<TWEN);
  if (!waitInt()) return 20;
  twst=TWSR & 0xF8;
  if (twst != TW_MT_DATA_ACK) return 21;




  /***//*

  retry=2;
  do
  {
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO) | (1<<TWSTA)  ; 
    if (!waitInt()) return 22;
    twst=TWSR & 0xF8;
    if (twst != TW_START && twst != TW_REP_START) return 23;

    TWDR = device|0x01; // send device address with the read bit (1)
    TWCR = (1<<TWINT) | (1<<TWEN);
    if (!waitInt()) return 24;
    twst=TWSR & 0xF8;
  } 
  while (twst == TW_MR_SLA_NACK && retry-- > 0);
  if (twst != TW_MR_SLA_ACK) return 25;

  for(uint8_t i=0;i<num;i++)
  {
    if (i==num-1)
      TWCR = (1<<TWINT) | (1<<TWEN) ;
    else
      TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
    if (!waitInt()) return 26;
    twst=TWSR & 0xF8;
    if (twst != TW_MR_DATA_ACK && twst != TW_MR_DATA_NACK) return twst;
    data[i]=TWDR;
  }

  return 0;

}*/
