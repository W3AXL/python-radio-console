## JSON Command Spec

Used in websocket communication between client & individial radio servers. Most command exchanges begin on the client and are responded to by the radio server. The server does not query the client at any point in time.

### Radio Query Commands

Used to inform the client on the raido status

```json
{
    "radio": {
        "command": "query"
    }
}
```

`command` can be any of the following:
- `query` indicates that the client wants information on a radio or radios **(client-to-server only)**

The `radioStatus` object is a json-encoded list of radio status parameters defined below

### Radio Status Command

Used to inform the client on the status of a radio. **(server-to-client only)**

```json
{
    "radio": {
        "status": {
            "name": "",
            "desc": "",
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
}
```

This status JSON is created in the `radioClass.py` `encodeClientStatus()` function

### Radio Control Commands

These are sent from the client to the server

```json
{
    "radioControl": {
        "command": "startTx",
        "options": null
    }
}
```

`command` can be any of the following:
- `startTx` / `stopTx` are used to start and stop PTT
- `chanUp` / `chanDn` are used to change channels
- `chanGoto` is used to goto a specific channel index **(not yet implemented)**
   - requires a number in the `options` field
- `button` indicates a button press. `options` specifies the button, valid options are:
   - `softkey1` through `softkey5` requisite softkey (dynamic for O5 control head or static for user-defined buttons)
   - `left` and `right` for paging through softkeys

### WebRTC Signalling Commands

These are sent both ways, and contain signalling messages for WebRTC configuration

```json
{
    "webRtcOffer": {
        "type": {},
        "sdp": {}
    }
}
```

This is the initial WebRTC offer from the client to the server.

`type` contains the SDP type
`sdp` contains the SDP data

```json
{
    "webRtcAnswer": {
        "type": {},
        "sdp": {}
    }
}
```

This is the WebRTC SDP answer from the server to the client

`type` contains the SDP type
`sdp` contains the SDP data

### NACK Message

```json
{
    "nack": {}
}
```