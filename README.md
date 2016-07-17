# Cutdown
## Arduino library for cutdown modules, handles XBee communication and arming / disarming

Cutdown modules are subpayloads whose sole job is to disconnect the string above from the string below. Usual implementation is to anchor to the string on one side and release the string on the other, so that the cutdown itself stays attached to the line. Cutdowns are useful in a variety of scenarios: dropping a payload (see: Supersonic), severing string remotely to get the payloads out of a tree, or cutting the main payload string from the balloon before it bursts.

Required components for this library: XBee connected to serial pins on Arduino (digital pins 1 and 2) and a cutdown method that can be triggered on command
