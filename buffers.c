#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include <string.h>
#include <stdio.h> /* For printf() */
#include "dev/leds.h"

/* Log configuration */
#include "sys/log.h"
#include "sys/node-id.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Configuration */
#define SEND_INTERVAL (60 * CLOCK_SECOND)


/*---------------------------------------------------------------------------*/
PROCESS(buffer_process, "NullNet broadcast example");
AUTOSTART_PROCESSES(&buffer_process);


 struct Data {
      uint8_t color;
      uint8_t p_id;

  };
    

/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len,
  const linkaddr_t *src, const linkaddr_t *dest)
{
    static uint8_t l_p_id=0;

    struct Data d;

    memcpy(&d, data, sizeof(len));

    printf("Process id is %d\n",d.p_id);
	LOG_INFO("Recived Data from ");
    LOG_INFO_LLADDR(src);
    LOG_INFO("\n");    

    if(d.p_id == l_p_id+1) {

    switch (d.color) {
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


         nullnet_buf = (uint8_t *)&d;
         nullnet_len = sizeof(d);
         memcpy(nullnet_buf, &d, sizeof(d));
         nullnet_len = sizeof(d);
         NETSTACK_NETWORK.output(NULL);


         l_p_id++;


    }


  
}

PROCESS_THREAD(buffer_process, ev, data)
{
  static struct etimer periodic_timer;

  PROCESS_BEGIN();


  
  nullnet_set_input_callback(input_callback);

  etimer_set(&periodic_timer, SEND_INTERVAL);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
   
	LOG_INFO("\n");
	LOG_INFO("Sending Data to ");
    LOG_INFO_LLADDR(&linkaddr_node_addr);
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}
