In this code, we want to verify the fingerprint module's password. For this, we are asked to use
UASRT. The board used is STM32F446RE. We used UASRT2 peripheral for communication. The Tx pin is
PA2 and Rx pin is PA3. The board is a little endian system and we are asked to first send higher
bytes followed by lower bytes. For this, we need to somehow arrange the package in Higher bytes
to lower bytes form. The transmitter code shouls be implemented inside the function given below.

void USART2_TRANSMIT(uint8_t * buff , uint8_t length)


For the receiver, we are receiving data byte by byte. We need to store the received data and do
some process according to the confirmation code.


The command and response is stored in a CSV file. There are 200 commands from user and the 
corresponding 200 responses. It is given that you able to read commands from CSV file. To store 
the 200 reponses we can use an array of Structures. You simply need to write the code to transmit
and receive the package and do some processing according if required.