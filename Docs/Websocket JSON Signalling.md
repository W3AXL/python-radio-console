## JSON Command Spec

Used in websocket communication between client & server. Most command exchanges begin on the client and are responded to by the server. The server does not query the client at any point in time.

### Radio Query Commands

Used to inform the client on the server's configured radios

```json
{
    "radios": {
        "command": "query"
    }
}
```

```json
{
    "radios": {
        "command": "list",
        "radioList": {
            " radioList JSON goes here "
        }
    }
}
```

`command` can be any of the following:
- `query` indicates that the client wants information on a radio or radios **(client-to-server only)**
- `info` indicates that a radioList object will follow **(server-to-client only)**

The `radioList` object is the standard JSON-encoded list of radios and their current states

### Radio Status Command

Used to inform the client on the status of a specific radio. **(server-to-client only)**

```json
{
    "radio": {
        "name": "",
        "zone": "",
        "chan": "",
        "lastid": "",
        "state": 2,
        "muted": false,
        "error": false,
        "errorText": "",
        "scanning": true,
        "talkaround": false,
        "monitor": true,
        "lowpower": true
    }
}
```

This status JSON is created in the `radioClass.py` `encodeClientStatus()` function

### Radio Control Commands

These are sent from the client to the server

```json
{
    "control": {
        "index": 1,
        "command": "startTx",
        "options": null
    }
}
```

`index` specifies the radio index the control command is destined for

`command` can be any of the following:
- `startTx` / `stopTx` are used to start and stop PTT
- `chanUp` / `chanDn` are used to change channels
- `chanGoto` is used to goto a specific channel index **(not yet implemented)**
   - requires a number in the `options` field
- `button` indicates a button press. `options` specifies the button, valid options are:
   - `mon` monitor
   - `nuisance` nuisance delete
   - `power` low/high power
   - `scan` scan
   - `direct` talkaround/repeater

### Audio Control Commands

These are used to control various audio functions

```json
{
    "audioCtrl": {
        "command": "mute",
        "index": 1
    }
}
```

`command` can be any of the following:
 - `startAudio` is used to initialize the server-side audio handling routines. `index` should be `null` in this case
 - `mute` / `unmute` are used to mute/unmute a specific radio. `index` must be specified
 

### Legacy Audio Data Command

This is used for sending audio data arrays between the client and server

```json
{
    "audioData": {
        "source": "mic",
        "data": [ " array of mic samples " ]
    }
}
```

`source` can be either `mic` or `speaker`

`data` contains the array of mu-law encoded audio samples (as a comma-separated string)

### WebRTC Signalling Commands

These are sent both ways, and contain signalling messages for WebRTC configuration

**To be Implemented**

### NACK Message

```json
{
    "nack": {}
}
```