# Introduction
BLE LLD is a radio communication layer. It relies on BLE radio hardware, but it is a proprietary radio abstraction layer, not a BLE stack.

BLE LLD provides a light and simple layer to develop proprietary protocols and applications.

Two layers are available:
 - LLD with full features
 - HAL with simple API

# Architecture
```
        ┌───────┐   ┌─────────────┐
        │  HAL  ◄───┤             │
        └───┬───┘   │             │
            │       │             │
        ┌───▼───┐   │             │
        │  LLD  ◄───┤ application │
        └───┬───┘   │             │
            │       │             │
        ┌───▼───┐   │             │
        │  LLD  ◄───┤             │
        │ proxy │   │             │
appli   └───────┘   └──────┬──────┘
core                       │
─────────────────────────IPCC────────
radio                      │
core                   ┌───▼───┐
                       │  LLD  │
                       │ proxy │
                       └───┬───┘
                           │
                       ┌───▼───┐
                       │  LLD  │
                       └───┬───┘
                           │
                       ┌───▼───┐
                       │  BLE  │
                       │ radio │
                       └───────┘
```

On appli core:
 - Application is the user program using a custom radio protocol
 - HAL is a wrapper based on LLD for simple communications
 - LLD is the layer for full features communications
 - LLD proxy packs/unpacks data and commands to/from radio core

On radio core:
 - LLD proxy packs/unpacks data and commands to/from appli core
 - LLD provides the radio abstraction
 - BLE radio is the RF hardware

## Dual core
BLE LLD is designed to run on a dual core hardware:
 - Application core runs user code
 - Radio core runs private code dedicated to radio management

The software communication layer between both core is called IPCC. This transport layer is decoupled from BLE LLD.

This architecture brings some important constraints:
 - no application code runs on radio core
 - it takes a long time to run application code after a radio event

To help implement fast radio operations sequences despite those constraints, "action packets" can be chained by the radio core. This chaining is configured by the application.

Those constraints impact protocol design and LLD/HAL choice.

## Action Packet
Action packet is the structure used by radio core to control transmissions and receptions of radio packets.

Action packets can be chained to execute complex radio sequences. This chaining is configured with two fields of the action packet. One for the next action packet to run if operation (Tx or Rx) succeed, and the other for the next action packet to run if operation failed.

### Back-to-back vs wake-up
Delay between action packets (or before the first packet) can be configured with two modes: back-to-back or wake-up.

In back-to-back mode, everything stays powered, so it offers the lowest delays between action packets. Back-to-back time is a global parameter configured with `BLE_LLD_SetBackToBackTime()`, it cannot be configured separately for each action packet.

In wake-up mode, radio goes to sleep, so the delay between action packets cannot be as short as in back-to-back mode. Wake-up time is configured separately for each action packet.

For the first packet of a sequence, since the radio is not doing anything, wake-up mode must be used.

## Radio packet details
Packets include an address that must match configured address of the recipient.
Each packet can carry a payload of maximum 255 bytes (less if using encryption).
Packets include a CRC and are checked for error at reception.

# Usage

## Blocking functions
API functions are blocking, they wait for processing by the radio core before returning (but not for radio packet transmission/reception).

## Radio proxy configuration
Due to the dual core hardware, user cannot directly access radio core, but must control radio through a proxy. Since BLE LLD is independent from the communication layer with the radio core, it must be "wired" by the application, those wiring functions are prefixed BLE_LLD_PRX_.

`BLE_LLD_PRX_Init()` must be called first to configure the radio core proxy. `BLE_LLD_PRX_EventProcessInter()` and `BLE_LLD_PRX_EventProcessTask()` are responsible for radio events processing and must be called respectively inside and after radio event interruption.

Note: proxy configuration is required no matter which API (LLD/HAL) is used.

## Radio events
User can register a callback function when he starts/configures a radio operation.

When an event occurs on the radio core (e.g. transmission success, reception failure, etc.) the BLE LLD proxy is notified and can in turn run the callback if one was registered for this event.

This mechanism allows for the user application to react to radio events.

## HAL interface
HAL is just a layer on top of LLD to make simple communications easier. HAL has a simple API with limited features, it can be used when no custom packet chaining is required. Under the hood, HAL calls LLD to configure some action packets.

### Configuration
Before packets exchange with HAL, it must be initialized with `HAL_BLE_LLD_Init()` then configured with `HAL_BLE_LLD_Configure()`.

### Communication
Two set of functions are available:
 - without ACK: the radio transmits/receives just one packet
 - with ACK: the radio transmits/receives one packet, then another packet goes in the opposite direction

"With ACK" functions can be used to detect packet loss, thus they can be used to implement a reliable communication channel with retransmissions of lost packets.

## LLD interface
LLD is the layer that exposes all the features supported by the radio core. Its API is more complex and can be used to implement custom packets chaining.

### Configuration
Before packets exchange with LLD, it must be initialized with `BLE_LLD_Init()` then configured with:
 - `BLE_LLD_SetChannel()`
 - `BLE_LLD_SetTxAttributes()`
 - `BLE_LLD_SetTxPower()`
 - `BLE_LLD_SetTx_Rx_Phy()`

### Communication
With LLD API, user is responsible for the configuration of each action packet.

Each action packet must be configured by setting all the required fields for the desired action (some fields are specific to Tx, others to Rx) then calling `BLE_LLD_SetReservedArea()`.

To start the execution of the first action packet, call `BLE_LLD_MakeActionPacketPending()` on that action packet. The radio core will then chain action packets based on their configuration and the result of the radio operation. At the end of each action packet, an event will be sent to the application if a callback was registered for that action packet.

Action packets chaining can be interrupted with `BLE_LLD_StopActivity()`. This will kill the radio, and a new initialization will be required before any other operation.

Below diagram shows the action packets configured to implement `HAL_BLE_LLD_ReceivePacketWithAck()`.
```
      ┌───────┐
      │ START │
      └───┬───┘
          │
          │
 ┌────────▼────────┐
 │ Action Packet 2 │
 │                 │
 │  * reception    │
 │  * wake-up      │
 │  * timeout      │
 └──────┬───┬──────┘
SUCCESS │   │ FAILURE
        │   │
        │   └──────────┐
        │              │
 ┌──────▼──────────┐   │
 │ Action Packet 3 │   │
 │                 │   │
 │  * transmission │   │
 │  * back-to-back │   │
 └──────┬───┬──────┘   │
SUCCESS │   │ FAILURE  │
        │   │          │
      ┌─▼───▼─┐        │
      │ STOP  ◄────────┘
      └───────┘
```
Action Packet 2 is run first, it configures the reception for the data packet. If data is properly received (CRC OK), Action Packet 3 is run next and it configures the transmission of the ACK packet. Then the radio stops. If an action packet fails, radio stops.

## Tone generation
For test purpose, a tone can be generated with `BLE_LLD_StartTone()`. Use `BLE_LLD_StopTone()` to stop this tone. After a tone, the radio needs to be reinitialized before any other operation.