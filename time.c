
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
}

void time_set_V_callback(tc_callback_t callback)
{
	tc_set_overflow_interrupt_callback(V_TIMER, callback);
}

void time_set_I_callback(tc_callback_t callback)
{
	tc_set_overflow_interrupt_callback(I_TIMER, callback);
}

void time_start_timers(uint16_t v_delay)
{
	tc_enable(V_TIMER);
	tc_enable(I_TIMER);
    configure_timer(I_TIMER);
    configure_timer(V_TIMER);
    tc_write_period(V_TIMER, v_delay);
	tc_write_count(V_TIMER, 0);
	tc_write_count(I_TIMER, 0);
}

void time_set_V_period(void)
{
	tc_write_period(V_TIMER, TIMER_PERIOD);
}

void time_stop_I_timer(void)
{
	tc_disable(I_TIMER);
}

void time_stop_V_timer(void)
{
	tc_disable(V_TIMER);
}
