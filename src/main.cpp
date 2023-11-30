/**
 * \file
 * \brief Файл с точкой входа _tmain
 * Логика работы реализована в данном файле
 *
 */

#include "stdio.h"
#include <iostream>
#include <fstream>
#include <modbus.h>
#include "modbus-rtu-private.h"
#include "modbus-private.h"

using namespace std;

modbus_t *mb;
modbus_rtu_t *ctx_rtu;

int main(int argc, char *argv[])
{
  uint16_t devnull; // мусорка для dummy read

  mb = modbus_new_rtu(argv[1], 9600, 'N', 8, 1); // Параметры из командной строки

  ctx_rtu = (modbus_rtu_t *)(mb->backend_data);

#ifdef DEBUG
    modbus_set_debug(mb, TRUE);
#endif

  if (mb == NULL)
  {
    cerr << "ERR1" << endl;
    exit(EXIT_FAILURE);
  }

  if (modbus_connect(mb) != 0)
  {
    cerr << "ERR2" << endl;
    modbus_free(mb);
    exit(EXIT_FAILURE);
  }

    modbus_set_slave(mb, 10); // Задать слейв-id
    modbus_set_response_timeout(mb,1,0);

    uint16_t answer = 0;
  
    
    if(modbus_read_registers(mb, 30005, 1, &answer) < 0)
    {
      cerr << "ERR3" << endl;
      modbus_close(mb);
      modbus_free(mb);
      exit(EXIT_FAILURE);
    }

    if(answer>120) // If we are using grid power
    {
      // Show grid voltage
      cout << answer/10 << "B";
    }
    else
    {
      // Check and show accumulator voltage
      if(modbus_read_registers(mb, 30014, 1, &answer) < 0)
      {
        cerr << "ERR4" << endl;
        modbus_close(mb);
        modbus_free(mb);
        exit(EXIT_FAILURE);
      }

      cout << answer/10 << "," << answer%10 << "B";
    }

    modbus_close(mb);
    modbus_free(mb);
    exit(EXIT_SUCCESS);

  return 0;
}
