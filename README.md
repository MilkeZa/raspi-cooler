# Raspi-Cooler

Raspi-Cooler is a small, automated system designed to keep an old Raspberry Pi board cool using a few basic electronic components.

## Files

| Name | Purpose | Location |
| :--- | :------ | :------- |
| trigger_fan.c | Reads input temperature and sends fan state signal out, HIGH if cooling is necessary, LOW otherwise. | Pi |
| trigger-fan.sh | Runs each minute via a cron job, it gets the approximate cpu temperature and strips the output, then passes it along to the trigger_fan executable as an argument. | Pi |
| fan_listener.py* | Listens for fan state signal changes and opens/closes relay accordingly, cutting/providing power to the cooling fan. | Pico |

\* This file should be re-named *main.py* when loaded onto the Pico so that it runs at boot.

## Hardware

| Name | Purpose |
| :--- | :------ |
| Raspberry Pi | Device to be cooled. It sends the cooling signal. |
| Raspberry Pi Pico | Listens for the cooling signal and controls the relay used to turn the cooling fan ON/OFF. |
| 120mm Cooling Fan | Cools the Pi by blowing air over it. |
| 5v Relay | Allows/Prohibits the flow of current to the fan based on input received from the Pico. |
| 9v Power Supply* | Provides power to the cooling fan. |
| Jumper Wires | Connects the components to one another. |

\* This is not a requirement as cooling fans like the one used can run on 5v power, they are just better at cooling when run at higher voltages. Stay within the safe range of the fan, though, as you obviously shouldn't plug a fan like this into a 120v outlet directly.

## Running

A great way to automate the running of commands or executables is via a Cron job. The trigger-fan.sh script was setup using the following.

``* * * * * cd /home/[USER]/utilities/cooling-scripts && ./trigger-fan.sh``

Another setup that would work is below.

``* * * * * cd ~/utilities/cooling-scripts && ./trigger-fan.sh``

You may also elect to manually run the executable doing the following, which was done heavily during testing. Any float value can be passed in as the TEST_TEMPERATURE value. The executable won't run unless called using *sudo*.

``sudo ~/utilities/cooling-scripts/trigger_fan [TEST_TEMPERATURE]``
