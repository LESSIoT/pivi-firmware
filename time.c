
#include "time.h"

void test_ovf_interrupt_callback(void)
{
	gpio_toggle_pin(LED0_GPIO);
}

void configure_timer(volatile void *TIMER)
{
	tc_set_wgm(TIMER, TC_WG_NORMAL);
	tc_write_period(TIMER, TIMER_PERIOD);
	tc_set_overflow_interrupt_level(TIMER, TC_INT_LVL_LO);
	tc_set_resolution(TIMER, TIMER_PERIOD);
}

void time_init(void)
{
	pmic_init();
    cpu_irq_enable();
    sleepmgr_init();
    time_start();
    configure_timer(I_TIMER);
    configure_timer(V_TIMER);
}


void time_start(void)
{
	tc_enable(I_TIMER);
	tc_enable(V_TIMER);

	tc_set_overflow_interrupt_callback(I_TIMER, test_ovf_interrupt_callback);
}
