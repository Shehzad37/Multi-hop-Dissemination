
#include "contiki.h"
#include <random.h>
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h> /* For printf() */
#include "dev/leds.h"
#include "sys/node-id.h"
/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define SEND_INTERVAL (60 * CLOCK_SECOND)
struct Data {
      uint8_t color;
      uint8_t p_id;

  };

PROCESS(origin_process, "Origin Broadcast Process");
AUTOSTART_PROCESSES(&origin_process);





PROCESS_THREAD(origin_process, ev, data)
{
  static struct etimer periodic_timer;
//   static unsigned count = 0;
  
    


  PROCESS_BEGIN();

   


  static struct Data data;

  nullnet_buf = (uint8_t *)&data;
  nullnet_len = sizeof(data);
  
    data.p_id = 1;

  etimer_set(&periodic_timer, SEND_INTERVAL);

  while(1) {
          printf("p_id is %d", data.p_id);
		  LOG_INFO("\n");
          data.color = (random_rand()%3);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    
    memcpy(nullnet_buf, &data, sizeof(data));
    nullnet_len = sizeof(data);
	
	
	LOG_INFO("Sending Data to ");
	LOG_INFO_LLADDR(&linkaddr_node_addr);
	LOG_INFO("\n");
    NETSTACK_NETWORK.output(NULL);
    
    etimer_reset(&periodic_timer);



        data.p_id =  data.p_id+1;


    
    switch (data.color) {
        case 0:
            leds_off(LEDS_GREEN);
            leds_off(LEDS_YELLOW);
            leds_on(LEDS_RED);
            break;
        case 1:
            leds_off(LEDS_RED);
            leds_off(LEDS_YELLOW);
            leds_on(LEDS_GREEN);
            break;
        case 2:
            leds_off(LEDS_RED);
            leds_off(LEDS_GREEN);
            leds_on(LEDS_YELLOW);
            break;

        }


   

  }

  PROCESS_END();
}
