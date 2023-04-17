"""
File   : fan_listener.py
Author : Zachary Milke
Description :
    Sits waiting for a fanState signal sent from a Raspberry Pi, indicating
    whether or not to close a relay allowing power to flow to a cooling fan.

Created on : 13-APR-2023
Updated on : 17-APR-2023

hb_in      - GPIO 12. Listens for a heartbeat signal from Pi.
hb_out     - GPIO 13. Sends hearbeat acknowledge signal to Pi.
signal_in  - GPIO 14. Fan state signal from Raspberry Pi to Pico.
signal_out - GPIO 15. Trigger from Pico to relay indicating state.
"""


from _thread import start_new_thread
from utime import sleep_ms
from machine import Pin


def handle_fan_state(state_check_delay: int = 250):
    """ This method handles the fan state logic and triggering of the relay.
    If the value of the input pin is HIGH, open the relay, allowing current to
    flow to the fan, otherwise, close it.

    Params
    -----
    state_check_delay : int [optional]
        How long in milliseconds the Pico will sleep between checks for the fan
        state signal coming in from the Pi.
    """

    sig_in, sig_out = Pin(14, Pin.IN), Pin(15, Pin.OUT)
    while True:
        if sig_in.value() == 1:
            sig_out.value(1)
        else:
            sig_out.value(0)
        sleep_ms(state_check_delay)


def handle_heartbeat(ack_reset_delay: int = 250):
    """ Running on a separate core, this method listens for a heartbeat signal
    from the Raspberry Pi and responds accordingly. 

    Params
    -----
    ack_reset_delay : int [optional]
        How long in milliseconds the heartbeat acknowledge signal is set to
        HIGH.
    """

    hb_in, hb_out = Pin(12, Pin.IN), Pin(13, Pin.OUT)
    while True:
        if hb_in.value() == 1:
            hb_out.value(1)
            sleep_ms(ack_reset_delay)
            hb_out.value(0)


def start() -> None:
    """ Start functions on independent threads allowing for concurrent running.
    """

    try:
        core1_thread = start_new_thread(handle_heartbeat, ())
        handle_fan_state()
    except Exception:
        return


def main():
    """ Main entrypoint of the application.
    """

    start()


if __name__ == "__main__":
    main()
