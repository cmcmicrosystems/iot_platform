# Compact BLE IoT Sensor Node
This IoT reference design is a compact BLE sensor node that features:
- Bluetooth Low Energy (BLE) wireless connectivity
- Array of real-time sensors
- Direct smartphone/tablet connectivity
- Coin cell battery for long life operation
- Small form factor
- JTAG programming access

![BLE Sensor Node diagram](IoT_platform.png)

The BLE SoC integrates an ARM Cortex-M3 to
control the wireless transceiver and interface with
the sensors. This design combines an ambient light
sensor, a temperature sensor, and an accelerometer
to enable applications such as building automation,
industrial equipment monitoring, etc.

A software interface app to display and log data is
included, and can be readily installed on any
Android system with BLE capabilities. This app
can communicate with and configure any sensor
node within the communications radius. The app
also provides estimates of the signal strength and
remaining battery lifetime for each sensor node.

This compact BLE sensor node can be used in
applications including:

- Real time sensing of ambient light and temperature for environmental monitoring and control
- HVAC systems and building automation
- Shock, vibration, and temperature monitoring of industrial equipment
- Theft, temperature monitoring of storage/shipping containers

This IoT Platform is a PCB version of a reference design developed for the multi-technology interposer platform.
