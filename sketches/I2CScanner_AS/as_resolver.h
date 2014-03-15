#ifndef AS_RESOLVER_H_
#define AS_RESOLVER_H_

struct _solving_name {
  uint8_t addr;
  char * name;
};

// addresses and description taken from the ArduSatSDK
// https://github.com/ArduSat/ArduSatSDK
// NOTE : this array is stored in memory, but as it's only one sketch it doesn't really matter

struct _solving_name resolvarray[] = {
  { 0x0E, "MAG3110" },
  { 0x29, "TSL2561-1 (bottomplate camera)" },
  { 0x39, "TSL2561-2 (bottomplate split)" },
  { 0x48, "TMP102-1 (payload #1)" },
  { 0x49, "TMP102-2 (payload #2)" },
  { 0x4A, "TMP102-3 (bottomplate #1)" },
  { 0x4B, "TMP102-4 (bottomplate #2)" },
  { 0x53, "ADXL345" },
  { 0x1D, "ADXL345 !! on HIGH address (try grounding ADDR pin)" },
  { 0x69, "ITG3200" },
  { 0x68, "ITG3200 !! LOW address instead of HIGH (try to connect AD0 to HIGH)" },
  { 0x5A, "IR thermometer" },
  { 0x23, "Geiger" },
  { 0x22, "Spertruino" },
  { 0xFF, NULL}      // keep this line for the resolv() function to stop !
};

char * resolv(uint8_t addr) {
  struct _solving_name * t_ptr = resolvarray;
  while (t_ptr->addr != 0xFF) {
    if (t_ptr->addr == addr)
      return(t_ptr->name);
    t_ptr += 1;
  };
  return(NULL);
};  

#endif /* AS_RESOLVER_H_ */

