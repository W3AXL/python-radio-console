/***********************************************************************************
    Global Variables
***********************************************************************************/

var version = "1.0.0"

// User Config Var
var config = {
    timeFormat: "Local",

    audio: {
        rxAgc: false,
        unselectedVol: -3.0    // volume difference in dB for unselected radios
    },
    
    serverAddress: "",
    serverPort: 0,
    serverAutoConn: false
}

// Radio List (populated from server)
var radioList = [];

// Radio speaker sources & gain nodes
var radioSources = [];

// Websocket connection to server
var serverSocket = null;

// Audio variables
var audio = {
    // Audio context
    context: null,
    // Input device, stream, meter, etc
    input: null,
    inputStream: null,
    inputAnalyzer: null,
    inputPcmData: null,
    inputMeter: document.getElementById("meter-mic"),
    // Output device, analyzer, and gain (for volume)
    output: null,
    outputGain: null,
    outputAnalyzer: null,
    outputPcmData: null,
    outputMeter: document.getElementById("meter-spkr"),
    dummyOutputs: [],
    // AGC parameters
    agcThreshold: -50.0,
    agcKnee: 40.0,
    agcRatio: 8.0,
    agcAttack: 0.0,
    agcRelease: 0.3,
    agcMakeup: 2.0,     // 3.0dB
}

// WebRTC Variables
var rtc = {
    // Peer connection object
    peer: null,
    // Audio codec
    codec: "opus/48000/2",  // I've found that OPUS seems to have better latency than PCMU
    //codec: "PCMU/8000",
    // Total audio round trip time (in ms) (set as const for now, adds delay before muting RX audio and stopping TX)
    rxLatency: 500,
    txLatency: 300
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
    // Populate version
    $("#navbar-version").html(version);

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
        // Update stream volumes
        updateRadioAudio();
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
        updateRadioAudio();
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
                    <a href="#" onclick="showPanMenu(event, this)" class="enabled"><ion-icon name="headset-sharp" id="icon-panning"></ion-icon></a>
                        <div class="panning-dropdown closed">
                            <a id="left-spkr-button" href="#" onclick="toggleSpkr(event, this, 'left')"><ion-icon name="volume-off-sharp" style="transform: scaleX(-1);"></ion-icon></a>
                            <a id="right-spkr-button" href="#" onclick="toggleSpkr(event, this, 'right')"><ion-icon name="volume-off-sharp"></ion-icon></a>
                        </div>
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
    setTimeout(function() {
        radioCard.removeClass("transmitting");
    }, rtc.txLatency);
    setTimeout(function () {
        radioCard.removeClass("receiving");
    }, rtc.rxLatency);
    radioCard.removeClass("disconnected");

    // Update radio state
    switch (radio.state) {
        case "Transmitting":
            setTimeout(function() {
                radioCard.addClass("transmitting");
            }, rtc.txLatency);
            break;
        case "Receiving":
            setTimeout(function() {
                radioCard.addClass("receiving");
            }, rtc.rxLatency);
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
        // Enable softkeys
        $("#radio-controls button").prop("disabled", false);
        // Get softkey text
        radio.softkeys.forEach(function(keytext, index) {
            $(`#softkey${index+1} .softkey`).html(keytext);
        });
        // Set softkeys on/off
        radio.softkeyStates.forEach(function(state, index) {
            if (state) { $(`#softkey${index+1}`).addClass("button-active") } else { $(`#softkey${index+1}`).removeClass("button-active") }
        });
    
        // Clear if we don't
    } else {
        // Clear text
        $("#selected-zone-text").html("");
        $("#selected-chan-text").html("");
        for (i=0; i<6; i++) {
            $(`#softkey${i+1} .softkey`).html("");
        }
        // Disable softkeys
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
            // Wait and then stop TX (handles mic latency)
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
            }, rtc.txLatency);
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
 * Toggle a softkey on the selected radio
 * @param {int} idx softkey index
 */
function softkey(idx) {
    sendButton(`softkey${idx}`);
}

/**
 * Left arrow button
 */
function button_left() {
    sendButton("left");
}

/**
 * Right arrow button
 */
function button_right() {
    sendButton("right");
}

/**
 * Send button command to selected radio
 * @param {string} buttonName name of button
 */
function sendButton(buttonName) {
    if (!pttActive && selectedRadio && serverSocket) {
        console.log(`Sending button: ${buttonName}`);
        serverSocket.send(
            `{
                "radioControl": {
                    "index": ${selectedRadioIdx},
                    "command": "button",
                    "options": "${buttonName}"
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
    var timeFormat = $("#client-timeformat").val();
    var rxAgc = $("#client-rxagc").is(":checked");
    var unselectedVol = $("#unselected-vol").val();

    // Set config
    config.timeFormat = timeFormat;
    config.audio.rxAgc = rxAgc;
    config.audio.unselectedVol = parseFloat(unselectedVol);

    // Save config to cookie
    saveConfig();

    // Update radio audio
    if (audio.context) {
        updateRadioAudio();
    }
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
        $("#client-rxagc").prop("checked", config.audio.rxAgc);
        $(`#unselected-vol option[value=${config.audio.unselectedVol}]`).attr('selected', 'selected');
    } else {
        console.warn("No config cookie detected, using defaults");
    }
}

/**
 * Clears out config from cookies
 */
function clearConfig() {
    Cookies.remove('config');
}

/***********************************************************************************
    WebRTC Functions

    These are adapted/borrowed from:
    https://github.com/webrtc/samples/tree/gh-pages/src/content/peerconnection/audio
***********************************************************************************/

function dummyTrack() {
    osc = audio.context.createOscillator();
    dst = osc.connect(audio.context.createMediaStreamDestination());
    osc.start();
    return Object.assign(dst.stream.getAudioTracks()[0], {enabled: false});
}

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
                // Set up mic meter dependecies
                audio.inputStream = audio.context.createMediaStreamSource(stream);
                audio.inputAnalyzer = audio.context.createAnalyser();
                audio.inputPcmData = new Float32Array(audio.inputAnalyzer.fftSize);
                audio.inputStream.connect(audio.inputAnalyzer);
                // Add the first available mic track to the peer connection
                rtc.peer.addTrack(stream.getTracks()[0]);
                // Add additional silent audio tracks so we can send the proper amount of speaker tracks back from the server
                for (var i=0; i<radioList.length - 1; i++) {
                    rtc.peer.addTrack(dummyTrack());
                }
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

    if (rtc.peer.connectionState == "closed") {
        console.log("RTC peer connection already closed");
        return
    }

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
    var sdpConfig = {
        sdpSemantics: 'unified-plan'
    };

    // Create peer
    var peer = new RTCPeerConnection(sdpConfig);

    // Register event listeners for debug
    peer.addEventListener('icegatheringstatechange', function() {
        console.log(`new peer iceGatheringState: ${peer.iceGatheringState}`);
    }, false);

    peer.addEventListener('iceconnectionstatechange', function() {
        console.log(`new peer iceConnectionState: ${peer.iceConnectionState}`);
        if (peer.iceConnectionState == "connected") {
            connected();
        } else if (peer.iceConnectionState == "failed") {
            // Disconnect the client if we had an error (for now, maybe auto-reconnect later?)
            console.error("WebRTC ICE connection error!");
            stopWebRtc();
            serverSocket.close();
        } else if (peer.iceConnectionState == "disconnected") {
            console.error("WebRTC ICE connection disconnected");
            stopWebRtc();
            if (serverSocket) {
                serverSocket.close();
            }
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
            var newIdx = radioSources.length;
            console.debug("New ontrack event:");
            console.debug(event);
            console.log(`Got new audio track from server, index ${newIdx}`);

            // DEBUG: oscillator test
            //var newStream = audio.context.createOscillator();
            //newStream.frequency.setValueAtTime(220 + (360 * newIdx), audio.context.currentTime);

            // Create a new MediaStream from the track we want
            var newStream = new MediaStream( [event.track ]);

            // Create a dummy stream element (chrome bug means the stream won't play if you don't do this)
            var newDummy = new Audio();
            newDummy.muted = true;
            newDummy.srcObject = newStream;
            newDummy.play();
            audio.dummyOutputs.push(newDummy);
            console.debug(`Started dummy audio element for stream ${newIdx}`);

            // Create audio source from the track and put it in an object with a local gain node
            var newSource = {
                audioNode: audio.context.createMediaStreamSource(newStream),
                agcNode: audio.context.createDynamicsCompressor(),
                makeupNode: audio.context.createGain(),
                gainNode: audio.context.createGain(),
                muteNode: audio.context.createGain(),
                panNode: audio.context.createStereoPanner(),
                leftSpkr: true,
                rightSpkr: true
            }

            // Setup AGC node
            newSource.agcNode.knee.setValueAtTime(audio.agcKnee, audio.context.currentTime);
            newSource.agcNode.ratio.setValueAtTime(audio.agcRatio, audio.context.currentTime);
            newSource.agcNode.attack.setValueAtTime(audio.agcAttack, audio.context.currentTime);
            newSource.agcNode.release.setValueAtTime(audio.agcRelease, audio.context.currentTime);

            // Update radio connections
            newSource.audioNode.connect(newSource.agcNode);
            newSource.agcNode.connect(newSource.makeupNode);
            newSource.makeupNode.connect(newSource.gainNode);
            newSource.gainNode.connect(newSource.muteNode);
            newSource.muteNode.connect(newSource.panNode);
            newSource.panNode.connect(audio.outputGain);
            newSource.panNode.connect(audio.outputAnalyzer);

            console.debug(`New source ID: ${newSource.audioNode.id}`);

            // Add to list of radio streams
            radioSources.push(newSource);

            // Update the radio audio
            updateRadioAudio();
            
            // If we got a speaker track back, we have both audio streams and can start the meter frame callback
            window.requestAnimationFrame(audioMeterCallback);
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

        // Debug
        console.debug("SDP offer:");
        console.debug(offer.sdp);

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
 * @param {int} numberOfTracks number of audio tracks to add (should be the same as the number of radios configured)
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

    // Create analyzer node for volume meter under volume slider
    audio.outputAnalyzer = audio.context.createAnalyser();
    audio.outputPcmData = new Float32Array(audio.outputAnalyzer.fftSize);

    // Create gain node for output volume and connect it to the default output device
    audio.outputGain = audio.context.createGain();
    audio.outputGain.gain.value = 0.75;
    audio.outputGain.connect(audio.context.destination);

    // Enable volume slider
    $("#console-volume").prop('disabled', false);
}

/**
 * Updates audio meters based on current data. Only called after both mic & speaker are set up and running (otherwise errors)
 */
function audioMeterCallback() {
    // Get data from speaker & mic
    audio.outputAnalyzer.getFloatTimeDomainData(audio.outputPcmData);
    audio.inputAnalyzer.getFloatTimeDomainData(audio.inputPcmData);

    // Output meter
    let sumSquares = 0.0;
    for (const amplitude of audio.outputPcmData) { sumSquares += amplitude * amplitude; }
    audio.outputMeter.value = Math.sqrt(sumSquares / audio.outputPcmData.length);

    // Input meter (only show when PTT)
    if (pttActive) {
        sumSquares = 0.0;
        for (const amplitude of audio.inputPcmData) { sumSquares += amplitude * amplitude; }
        audio.inputMeter.value = Math.sqrt(sumSquares / audio.outputPcmData.length);
    } else {
        audio.inputMeter.value = 0.0;
    }

    window.requestAnimationFrame(audioMeterCallback);
}

/**
 *  Change volume of console based on slider
 */
function changeVolume() {
    // Convert 0-100 to 0-1 for multiplication with audio, using an inverse-square curve for better "logarithmic" volume
    const newVol = Math.pow($("#console-volume").val() / 100, 2);
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

/**
 * Updates the audio parameters for each radio audio source based on current config and selected radio
 * NOTE: For whatever reason between the python server and this script the track order gets reversed
 * Therefore, we ascend the source list while descending the radio list
 */
 function updateRadioAudio() {
    console.debug("Updating radio sound parameters");
    radioSources.forEach(function(source, idx) {
        radioListIdx = radioList.length - idx - 1;
        if (radioListIdx == selectedRadioIdx) {
            console.debug(`Radio ${idx} is selected. Setting gain to 1`);
            radioSources[idx].gainNode.gain.setValueAtTime(1, audio.context.currentTime);
        } else {
            console.debug(`Radio ${idx} is unselected. Setting gain to ${config.audio.unselectedVol}`);
            radioSources[idx].gainNode.gain.setValueAtTime(dbToGain(config.audio.unselectedVol), audio.context.currentTime);
        }
        // Set AGC based on user setting
        if (config.audio.rxAgc) {
            console.log(`Enabling AGC for radio ${idx}`);
            radioSources[idx].agcNode.threshold.setValueAtTime(audio.agcThreshold, audio.context.currentTime);
            radioSources[idx].makeupNode.gain.setValueAtTime(audio.agcMakeup, audio.context.currentTime);
        } else {
            console.log(`Byassing AGC for radio ${idx}`);
            radioSources[idx].agcNode.threshold.setValueAtTime(0, audio.context.currentTime);
            radioSources[idx].makeupNode.gain.setValueAtTime(1.0, audio.context.currentTime);
        }
    });
}

/**
 * Mutes the radio's audio at the given radio index
 * @param {int} radioListIdx index of the radio in the radio list (not the source list)
 * @param {bool} mute whether to mute or not
 */
function muteRadio(radioListIdx, mute) {
    var sourceIdx = radioSources.length - radioListIdx - 1;
    if (mute) {
        radioSources[sourceIdx].muteNode.gain.setValueAtTime(0, audio.context.currentTime);
    } else {
        radioSources[sourceIdx].muteNode.gain.setValueAtTime(1, audio.context.currentTime);
    }
}

/**
 * Updates the status of the mute for each radio audio source
 */
function updateMute() {
    console.debug("Updating radio source mute statuses");
    radioSources.forEach(function(source, idx) {
        radioListIdx = radioList.length - idx - 1;
        // Mute if we're muted or not receiving, after the specified delay in rtc.rxLatency
        if (radioList[radioListIdx].muted || (radioList[radioListIdx].state != 'Receiving')) {
            setTimeout(function() {
                console.debug(`Muting audio for radio ${radioListIdx} (${radioList[radioListIdx].name})`);
                radioSources[idx].muteNode.gain.setValueAtTime(0, audio.context.currentTime);
            }, rtc.rxLatency);
        } else {
            setTimeout(function() {
                console.debug(`Unmuting audio for radio ${radioListIdx} (${radioList[radioListIdx].name})`);
                radioSources[idx].muteNode.gain.setValueAtTime(1, audio.context.currentTime);
            }, rtc.rxLatency);
        }
    });
}

/**
 * Shows or hides the dropdown
 * @param {event} event 
 * @param {object} obj 
 */
function showPanMenu(event, obj) {
    $(obj).closest(".radio-card").find(".panning-dropdown").toggleClass("closed");
    event.stopPropagation();
}

function closeAllPanMenus() {
    $(".panning-dropdown").addClass("closed");
}

/**
 * Toggles the left or right speaker of a radio on or off
 * @param {event} event 
 * @param {element} obj object calling the function
 * @param {string} channel "left" or "right"
 */
function toggleSpkr(event, obj, channel) {
    // Get calling radio id
    const radioId = $(obj).closest(".radio-card").attr('id');
    // Get index of radio in list
    const idx = getRadioIndex(radioId);
    // Get index in audio sources (inverse of index in list)
    const sourceIdx = radioSources.length - idx - 1;
    // Debug log
    console.debug(`Toggling ${channel} speaker for radio ${radioId}`);
    if (channel == 'left') {
        radioSources[sourceIdx].leftSpkr = !radioSources[sourceIdx].leftSpkr;
    } else if (channel == 'right') {
        radioSources[sourceIdx].rightSpkr = !radioSources[sourceIdx].rightSpkr;
    }
    updatePan(sourceIdx, radioId);
    event.stopPropagation();
}

/**
 * Updates panning for specified radio source
 * @param {int} sourceIdx index of source in radioSources list
 */
function updatePan(sourceIdx, radioId) {
    if (radioSources[sourceIdx].leftSpkr && radioSources[sourceIdx].rightSpkr) {
        // Center pan
        radioSources[sourceIdx].panNode.pan.setValueAtTime(0, audio.context.currentTime);
        // Enable both icons
        $(`#${radioId} #right-spkr-button`).removeClass("disabled");
        $(`#${radioId} #left-spkr-button`).removeClass("disabled");
    } else if (radioSources[sourceIdx].leftSpkr) {
        // Left pan
        radioSources[sourceIdx].panNode.pan.setValueAtTime(-1, audio.context.currentTime);
        // Update icons
        $(`#${radioId} #right-spkr-button`).addClass("disabled");
        $(`#${radioId} #left-spkr-button`).removeClass("disabled");
    } else if (radioSources[sourceIdx].rightSpkr) {
        // Right pan
        radioSources[sourceIdx].panNode.pan.setValueAtTime(1, audio.context.currentTime);
        // Update icons
        $(`#${radioId} #right-spkr-button`).removeClass("disabled");
        $(`#${radioId} #left-spkr-button`).addClass("disabled");
    }
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
    serverSocket = new WebSocket("wss://" + config.serverAddress + ":" + config.serverPort);
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
    //$("#navbar-status").html("Websocket connected");
    console.log("Websocket connected");
    // Query radios
    console.log("Querying master radio list");
    serverSocket.send(
        `{
            "radios": {
                "command": "query"
            }
        }`
    )
    // Start webrtc
    waitForRadioList(startWebRtc);
}

/**
 * Waits for the radiolist to be populated before calling callback
 * @param {function} callback 
 */
function waitForRadioList(callback) {
    setTimeout(
        function() {
            if (radioList.length > 0) {
                if (callback != null) {
                    callback();
                }
            } else {
                waitForRadioList(callback);
            }
        },
    5); // 5 ms timeout
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
                // Debug
                console.debug(radioStatus);
                // Update radio entry
                radioList[idx] = radioStatus;
                // Update radio card
                updateRadioCard(idx);
                // Update bottom controls
                updateRadioControls();
                // Update radio mute status
                updateMute();
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

/**
 * Converts a power gain value to the equivalent constant multiplier
 * @param {float} db Gain in decibels
 * @returns gain as a factor relative to 1
 */
function dbToGain(db) {
    return Math.pow(10, db/20).toFixed(3);
}