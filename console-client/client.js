/***********************************************************************************
    Global Variables
***********************************************************************************/

// User Config Var
var config = {
    timeFormat: "Local",

    serverAddress: "",
    serverPort: 0,
    serverAutoConn: false
}

// Radio List (populated from server)
var radioList = [];

// Websocket connection to server
var serverSocket = null;

// Audio variables
var audio = {
    // Audio context
    context: null,
    // length of buffers in s (these must match the python script)
    spkrBufferDur: 0.2,
    micBufferDur: 0.2,
    // audio transfer sample rate
    transferSamplerate: 16000,
    // Input device, buffer, resampler, and processor
    input: null,
    inputStream: null,
    inputBuffer: null,
    inputProcessor: null,
    // Output device, buffer, processor, and gain (for volume)
    output: null,
    outputBuffer: null,
    outputProcessor: null,
    outputGain: null,
}

// WebRTC Variables
var rtc = {
    // Peer connection object
    peer: null,
    // Audio codec (Opus 48khz stereo)
    codec: "opus/48000/2"
}

testInput = null,

/***********************************************************************************
    State variables
***********************************************************************************/

// Detected timezone
timeZone = "";
// Selected radio
var selectedRadio = null;
var selectedRadioIdx = null;
// PTT state
var pttActive = false;
// Menu state
var menuOpen = false;
// server disconnect commanded
var disconnecting = false;

/***********************************************************************************
    Page Setup Functions
***********************************************************************************/

/**
 * Page load function. Starts timers, etc.
 */
function pageLoad() {
    console.log("Starting client-side runtime");
    // Read config
    readConfig();

    // Get client timezone
    const d = new Date();
    timeZone = d.toLocaleString('en', { timeZoneName: 'short' }).split(' ').pop();

    // Setup clock timer
    setInterval(updateClock, 250);

    // Connect and load if autoconnect is true
    if (config.serverAutoConn) {
        connect()
    }

    // Bind body click to deselecting radios
    //$("#body").click(function () {
    //    deselectRadios();
    //});
}

/**
 * Connect to websocket and setup audio
 */
function connect() {
    // Update navbar
    $("#navbar-status").html("Connecting");
    $("#navbar-status").addClass("pending");
    // Connect websocket first
    connectWebsocket();
    // Start audio devices if they're not already started
    if (!audio.context) {
        startAudioDevices();
    }
    // Wait for the websocket to be connected, then start WebRTC
    waitForWebSocket(serverSocket, startWebRtc);
}

/**
 * Called when GUI is fully connected to server
 */
function connected() {
    // Change button
    $("#server-connect-btn").html("Disconnect");
    $("#server-connect-btn").prop("disabled",false);
    // Change status
    $("#navbar-status").html("Connected");
    $("#navbar-status").removeClass("pending");
    $("#navbar-status").addClass("connected");
    // Get radios
    serverSocket.send(
        `{
            "radios": {
                "command": "query"
            }
        }`
    )
}

/**
 * Disconnect from websocket and teardown audio devices
 */
function disconnect() {
    // Change button
    $("#server-connect-btn").html("Disconnecting...");
    $("#server-connect-btn").prop("disabled", true);
    // Change status
    $("#navbar-status").html("Disconnecting");
    $("#navbar-status").removeClass("connected");
    $("#navbar-status").addClass("pending");
    // Change status
    disconnecting = true;
    // disconnect websocket
    disconnectWebsocket();
}

/**
 * Called when client is done disconnecting
 */
function disconnected() {
    // Update button
    $("#server-connect-btn").html("Connect");
    $("#server-connect-btn").prop("disabled", false);
    // Change status
    $("#navbar-status").html("Disconnected");
    $("#navbar-status").removeClass("connected");
    $("#navbar-status").removeClass("pending");
    // Clear radio cards
    clearRadios();
    // Disable volume slider
    $("#console-volume").prop('disabled', true);
    // Reset variables
    disconnecting = false;
}

// Keydown handler
$(document).on("keydown", function (e) {
    switch (e.which) {
        // Spacebar
        case 32:
            e.preventDefault();
            startPtt();
            break;
    }
});

// Keyup handler
$(document).on("keyup", function (e) {
    switch (e.which) {
        // Spacebar
        case 32:
            e.preventDefault();
            stopPtt();
            break;
    }
});

// Handle losing focus of the window
$(window).blur(function () {
    if (pttActive) {
        console.warn("Killing active PTT due to window focus lost")
        stopPtt();
    }
})

// Bind pageLoad function to document load
$(document).ready(pageLoad());

/***********************************************************************************
    Radio UI Functions
***********************************************************************************/

/**
 * Select a radio
 * @param {string} id the id of the radio to select
 */
function selectRadio(id) {
    console.log("Selecting radio " + id);
    // If the radio was already selected, deselect it
    if (selectedRadio == id) {
        // Deselect all radio cards
        deselectRadios();
        // Remove the selected class
        $(`#${id}`).removeClass("selected");
        // Disable the radio controls
        updateRadioControls();
        // Update the variables
        selectedRadio = null;
        selectedRadioIdx = null;
    } else {
        // Deselect all radio cards
        deselectRadios();
        // Select the new radio card
        $(`#${id}`).addClass("selected");
        // Update the variable
        selectedRadio = id;
        selectedRadioIdx = getRadioIndex(id);
        // Update controls
        updateRadioControls();
    }
}

/**
 * Deselect all radios
 */
function deselectRadios() {
    // Stop PTT if we're transmitting
    stopPtt();
    // Remove selected class from all radio cards
    $(".radio-card").removeClass("selected");
    // Set selected radio to null
    selectedRadio = null;
    selectedRadioIdx = null;
    // Update controls
    updateRadioControls();
}

/**
 * Populate radio cards based on the radios in radioList[] and bind their buttons
 */
function populateRadios() {
    // Add a card for each radio in the list
    radioList.forEach((radio, index) => {
        console.log("Adding radio " + radio.name);
        // Add the radio card
        addRadioCard("radio" + String(index), radio.name);
        // Populate its text
        updateRadioCard(index);
    });
    // Bind the cards
    bindRadioCardButtons();
}

/**
 * Clear radio cards and remove all radios from radioList
 */
function clearRadios() {
    // deselect any selected radios
    deselectRadios();
    // Clear main layout
    $("#main-layout").empty();
    // Clear radio list
    radioList = [];
}

/**
 * Add a radio card with the specified id and name
 * @param {string} id ID of the card element
 * @param {string} name Name to display in header
 */
function addRadioCard(id, name) {
    var newCardHtml = `
        <div class="radio-card" id="${id}">
            <div class="header">
                <div class="selected-icon">
                    <ion-icon name="caret-forward-circle-sharp"></ion-icon>
                </div>
                <h2>${name}</h2>
                <div class="icon-stack">
                    <a href="#" onclick="toggleMute(event, this)" class="enabled"><ion-icon name="volume-high-sharp" id="icon-mute"></ion-icon></a>
                    <a href="#"><ion-icon name="warning-sharp" id="icon-alert"></ion-icon></a>
                </div>
            </div>
            <div class="content">
                <div>
                    <h3>CHANNEL</h3>
                    <div id="channel-text" class="value-frame"></div>
                </div>
                <div>
                    <h3>LAST ID</h3>
                    <div id="id-text" class="value-frame"></div>
                </div>
            </div>
            <div class="footer"></div>
        </div>
    `;

    $("#main-layout").append(newCardHtml);
}

/**
 * Binds radio card buttons & click events to functions
 */
function bindRadioCardButtons() {
    // Bind clicking of the card to selection of a radio
    $(".radio-card").on('click', function (event) {
        // Prevent continual propagation
        event.stopPropagation();
        event.stopImmediatePropagation();
        // Get the ID of the selecting item
        var cardId = $(this).attr('id');
        // Select the radio
        selectRadio(cardId);
    })
    // Bind the minimize button
    $(".minimize-radio-card").click(function (event) {

    })
}

function updateRadioCard(idx) {
    // Get radio from radioList
    var radio = radioList[idx];

    // Get card object
    var radioCard = $("#radio" + String(idx));

    // Update text boxes
    radioCard.find("#channel-text").html(radio.chan);
    radioCard.find("#id-text").html(radio.lastid);

    // Remove all current classes
    radioCard.removeClass("transmitting");
    radioCard.removeClass("receiving");
    radioCard.removeClass("disconnected");

    // Update radio state
    switch (radio.state) {
        case "Transmitting":
            radioCard.addClass("transmitting");
            break;
        case "Receiving":
            radioCard.addClass("receiving");
            break;
        case "Disconnected":
            radioCard.addClass("disconnected");
            break;
    }

    // Update mute icon
    if (radio.muted) {
        radioCard.find("#icon-mute").attr('name', 'volume-mute-sharp');
        radioCard.find("#icon-mute").addClass("muted");
    } else {
        radioCard.find("#icon-mute").attr('name', 'volume-high-sharp');
        radioCard.find("#icon-mute").removeClass("muted");
    }

    // Update alert icon
    if (radio.error) {
        radioCard.find("#icon-alert").addClass("alerting");
    } else {
        radioCard.find("#icon-alert").removeClass("alerting");
    }
}

/**
 * Update the bottom control bar based on the selected radio
 */
function updateRadioControls() {
    // Update if we have a selected radio
    if (selectedRadio) {
        // Get the radio from the list
        var radio = radioList[selectedRadioIdx];
        // If the radio is disconnected, don't enable the controls
        if (radio.state == "Disconnected") { return }
        // Populate text
        $("#selected-zone-text").html(radio.zone);
        $("#selected-chan-text").html(radio.chan);
        // Enable buttons
        $("#radio-controls button").prop("disabled", false);
        // Set buttons to active based on state
        if (radio.scanning) { $("#control-scan").addClass("button-active") } else { $("#control-scan").removeClass("button-active") }
        if (radio.talkaround) { $("#control-dir").addClass("button-active") } else { $("#control-dir").removeClass("button-active") }
        if (radio.monitor) { $("#control-mon").addClass("button-active") } else { $("#control-mon").removeClass("button-active") }
        if (radio.lowpower) { $("#control-lpwr").addClass("button-active") } else { $("#control-lpwr").removeClass("button-active") }
        // Clear if we don't
    } else {
        // Clear text
        $("#selected-zone-text").html("");
        $("#selected-chan-text").html("");
        // Disable buttons
        $("#radio-controls button").prop('disabled', true);
        $("#radio-controls button").removeClass("button-active");
    }
}

/***********************************************************************************
    Radio Backend Functions
***********************************************************************************/

/**
 * Start radio PTT
 */
function startPtt() {
    if (!pttActive && selectedRadio) {
        console.log("Starting PTT on " + selectedRadio);
        pttActive = true;
        playSound("sound-tx-granted");
        // Only send the TX command if we have a valid socket
        if (serverSocket) {
            serverSocket.send(
                `{
                    "radioControl": {
                        "index": ${selectedRadioIdx},
                        "command": "startTx",
                        "options": null
                    }
                }`
            );
        }
    } else if (!pttActive && !selectedRadio) {
        pttActive = true;
        console.log("No radio selected, ignoring PTT");
    }
}

/**
 * Stop radio PTT
 */
function stopPtt() {
    if (pttActive) {
        console.log("PTT released");
        pttActive = false;
        if (serverSocket && selectedRadio) {
            // Wait 250ms and then stop TX (handles mic latency)
            setTimeout( function() {
                serverSocket.send(
                    `{
                        "radioControl": {
                            "index": ${selectedRadioIdx},
                            "command": "stopTx",
                            "options": null
                        }
                    }`
                )
            }, 250);
        }
    }
}

/**
 * Change channel on selected radio
 * @param {bool} down Whether to go down or not (heh)
 */
function changeChannel(down) {
    if (!pttActive && selectedRadio && serverSocket) {
        if (down) {
            console.log("Changing channel down on " + selectedRadio);
            serverSocket.send(
                `{
                    "radioControl": {
                        "index": ${selectedRadioIdx},
                        "command": "chanDn",
                        "options": null
                    }
                }`
            );
        } else {
            console.log("Changing channel up on " + selectedRadio);
            serverSocket.send(
                `{
                    "radioControl": {
                        "index": ${selectedRadioIdx},
                        "command": "chanUp",
                        "options": null
                    }
                }`
            );
        }
    }
}

/**
 * Toggle monitor
 */
function toggleMonitor() {
    if (!pttActive && selectedRadio && serverSocket) {
        console.log("Toggling monitor on " + selectedRadio);
        serverSocket.send(
            `{
                "radioControl": {
                    "index": ${selectedRadioIdx},
                    "command": "button",
                    "options": "monitor"
                }
            }`
        )
    }
}

/**
 * Hit nuisance delete button
 */
 function nuisanceDelete() {
    if (!pttActive && selectedRadio && serverSocket) {
        console.log("Nuisance delete: " + selectedRadio);
        serverSocket.send(
            `{
                "radioControl": {
                    "index": ${selectedRadioIdx},
                    "command": "button",
                    "options": "nuisance"
                }
            }`
        )
    }
}

/**
 * Toggle low power
 */
function togglePower() {
    if (!pttActive && selectedRadio && serverSocket) {
        console.log("Toggling power on " + selectedRadio);
        serverSocket.send(
            `{
                "radioControl": {
                    "index": ${selectedRadioIdx},
                    "command": "button",
                    "options": "power"
                }
            }`
        )
    }
}

/**
 * Turn scan on or off for selected radio
 */
function toggleScan() {
    if (!pttActive && selectedRadio && serverSocket) {
        console.log("Toggling scan for " + selectedRadio);
        serverSocket.send(
            `{
                "radioControl": {
                    "index": ${selectedRadioIdx},
                    "command": "button",
                    "options": "scan"
                }
            }`
        )
    }
}

/**
 * Toggle talkaround (I should really standardize on Talkaround vs Direct)
 */
function toggleDirect() {
    if (!pttActive && selectedRadio && serverSocket) {
        console.log("Toggling talkaround for " + selectedRadio);
        serverSocket.send(
            `{
                "radioControl": {
                    "index": ${selectedRadioIdx},
                    "command": "button",
                    "options": "direct"
                }
            }`
        )
    }
}

/**
 * Toggle the status of radio mute
 * @param {string} obj element whose parent radio to toggle mute on
 */
function toggleMute(event, obj) {
    // Only do stuff if we have a socket connection
    if (serverSocket != null) {
        // Get ID of radio to mute
        const radioId = $(obj).closest(".radio-card").attr('id');
        // Get index of radio in list
        const idx = getRadioIndex(radioId);
        // Change mute status
        if (radioList[idx].muted) {
            console.log("Unmuting " + radioId);
            serverSocket.send(
                `{
                    "audioControl": {
                        "command": "unmute",
                        "index": ${idx}
                    }
                }`
            )
        } else {
            console.log("Muting " + radioId);
            serverSocket.send(
                `{
                    "audioControl": {
                        "command": "mute",
                        "index": ${idx}
                    }
                }`
            )
        }
        // Update card
        //updateRadioCard(idx);
        // Stop propagation so we don't also select the muted radio
        event.stopPropagation();
    }
}

/***********************************************************************************
    Global UI Functions
***********************************************************************************/

/**
 * Toggles the state of the sidebar menu
 */
function toggleMainMenu() {
    if (menuOpen) {
        $("#sidebar-mainmenu").addClass("sidebar-closed");
        $("#button-mainmenu").removeClass("button-active")
        menuOpen = false;
    } else {
        $("#sidebar-mainmenu").removeClass("sidebar-closed");
        $("#button-mainmenu").addClass("button-active")
        menuOpen = true;
    }
}

/**
 * Shows the specified popup and dims the main screen behind it
 * @param {string} id element ID of the popup to show
 */
function showPopup(id) {
    $("#body-dimmer").show();
    $(id).show();
}

/**
 * Close a popup window and undim the background
 * @param {string} obj the object whose parent .popup window will be closed
 */
function closePopup(obj = null) {
    // Close specific popup if specified
    if (obj) {
        $(obj).closest(".popup").hide();
    }
    // Close all popups otherwise
    else {
        $('.popup').hide();
    }
    $("#body-dimmer").hide();
}

/**
 * Update the clock based on the selected time format
 */
function updateClock() {
    var timestr = "HH:mm:ss"
    if (config.timeFormat == "Local") {
        var time = getTimeLocal(timestr);
        $("#clock").html(time + " " + timeZone);
    } else if (config.timeFormat == "UTC") {
        $("#clock").html(getTimeUTC(timestr + " UTC"));
    } else {
        console.error("Invalid time format!")
    }
}

function connectButton() {
    // Connect if we're not connected
    if (!serverSocket) {
        connect();
    } else if (serverSocket && !disconnecting) {
        disconnect();
    }
}

/***********************************************************************************
    Global Backend Functions
***********************************************************************************/

/**
 * Returns UTC time string in given format
 * @param {string} formatString Time formatting string
 * @returns the formatted time string
 */
function getTimeUTC(formatString) {
    // Get UTC time
    var now = dayjs.utc();
    return now.format(formatString);
}

/**
 * Returns local time string in given format
 * @param {string} formatString Time formatting string
 * @returns the formatted local time string
 */
function getTimeLocal(formatString) {
    // Get local time
    var now = dayjs();
    return now.format(formatString);
}

/**
 * Get radio index from id string (radio1 returns 1)
 * @param {string} id radio id
 * @returns index of radio
 */
function getRadioIndex(id) {
    return idx = parseInt(id.replace("radio", ""));
}

/**
 * Save the server config input
 */
function saveServerConfig() {
    // Disconnect from existing server

    // Get values
    var address = $("#server-address").val();
    var port = $("#server-port").val();
    var autoconnect = $("#server-autoconnect").prop('checked');

    // Remove invalid class
    $("#server-port").removeClass("invalid");

    // Validate port
    if (parseInt(port) < 1 || parseInt(port) > 65535) {
        $("#server-port").addClass("invalid");
        return
    }

    // Save config info
    config.serverAddress = address;
    config.serverPort = port;
    config.serverAutoConn = autoconnect;

    // Save config to cookie
    saveConfig();
}

/**
 * Save the client config
 */
function saveClientConfig() {
    // Get values
    var timeFormat = $("#client-timeformat").val()

    // Set config
    config.timeFormat = timeFormat;

    // Save config to cookie
    saveConfig();
}

/**
 * Save config to cookie, as JSON
 */
function saveConfig() {
    // Convert config object to cookie
    configJson = JSON.stringify(config);
    console.log("Saving config json: " + configJson);
    // Save to cookie
    Cookies.set('config',configJson);
}

function readConfig() {
    // Read config from cookie
    configJson = Cookies.get('config');
    // Only try and parse if we have a stored config cookie
    if (configJson) {
        // Convert to config object
        config = JSON.parse(configJson);
        // Update server popup values
        $("#server-address").val(config.serverAddress);
        $("#server-port").val(config.serverPort);
        $("#server-autoconnect").prop('checked',config.serverAutoConn);
        // Update client popup values
        $("#client-timeformat").val(config.timeFormat);
    } else {
        console.warn("No config cookie detected, using defaults");
    }
}

/***********************************************************************************
    WebRTC Functions

    These are adapted/borrowed from:
    https://github.com/webrtc/samples/tree/gh-pages/src/content/peerconnection/audio
***********************************************************************************/

/**
 * Initiate WebRTC connection with server
 * @returns {boolean} true if connection starts successfully
 */
function startWebRtc() {
    console.log("Starting WebRTC session");
    $("#navbar-status").html("Connecting WebRTC");

    // Create peer
    rtc.peer = createPeerConnection();
    if (rtc.peer) {
        console.log("Created peer connection");
    } else {
        console.error("Failed to create peer connection");
        return false
    }
    
    // Find the right getUserMedia()
    if (!navigator.getUserMedia) {
        navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia;
    }

    // Open the microphone
    if (navigator.getUserMedia) {
        // Get the microphone
        navigator.getUserMedia({audio:true},
            // Add tracks to peer connection and negotiate if successful
            function(stream) {
                // Add the first available mic track to the peer connection
                rtc.peer.addTrack(stream.getTracks()[0])
                // Create and send the WebRTC offer
                return sendRtcOffer();
            },
            // Report a failure to capture mic
            function(e) {
                alert('Error capturing microphone device');
                return false;
            }
        );
    } else {
        alert('Cannot capture microphone: getUserMedia() not supported in this browser');
        return false;
    }
}

function stopWebRtc() {

    // Close any active peer transceivers
    if (rtc.peer.getTransceivers) {
        rtc.peer.getTransceivers().forEach(function(tx) {
            if (tx.stop) {
                tx.stop();
            }
        })
    }

    // Close any local audio
    rtc.peer.getSenders().forEach(function(sender) {
        sender.track.stop();
    });

    // Close peer connection
    setTimeout(function() {
        rtc.peer.close();
    }, 500);
}

/**
 * Create a new WebRTC peer connection
 * @returns {RTCPeerConnection} the created peer connection object
 */
function createPeerConnection() {

    // Create config object
    var config = {
        sdpSemantics: 'unified-plan'
    };

    // Create peer
    var peer = new RTCPeerConnection(config);

    // Register event listeners for debug
    peer.addEventListener('icegatheringstatechange', function() {
        console.log(`new peer iceGatheringState: ${peer.iceGatheringState}`);
    }, false);

    peer.addEventListener('iceconnectionstatechange', function() {
        console.log(`new peer iceConnectionState: ${peer.iceConnectionState}`);
        if (peer.iceConnectionState == "connected") {
            connected();
        }
    }, false);

    peer.addEventListener('signalingstatechange', function() {
        console.log(`new peer signallingState: ${peer.signalingState}`);
    })

    // Print initial states
    console.log(`new peer iceGatheringState: ${peer.iceGatheringState}`);
    console.log(`new peer iceConnectionState: ${peer.iceConnectionState}`);
    console.log(`new peer signallingState: ${peer.signalingState}`);

    // Connect audio stream from peer to the web audio objects
    peer.addEventListener('track', function(event) {
        if (event.track.kind == 'audio') {
            console.log("Got new audio track from server");
            var source = audio.context.createMediaStreamSource(event.streams[0]);
            source.connect(audio.outputGain);
        }
    })

    // Return the new peer object
    return peer;
}

/**
 * Create and send the SDP offer to the server
 * @returns {boolean} true on success
 */
function sendRtcOffer() {
    // Generate the SDP offer and assign it to the peer object
    rtc.peer.createOffer().then(function(offer) {
        return rtc.peer.setLocalDescription(offer);
    }).then(function() {
        // Wait for ICE gathering to complete (this looks messy but it's just waiting for that)
        return new Promise(function(resolve) {
            if (rtc.peer.iceGatheringState === 'complete') {
                resolve();
            } else {
                function checkState() {
                    if (rtc.peer.iceGatheringState === 'complete') {
                        rtc.peer.removeEventListener('icegatheringstatechange', checkState);
                        resolve();
                    }
                }
                rtc.peer.addEventListener('icegatheringstatechange', checkState);
            }
        });
    }).then(function() {
        // Generate the specifics of the offer
        var offer = rtc.peer.localDescription;
        offer.sdp = sdpFilterCodec('audio', rtc.codec, offer.sdp);

        // Send the offer to the server via WebSocket
        serverSocket.send(
        `{
            "webRtcOffer": {
                "type": ${JSON.stringify(offer.type)},
                "sdp": ${JSON.stringify(offer.sdp)}
            }
        }`
        );
    }).catch(function(e) {
        console.error(e);
        return false;
    });
    // Return true if nothing bad happened
    return true;
}

/**
 * Take the SDP response from the server and configure the peer
 * @param {string} answerType SDP type
 * @param {string} answerSdp SDP
 */
function gotRtcResponse(answerType, answerSdp) {
    console.log("Got WebRTC response from server");
    var answer = {
        type: answerType,
        sdp: answerSdp
    }
    rtc.peer.setRemoteDescription(answer);
}

/**
 * Find an SDP based on the specified codec
 * 
 * This was stolen directly from the aiortc example
 * 
 * @param {string} kind 'audio' or 'video'
 * @param {string} codec specific codec descriptor
 * @param {*} realSdp existing SDP
 * @returns new SDP using specified codec
 */
function sdpFilterCodec(kind, codec, realSdp) {
    var allowed = []
    var rtxRegex = new RegExp('a=fmtp:(\\d+) apt=(\\d+)\r$');
    var codecRegex = new RegExp('a=rtpmap:([0-9]+) ' + escapeRegExp(codec))
    var videoRegex = new RegExp('(m=' + kind + ' .*?)( ([0-9]+))*\\s*$')
    
    var lines = realSdp.split('\n');

    var isKind = false;
    for (var i = 0; i < lines.length; i++) {
        if (lines[i].startsWith('m=' + kind + ' ')) {
            isKind = true;
        } else if (lines[i].startsWith('m=')) {
            isKind = false;
        }

        if (isKind) {
            var match = lines[i].match(codecRegex);
            if (match) {
                allowed.push(parseInt(match[1]));
            }

            match = lines[i].match(rtxRegex);
            if (match && allowed.includes(parseInt(match[2]))) {
                allowed.push(parseInt(match[1]));
            }
        }
    }

    var skipRegex = 'a=(fmtp|rtcp-fb|rtpmap):([0-9]+)';
    var sdp = '';

    isKind = false;
    for (var i = 0; i < lines.length; i++) {
        if (lines[i].startsWith('m=' + kind + ' ')) {
            isKind = true;
        } else if (lines[i].startsWith('m=')) {
            isKind = false;
        }

        if (isKind) {
            var skipMatch = lines[i].match(skipRegex);
            if (skipMatch && !allowed.includes(parseInt(skipMatch[2]))) {
                continue;
            } else if (lines[i].match(videoRegex)) {
                sdp += lines[i].replace(videoRegex, '$1 ' + allowed.join(' ')) + '\n';
            } else {
                sdp += lines[i] + '\n';
            }
        } else {
            sdp += lines[i] + '\n';
        }
    }

    return sdp;
}

/***********************************************************************************
    Audio Handling Functions
***********************************************************************************/

/** 
* Checks for browser compatibility and sets up audio devices
* @return {bool} True on success
*/
function startAudioDevices() {
    // Create audio context
    audio.context = new AudioContext();
    console.log("Created audio context");

    // Create gain node for output volume and connect it to the default output device
    audio.outputGain = audio.context.createGain();
    audio.outputGain.gain.value = 0.75;
    audio.outputGain.connect(audio.context.destination);

    // Enable volume slider
    $("#console-volume").prop('disabled', false);
}

/**
 *  Change volume of console based on slider
 */
function changeVolume() {
    // Convert 0-100 to 0-1 for multiplication with audio 
    const newVol = $("#console-volume").val() / 100;
    // Set gain node to new value
    audio.outputGain.gain.value = newVol;
}

/**
 * Play an HTML-embedded sound object
 * @param {string} soundId id of the HTML embed object
 */
function playSound(soundId) {
    document.getElementById(soundId).play();
}

/***********************************************************************************
    Websocket Client Functions
***********************************************************************************/

/**
 * Connect to the server's websocket
 */
function connectWebsocket() {
    // Change button
    $("#server-connect-btn").html("Connecting");
    $("#server-connect-btn").prop("disabled",true);
    // Change status
    $("#navbar-status").html("Connecting websocket");
    // Log
    console.log("Websocket connecting to " + config.serverAddress + ":" + config.serverPort);
    // Setup socket
    serverSocket = new WebSocket("ws://" + config.serverAddress + ":" + config.serverPort);
    serverSocket.onerror = handleSocketError;
    serverSocket.onmessage = recvSocketMessage;
    serverSocket.onclose = handleSocketClose;
    // Wait for connection
    waitForWebSocket(serverSocket, onConnectWebsocket);
}

/**
 * Wait for websocket connection to be active
 * @param {WebSocket} websocket 
 * @param {function} callback callback function to execute once connected
 */
function waitForWebSocket(socket, callback=null) {
    setTimeout(
        function() {
            if (socket.readyState === 1) {
                if (callback != null) {
                    callback();
                }
            } else {
                waitForWebSocket(socket, callback);
            }
        },
    5); // 5 ms timeout
}

/**
 * Called once the websocket connection is active
 */
function onConnectWebsocket() {
    $("#navbar-status").html("Websocket connected");
    console.log("Websocket connected");
}

/**
 * Disconnect from the websocket server
 */
function disconnectWebsocket() {
    
    // Disconnect if we had a connection open
    if (serverSocket.readyState == WebSocket.OPEN) {
        console.log("Disconnecting from server");
        serverSocket.close();
    }
}

/**
 * Callback for a new message from the websocket server and
 * parses the JSON command object. 
 * 
 * This command protocol is specified in `Docs/Websocket JSON Signalling.md`
 * @param {event} event 
 */
function recvSocketMessage(event) {

    // Convert to JSON
    var msgObj;
    try {
        msgObj = JSON.parse(event.data);
    } catch (e) {
        console.warn("Got invalid data from websocket: " + event.data);
        console.warn(e);
        return;
    }

    // Iterate through each message and its data (normally we'd only get one at a time, but I suppose you could get more than one)
    for (const [key, value] of Object.entries(msgObj)) {
        // Handle message data based on key type
        switch (key) {

            // List of configured radios
            case "radios":
                // Set our radioList object to the attached radioList object (which was parsed from JSON)
                console.log("Got master radio list update");
                radioList = value['radioList'];
                // Update the UI
                populateRadios();
                bindRadioCardButtons();
                break;

            // Single radio status update
            case "radio":
                // get index of radio
                const idx = value['index'];
                console.log("Got status update for radio " + idx.toString());
                // get status data
                var radioStatus = value['status'];
                // Strip out \u0000's from strings (TODO: figure out why python's decode method adds these and how to get rid of them)
                radioStatus['zone'] = radioStatus['zone'].replace(/\0/g, '');
                radioStatus['chan'] = radioStatus['chan'].replace(/\0/g, '');
                // Update radio entry
                radioList[idx] = radioStatus;
                // Update radio card
                updateRadioCard(idx);
                // Update bottom controls
                updateRadioControls();
                break;

            // WebRTC SDP answer
            case "webRtcAnswer":
                // get params
                answerType = value['type'];
                answerSdp = value['sdp'];
                gotRtcResponse(answerType,answerSdp);
                break;

            // Speaker audio data
            case "audioData":
                // make sure it's actually speaker data
                if (value['source'] != "speaker") {
                    break;
                }
                // Process it
                getSpkrData(value['data']);
                break;

            // NACK handler
            case "nack":
                console.error("Got NACK from server");
                break;
        }
    }
}

/**
 * Handle the websocket closing
 * @param {event} event socket closed event
 */
function handleSocketClose(event) {
    var clean = true;
    console.warn("Server connection closed");
    if (event.data) {console.warn(event.data);}

    // If it wasn't a commanded disconnect, alert the user after we're done cleaning up
    if (!disconnecting) {
        clean = false;
    }

    // Cleanup
    stopWebRtc();
    serverSocket = null;
    disconnected();

    // Show the optional alert
    if (!clean) {
        window.alert("Lost connection to server!");
    }
}

/**
 * Handle connection errors from the server
 * @param {event} event 
 */
function handleSocketError(event) {
    console.error("Server connection error: " + event.data);
    window.alert("Server connection errror: " + event.data);
}

/***********************************************************************************
    Utility Functions
***********************************************************************************/

/**
 * Escape a string to regex?
 * 
 * This was also stolen from the aiortc example. Not exactly sure what it does yet
 * @param {string} string string to escape
 * @returns {string} replaced string
 */
function escapeRegExp(string) {
    return string.replace(/[.*+?^${}()|[\]\\]/g, '\\$&'); // $& means the whole matched string
}