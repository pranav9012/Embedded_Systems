#include <stdint.h>
#define ALIAS_BASE 0x22000000U
#define BIT_BAND_BASE 0x20000000U
// modify 7th bit of the memory location 0x2000_0200 using the normal/usual
// method and using its alias address

// Alias address = 0x2200_0000 +(32*(0x2000_0200 - 0x2000_0000)) + 7(4)
int main()
{
  // normal/usual way
  uint8_t *ptr = (uint8_t*)0x20000200;
  *ptr = 0xff;
  *ptr &= ~(1 << 7);
  
  *ptr = 0xff;
 // using bit band method
  uint8_t *alis_addr = (uint8_t*)(ALIAS_BASE +(32*(0x20000200 - BIT_BAND_BASE)) + 7*4);
  
  *alis_addr = 0;// clearing 7th bit of the data stored in the memory address
  return 0;
}
