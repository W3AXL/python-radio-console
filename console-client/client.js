/***********************************************************************************
    Global Variables
***********************************************************************************/

var version = "2.0.0";

// Local user config variables (saved to cookie)
var config = {
    timeFormat: "Local",

    tpt: "mot",
    btnSounds: true,

    audio: {
        rxAgc: false,
        unselectedVol: -3.0,
        input: "default",
        output: "default"
    },
    
    serverAddress: "",
    serverPort: 0,
    serverAutoConn: false
}

// Radio List (read from radio config initially and populated with audio sources/sinks and rtc connections)
var radios = [];

// Audio variables
var audio = {
    // Audio context
    context: null,
    // Input device, stream, meter, etc
    input: null,
    inputStream: null,
    inputTrack: null,
    inputAnalyzer: null,
    inputPcmData: null,
    inputMeter: document.getElementById("meter-mic"),
    // Output device, analyzer, and gain (for volume control and visualization)
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
    agcMakeup: 1.0,     // right now any makeup gain causes clipping
}

// WebRTC Variables
var rtcConf = {
    // Audio codec
    codec: "opus/48000/2",  // I've found that OPUS seems to have better latency than PCMU
    //codec: "PCMU/8000",
    // Total audio round trip time (in ms) (set as const for now, adds delay before muting RX audio and stopping TX)
    rxLatency: 500,
    txLatency: 400
}

testInput = null;

// Radio Card Tempalte
const radioCardTemplate = document.querySelector('#card-template');

// Radio JSON validation
const validColors = ["red","amber","green","blue","purple"];

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
    console.log("Starting client runtime");

    // Enable JQuery Tooltips
    //$( document ).tooltip();

    // Populate version
    $("#navbar-version").html(version);

    // Query media devices
    getAudioDevices();

    // Query radio config from client.json
    readRadioConfig();

    // Read user config from cookie
    readUserConfig();

    // Get client timezone
    const d = new Date();
    timeZone = d.toLocaleString('en', { timeZoneName: 'short' }).split(' ').pop();

    // Setup clock timer
    setInterval(updateClock, 100);

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

function radioConnected(idx) {
    // UI update
    $(`#radio${idx} .icon-connect`).removeClass('disconnected');
    $(`#radio${idx} .icon-connect`).removeClass('connecting');
    $(`#radio${idx} .icon-connect`).addClass('connected');
    $(`#radio${idx} .icon-connect`).parent().prop('title','Connected, OK');
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
    // Check that the radio is connected before we select it
    if (radios[getRadioIndex(id)].status.state == "Disconnected") { return; }
    // Log
    console.debug("Selecting radio " + id);
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
 * Populate radio cards based on the radios in radios[] and bind their buttons
 */
function populateRadios() {
    // Add a card for each radio in the list
    radios.forEach((radio, index) => {
        console.log("Adding radio " + radio.name);
        // Add the radio card
        addRadioCard("radio" + String(index), radio.name, radio.color);
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
    radios = [];
}

/**
 * Add a radio card with the specified id and name
 * @param {string} id ID of the card element
 * @param {string} name Name to display in header
 */
function addRadioCard(id, name, color) {
    /*var newCardHtml = `
        <div class="radio-card disconnected" id="${id}">
            <div class="header">
                <div class="selected-icon">
                    <ion-icon name="caret-forward-circle-sharp"></ion-icon>
                </div>
                <h2>${name}</h2>
                <div class="icon-stack">
                    <!-- Pan Dropdown -->
                    <a href="#" onclick="showPanMenu(event, this)" class="enabled"><ion-icon name="headset-sharp" id="icon-panning"></ion-icon></a>
                        <div class="panning-dropdown closed">
                            <!-- Slider -->
                            <input type="range" class="topcoat-range" class="radio-pan" min="-1" max="1" value="0" step="0.1" onclick="stopClick(event, this)" oninput="changePan(event, this)" ondblclick="centerPan(event, this)">
                        </div>
                    <!-- Mute Button -->
                    <a href="#" onclick="toggleMute(event, this)" class="enabled"><ion-icon name="volume-high-sharp" id="icon-mute"></ion-icon></a>
                    <!-- Connection Icon -->
                    <a href="#" onclick="connectButton(event, this)" title="Disconnected"><ion-icon name="wifi-sharp" class="icon-connect disconnected"></ion-icon></a>
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
    `;*/

    // New, much easier way to add new cards
    var newCard = radioCardTemplate.content.cloneNode(true);
    newCard.querySelector(".radio-card").classList.add(color);
    newCard.querySelector(".radio-card").id = id;
    newCard.querySelector(".radio-card .header h2").textContent = name;

    $("#main-layout").append(newCard);
}

function stopClick(event, obj) {
    event.stopPropagation();
    event.preventDefault();
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
    var radio = radios[idx];

    // Get card object
    var radioCard = $("#radio" + String(idx));

    // Update text boxes
    radioCard.find("#channel-text").html(radio.status.chan);
    radioCard.find("#zone-text").html(radio.status.zone);

    // Remove all current classes
    setTimeout(function() {
        radioCard.removeClass("transmitting");
    }, rtcConf.txLatency);
    setTimeout(function () {
        radioCard.removeClass("receiving");
    }, rtcConf.rxLatency);
    radioCard.removeClass("disconnected");

    // Update radio state
    switch (radio.status.state) {
        case "Transmitting":
            setTimeout(function() {
                radioCard.addClass("transmitting");
            }, rtcConf.txLatency);
            break;
        case "Receiving":
            setTimeout(function() {
                radioCard.addClass("receiving");
            }, rtcConf.rxLatency); // used to unmute after latency delay but this makes sure we don't miss anything
            break;
        case "Disconnected":
            radioCard.addClass("disconnected");
            break;
    }

    // Update mute icon
    if (radio.status.muted) {
        radioCard.find("#icon-mute").attr('name', 'volume-mute-sharp');
        radioCard.find("#icon-mute").addClass("muted");
    } else {
        radioCard.find("#icon-mute").attr('name', 'volume-high-sharp');
        radioCard.find("#icon-mute").removeClass("muted");
    }

    // Update alert icon
    if (radio.status.error) {
        radioCard.find("#icon-alert").addClass("alerting");
    } else {
        radioCard.find("#icon-alert").removeClass("alerting");
    }

    // Update Scan Icon
    radioCard.find('.scan-icons').removeClass("scanning");
    radioCard.find('.scan-icons').removeClass("priority");
    radioCard.find('.scan-icons').removeClass("priority2");
    if (radio.status.scanning) {
        if (radio.status.priority == 2) {
            console.debug("Got priority 2 status");
            radioCard.find('.scan-icons').addClass("priority2");
        } else if (radio.status.priority == 1) {
            console.debug("Got priority 1 status");
            radioCard.find('.scan-icons').addClass("priority");
        } else {
            console.debug("Got scanning status");
            radioCard.find('.scan-icons').addClass("scanning");
        }
    } else {
        console.debug("Radio not scanning");
    }
}

/**
 * Update the bottom control bar based on the selected radio
 */
function updateRadioControls() {
    // Update if we have a selected radio
    if (selectedRadio) {
        // Get the radio from the list
        var radio = radios[selectedRadioIdx];
        // If the radio is disconnected, don't enable the controls
        if (radio.status.state == "Disconnected") { return }
        // Enable softkeys
        $("#radio-controls .btn").removeClass("disabled");
        // Get softkey text
        radio.status.softkeys.forEach(function(keytext, index) {
            $(`#softkey${index+1} .btn-text`).html(keytext);
        });
        // Set softkeys on/off
        radio.status.softkeyStates.forEach(function(state, index) {
            if (state) { $(`#softkey${index+1}`).addClass("pressed") } else { $(`#softkey${index+1}`).removeClass("pressed") }
        });
    
        // Clear if we don't
    } else {
        for (i=0; i<6; i++) {
            $(`#softkey${i+1} .btn-text`).html("");
        }
        // Disable softkeys
        $("#radio-controls .btn").addClass("disabled");
        $("#radio-controls .btn").removeClass("pressed");
    }
}

/**
 * Handles connect button click on radio card
 * @param {event} event button click event
 * @param {object} obj html object
 */
function connectButton(event, obj) {
    // Stop propagation of click
    event.stopPropagation();
    // Get ID of radio to mute
    const radioId = $(obj).closest(".radio-card").attr('id');
    // Get index of radio in list
    const idx = getRadioIndex(radioId);
    // If disconnected, connect
    if (radios[idx].status.state == 'Disconnected') {
        connectRadio(idx);
    } else {
        disconnectRadio(idx);
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
        // Play selected TPT
        if (config.tpt == "kw") {
            playSound("sound-tpt-kw");
        } else {
            playSound("sound-tpt-mot");
        }
        // Only send the TX command if we have a valid socket
        if (radios[selectedRadioIdx].wsConn) {
            radios[selectedRadioIdx].wsConn.send(
                `{
                    "radioControl": {
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
        if (radios[selectedRadioIdx].wsConn && selectedRadio) {
            // Wait and then stop TX (handles mic latency)
            setTimeout( function() {
                radios[selectedRadioIdx].wsConn.send(
                    `{
                        "radioControl": {
                            "command": "stopTx",
                            "options": null
                        }
                    }`
                );
                playSound("sound-tx-end");
            }, rtcConf.txLatency);
        }
    }
}

/**
 * Change channel on selected radio
 * @param {bool} down Whether to go down or not (heh)
 */
function changeChannel(down) {
    if (!pttActive && selectedRadio && radios[selectedRadioIdx].wsConn) {
        if (down) {
            if (config.btnSounds) {
                playSound("sound-btn-press");
            }
            console.log("Changing channel down on " + selectedRadio);
            radios[selectedRadioIdx].wsConn.send(
                `{
                    "radioControl": {
                        "command": "chanDn",
                        "options": null
                    }
                }`
            );
        } else {
            if (config.btnSounds) {
                playSound("sound-btn-press");
            }
            console.log("Changing channel up on " + selectedRadio);
            radios[selectedRadioIdx].wsConn.send(
                `{
                    "radioControl": {
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
    if (config.btnSounds) {
        playSound("sound-btn-press");
    }
    sendButton(`softkey${idx}`);
}

/**
 * Left arrow button
 */
function button_left() {
    if (config.btnSounds) {
        playSound("sound-btn-press");
    }
    sendButton("left");
}

/**
 * Right arrow button
 */
function button_right() {
    if (config.btnSounds) {
        playSound("sound-btn-press");
    }
    sendButton("right");
}

/**
 * Send button command to selected radio
 * @param {string} buttonName name of button
 */
function sendButton(buttonName) {
    if (!pttActive && selectedRadio && radios[selectedRadioIdx].wsConn) {
        console.log(`Sending button: ${buttonName}`);
        radios[selectedRadioIdx].wsConn.send(
            `{
                "radioControl": {
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
    // Get ID of radio to mute
    const radioId = $(obj).closest(".radio-card").attr('id');
    // Get index of radio in list
    const idx = getRadioIndex(radioId);
    // Only do stuff if we have a socket connection
    if (radios[idx].wsConn != null) {
        // Change mute status
        if (radios[idx].status.muted) {
            console.log("Unmuting " + radioId);
            radios[idx].wsConn.send(
                `{
                    "audioControl": {
                        "command": "unmute"
                    }
                }`
            )
        } else {
            console.log("Muting " + radioId);
            radios[idx].wsConn.send(
                `{
                    "audioControl": {
                        "command": "mute"
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
function showConfigPopup(id) {
    console.debug(`Showing popup ${id}`);
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

function connectAllButton() {
    // Connect if we're not connected
    if (!serverSocket) {
        //connect();
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
    saveUserConfig();
}

/**
 * Save the client config
 */
function saveClientConfig() {
    // Get values
    const timeFormat = $("#client-timeformat").val();
    const rxAgc = $("#client-rxagc").is(":checked");
    const unselectedVol = $("#unselected-vol").val();
    const talkPermitTone = $("#tpt-tone").val();
    const buttonSounds = $("#btn-sounds").is(":checked");
    const audioInput = $("#audio-input").val();
    const audioOutput = $("#audio-output").val();

    // Set config
    config.timeFormat = timeFormat;
    config.tpt = talkPermitTone;
    config.btnSounds = buttonSounds;
    config.audio.rxAgc = rxAgc;
    config.audio.unselectedVol = parseFloat(unselectedVol);
    config.audio.input = audioInput;
    config.audio.output = audioOutput;

    // Save config to cookie
    saveUserConfig();

    // Update radio audio
    if (audio.context) {
        updateRadioAudio();
    }
}

/**
 * Save config to cookie, as JSON
 */
function saveUserConfig() {
    // Convert config object to cookie
    configJson = JSON.stringify(config);
    console.log("Saving config json: " + configJson);
    // Save to cookie
    Cookies.set('config',configJson);
}

/**
 * Read config from cookie
 */
function readUserConfig() {
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
        $(`#tpt-tone option[value=${config.tpt}]`).attr('selected', 'selected');
        $("#btn-sounds").prop("checked", config.btnSounds);
        $(`#unselected-vol option[value=${config.audio.unselectedVol}]`).attr('selected', 'selected');
        $(`#audio-input option[value=${config.audio.input}]`).attr('selected', 'selected');
        $(`#audio-output option[value=${config.audio.output}]`).attr('selected', 'selected');
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
    Radio Config Reading
***********************************************************************************/

function readRadioConfig() {
    // Read the json config file from the server
    var radioJson = [];
    $.ajax({
        type: 'GET',
        url: './radios.json',
        dataType: 'json',
        success: gotRadioConfig,
        error: function(obj, status, error) {alert(`Failed to read radios.json: ${error}`);console.error(`${status}: ${error}`);},
        async: true,
        timeout: 100
    });
}

function gotRadioConfig(data, status, obj) {
    console.debug(`Successfully read radios.json, status: ${status}`);
    radioJson = data;
    console.debug(radioJson['RadioList']);
    radios = radioJson['RadioList'];
    // Populate default values (this map function adds the key,value pairs to every item in the list)
    radios = radios.map(v => ({
        ...v,
        status: {
            state: 'Disconnected'
        },
        rtc: {},
        wsConn: null,
        audioSrc: null,
    }));

    // Validate Input
    radios.forEach((radio, idx) => {
        // Validate Color
        if (!validColors.includes(radios[idx].color)) {
            console.warn(`Color ${radios[idx].color} not valid, defaulting to blue`);
            radios[idx].color = "blue";
        }
    });

    // Populate radio cards
    populateRadios();

    // Connect and load if autoconnect is true
    if (config.serverAutoConn) {
        //connect()
    }
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
 * @param {int} idx index of radio in radios[]
 * @returns {boolean} true if connection starts successfully
 */
function startWebRtc(idx) {
    console.log(`Starting WebRTC session for ${radios[idx].name}`);

    // Create peer
    radios[idx].rtc.peer = createPeerConnection(idx);
    if (radios[idx].rtc.peer) {
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

        // Old, deprecated way
        //navigator.getUserMedia({audio:true},

        // New, better (?) way
        navigator.mediaDevices.getUserMedia({audio:true}).then(
            // Add tracks to peer connection and negotiate if successful
            function(stream) {
                // Set up mic meter dependecies
                audio.inputStream = audio.context.createMediaStreamSource(stream);
                audio.inputAnalyzer = audio.context.createAnalyser();
                audio.inputPcmData = new Float32Array(audio.inputAnalyzer.fftSize);
                audio.inputStream.connect(audio.inputAnalyzer);
                // Get the first available track for the mic
                audio.inputTrack = stream.getTracks()[0];
                // Add a listener for when the mic track ends (happens occasionally, not sure why)
                audio.inputTrack.addEventListener("ended", () => {
                    alert('Mic track ended, please reconnect');
                    stopWebRtc();
                })
                // Add the first available mic track to the peer connection
                radios[idx].rtc.peer.addTrack(stream.getTracks()[0]);
                // Create and send the WebRTC offer
                return sendRtcOffer(idx);
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

/**
 * Stop RTC session for radio at index
 * @param {int} idx index of radio in radios[]
 * @returns 
 */
function stopWebRtc(idx) {
    // Return if there was never a peer connection to begin with
    if (!radios[idx].rtc.hasOwnProperty('peer')) {
        console.log("No peer connection created");
        return
    }

    // Return if stuff is already closed
    if (radios[idx].rtc.peer.connectionState == "closed") {
        console.log("RTC peer connection already closed");
        return
    }

    // Close any active peer transceivers
    if (radios[idx].rtc.peer.getTransceivers) {
        radios[idx].rtc.peer.getTransceivers().forEach(function(tx, idx) {
            console.debug(`Stopping RTC txcvr ${idx}`);
            if (tx.stop) {
                tx.stop();
            }
        })
    }

    // Close any local audio
    radios[idx].rtc.peer.getSenders().forEach(function(sender, idx) {
        console.debug(`Stopping RTC sender ${idx}`);
        sender.track.stop();
    });

    // Close peer connection
    setTimeout(function() {
        console.debug("Closing peer connection");
        radios[idx].rtc.peer.close();
        radios[idx].audioSrc = null;
    }, 500);
}

/**
 * Create a new WebRTC peer connection
 * @param {int} idx the index of the radio in radios[]
 * @returns {RTCPeerConnection} the created peer connection object
 */
function createPeerConnection(idx) {

    // Create config object
    var sdpConfig = {
        sdpSemantics: 'unified-plan'
    };

    // Create peer
    var peer = new RTCPeerConnection(sdpConfig);

    // Register event listeners for debug
    peer.addEventListener('icegatheringstatechange', function() {
        console.log(`new peer iceGatheringState for radio ${radios[idx].name}: ${peer.iceGatheringState}`);
    }, false);

    peer.addEventListener('iceconnectionstatechange', function() {
        console.log(`new peer iceConnectionState for radio ${radios[idx].name}: ${peer.iceConnectionState}`);
        if (peer.iceConnectionState == "connected") {
            radioConnected(idx);
        } else if (peer.iceConnectionState == "failed") {
            // Disconnect the client if we had an error (for now, maybe auto-reconnect later?)
            console.error(`WebRTC ICE connection failed for radio ${radios[idx].name}`);
            stopWebRtc(idx);
            radios[idx].wsConn.close();
        } else if (peer.iceConnectionState == "disconnected") {
            console.error(`WebRTC ICE connection disconnected for radio ${radios[idx].name}`);
            stopWebRtc(idx);
            if (radios[idx].wsConn) {
                radios[idx].wsConn.close();
            }
        }
    }, false);

    peer.addEventListener('signalingstatechange', function() {
        console.log(`new peer signallingState for radio ${radios[idx].name}: ${peer.signalingState}`);
    })

    // Print initial states
    console.log(`new peer iceGatheringState for radio ${radios[idx].name}: ${peer.iceGatheringState}`);
    console.log(`new peer iceConnectionState for radio ${radios[idx].name}: ${peer.iceConnectionState}`);
    console.log(`new peer signallingState for radio ${radios[idx].name}: ${peer.signalingState}`);

    // Connect audio stream from peer to the web audio objects
    peer.addEventListener('track', function(event) {
        if (event.track.kind == 'audio') {
            console.debug(`New ontrack event for radio ${radios[idx].name}:`);
            console.debug(event);
            console.log(`Got new audio track from server for radio ${radios[idx].name}`);

            // Create a new MediaStream from the track we want
            var newStream = new MediaStream( [event.track ]);

            // Create a dummy stream element (chrome bug means the stream won't play if you don't do this)
            var newDummy = new Audio();
            newDummy.muted = true;
            newDummy.srcObject = newStream;
            newDummy.play();
            audio.dummyOutputs.push(newDummy);
            console.debug(`Started dummy audio element for radio ${radios[idx].name}`);

            // Create audio source from the track and put it in an object with a local gain node
            var newSource = {
                audioNode: audio.context.createMediaStreamSource(newStream),
                agcNode: audio.context.createDynamicsCompressor(),
                makeupNode: audio.context.createGain(),
                gainNode: audio.context.createGain(),
                muteNode: audio.context.createGain(),
                panNode: audio.context.createStereoPanner(),
                analyzerNode: audio.context.createAnalyser(),
                leftSpkr: true,
                rightSpkr: true
            }
            // Create this afterwards because we need the value from the above node
            newSource.analyzerData = new Float32Array(newSource.analyzerNode.fftSize);

            // Create analyzer for RX meter

            // Setup AGC node
            newSource.agcNode.knee.setValueAtTime(audio.agcKnee, audio.context.currentTime);
            newSource.agcNode.ratio.setValueAtTime(audio.agcRatio, audio.context.currentTime);
            newSource.agcNode.attack.setValueAtTime(audio.agcAttack, audio.context.currentTime);
            newSource.agcNode.release.setValueAtTime(audio.agcRelease, audio.context.currentTime);

            // Update radio connections
            newSource.audioNode.connect(newSource.agcNode);
            newSource.agcNode.connect(newSource.makeupNode);
            newSource.makeupNode.connect(newSource.gainNode);
            newSource.makeupNode.connect(newSource.analyzerNode);
            newSource.gainNode.connect(newSource.muteNode);
            newSource.muteNode.connect(newSource.panNode);
            newSource.panNode.connect(audio.outputGain);

            console.debug(`New source ID: ${newSource.audioNode.id}`);

            // Add to list of radio streams
            radios[idx].audioSrc = newSource;

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
 * @param {int} idx index of radio in radios[]
 * @returns {boolean} true on success
 */
function sendRtcOffer(idx) {
    // Generate the SDP offer and assign it to the peer object
    radios[idx].rtc.peer.createOffer().then(function(offer) {
        return radios[idx].rtc.peer.setLocalDescription(offer);
    }).then(function() {
        // Wait for ICE gathering to complete (this looks messy but it's just waiting for that)
        return new Promise(function(resolve) {
            if (radios[idx].rtc.peer.iceGatheringState === 'complete') {
                resolve();
            } else {
                function checkState() {
                    if (radios[idx].rtc.peer.iceGatheringState === 'complete') {
                        radios[idx].rtc.peer.removeEventListener('icegatheringstatechange', checkState);
                        resolve();
                    }
                }
                radios[idx].rtc.peer.addEventListener('icegatheringstatechange', checkState);
            }
        });
    }).then(function() {
        // Generate the specifics of the offer
        var offer = radios[idx].rtc.peer.localDescription;
        offer.sdp = sdpFilterCodec('audio', rtcConf.codec, offer.sdp);

        // Debug
        console.debug("SDP offer:");
        console.debug(offer.sdp);

        // Send the offer to the server via WebSocket
        radios[idx].wsConn.send(
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
 * @param {int} idx index of radio in radios[]
 * @param {string} answerType SDP type
 * @param {string} answerSdp SDP
 */
function gotRtcResponse(idx, answerType, answerSdp) {
    console.log("Got WebRTC response from server");
    var answer = {
        type: answerType,
        sdp: answerSdp
    }
    radios[idx].rtc.peer.setRemoteDescription(answer);
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
 * Queries the specified type of media device
 * @param {string} type type of device to find ('audioinput', 'audiooutput', or 'videoinput')
 * @param {function} callback callback to pass the filtered list of devices once retreived
 */
async function queryDeviceType(type) {
    const devices = await navigator.mediaDevices.enumerateDevices();
    return devices.filter(device => device.kind === type)
}

/**
 * Populate the audio device lists
 */
async function getAudioDevices() {
    const audioInputs = await queryDeviceType('audioinput');
    const audioOutputs = await queryDeviceType('audiooutput');
    
    audioInputs.forEach(input => {
        var name = input.label;
        /*if (name.length > 30) {
            name = name.substring(0,30) + "...";
        }*/
        const device = input.deviceId;
        $("#audio-input").append($('<option>', {
            value: device,
            text: name
        }));
    })

    audioOutputs.forEach(output => {
        var name = output.label;
        /*if (name.length > 30) {
            name = name.substring(0,30) + "...";
        }*/
        const device = output.deviceId;
        $("#audio-output").append($('<option>', {
            value: device,
            text: name
        }));
    })
}

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
    // Per-Radio RX Level Bar
    radios.forEach((radio, idx) => {
        // Ignore radios with no connected audio
        if (radios[idx].audioSrc == null) {
            return
        }
        // Ignore if not receiving
        if (radios[idx].status.state != 'Receiving') {
            // If bar is not zero, set it
            if ($(`.radio-card#radio${idx} #rx-bar`).width() != '0') {$(`.radio-card#radio${idx} #rx-bar`).width('0')}
            return
        }
        // Get data
        radios[idx].audioSrc.analyzerNode.getFloatTimeDomainData(radios[idx].audioSrc.analyzerData);
        // Process into average amplitude
        var sumSquares = 0.0;
        for (const amplitude of radios[idx].audioSrc.analyzerData) { sumSquares += amplitude * amplitude; }
        // We calculate the geometric mean of these samples, and then multiply by an experimentally-found value to get approximately 0-100% scaling
        const newPct = String(Math.sqrt(sumSquares / radios[idx].audioSrc.analyzerData.length).toFixed(3) * 300);
        $(`.radio-card#radio${idx} #rx-bar`).width(newPct);
    });

    // Get data from mic
    audio.inputAnalyzer.getFloatTimeDomainData(audio.inputPcmData);

    // Input meter (only show when PTT)
    if (pttActive) {
        sumSquares = 0.0;
        for (const amplitude of audio.inputPcmData) { sumSquares += amplitude * amplitude; }
        const newPct = String(Math.sqrt(sumSquares / audio.outputPcmData.length).toFixed(3) * 300);
        // Apply to selected radio only
        $(`.radio-card#radio${selectedRadioIdx} #tx-bar`).width(newPct);
    } else {
        $(`.radio-card#radio${selectedRadioIdx} #tx-bar`).width('0');
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
    radios.forEach(function(radio, idx) {
        // Ignore if audio not connected
        if (radios[idx].audioSrc == null) {
            console.debug(`Audio not connected for radio ${radios[idx].name}, skipping`);
            return;
        }
        if (idx == selectedRadioIdx) {
            console.debug(`Radio ${radios[idx].name} is selected. Setting gain to 1`);
            radios[idx].audioSrc.gainNode.gain.setValueAtTime(1, audio.context.currentTime);
        } else {
            console.debug(`Radio ${radios[idx].name} is unselected. Setting gain to ${config.audio.unselectedVol}`);
            radios[idx].audioSrc.gainNode.gain.setValueAtTime(dbToGain(config.audio.unselectedVol), audio.context.currentTime);
        }
        // Set AGC based on user setting
        if (config.audio.rxAgc) {
            console.log(`Enabling AGC for radio ${radios[idx].name}`);
            radios[idx].audioSrc.agcNode.threshold.setValueAtTime(audio.agcThreshold, audio.context.currentTime);
            radios[idx].audioSrc.makeupNode.gain.setValueAtTime(audio.agcMakeup, audio.context.currentTime);
        } else {
            console.log(`Byassing AGC for radio ${radios[idx].name}`);
            radios[idx].audioSrc.agcNode.threshold.setValueAtTime(0, audio.context.currentTime);
            radios[idx].audioSrc.makeupNode.gain.setValueAtTime(1.0, audio.context.currentTime);
        }
    });
}

/**
 * Mutes the radio's audio at the given radio index
 * @param {int} idx index of the radio in radios[]
 * @param {bool} mute whether to mute or not
 */
function muteRadio(idx, mute) {
    if (mute) {
        radios[idx].audioSrc.muteNode.gain.setValueAtTime(0, audio.context.currentTime);
    } else {
        radios[idx].audioSrc.muteNode.gain.setValueAtTime(1, audio.context.currentTime);
    }
}

/**
 * Updates the status of the mute for each radio audio source
 */
function updateMute() {
    console.debug("Updating radio source mute statuses");
    radios.forEach(function(radio, idx) {
        // Ignore if we haven't created the node yet
        if (radios[idx].audioSrc == null) { 
            console.debug(`Audio not connected for radio ${radios[idx].name}, skipping`);
            return;
        }
        // Mute if we're muted or not receiving, after the specified delay in rtc.rxLatency
        if (radios[idx].status.muted || (radios[idx].status.state != 'Receiving')) {
            setTimeout(function() {
                console.debug(`Muting audio for radio ${radios[idx].name}`);
                radios[idx].audioSrc.muteNode.gain.setValueAtTime(0, audio.context.currentTime);
            }, rtcConf.rxLatency);
        } else {
            setTimeout(function() {
                console.debug(`Unmuting audio for radio ${radios[idx].name}`);
                radios[idx].audioSrc.muteNode.gain.setValueAtTime(1, audio.context.currentTime);
            }, rtcConf.rxLatency);
        }
    });
}

/**
 * Shows or hides the dropdown
 * @param {event} event 
 * @param {object} obj 
 */
function showPanMenu(event, obj) {
    const radioCard = $(obj).closest(".radio-card");
    if (radioCard.hasClass("disconnected")) {
        console.debug("Radio disconnected, not showing pan menu");
    } else {
        $(obj).closest(".radio-card").find(".panning-dropdown").toggleClass("closed");
    }
    event.stopPropagation();
}

function closeAllPanMenus() {
    $(".panning-dropdown").addClass("closed");
}

/**
 * Update the speaker pan for a radio (called by the slider value change)
 * @param {event} event the calling event
 * @param {object} obj the calling html object
 */
function changePan(event, obj) {
    // Prevent from selecting the card
    event.preventDefault();
    event.stopPropagation();
    event.stopImmediatePropagation();
    // Get new value
    const newPan = $(obj).val();
    // Get radio ID and index
    const radioId = $(obj).closest(".radio-card").attr('id');
    const idx = getRadioIndex(radioId);
    // Debug log
    console.debug(`Setting new pan for radio ${radios[idx]} to ${newPan}`);
    // Set pan
    radios[idx].audioSrc.panNode.pan.setValueAtTime(newPan, audio.context.currentTime);
}

/**
 * Center the pan for a radio (fired by slider doubleclick)
 * @param {event} event the calling button event 
 * @param {object} obj the calling html object
 */
function centerPan(event, obj) {
    // Update slider value
    $(obj).val(0);
    // Get radio index
    const radioId = $(obj).closest(".radio-card").attr('id');
    const idx = getRadioIndex(radioId);
    // Update pan
    console.debug(`Resetting pan for radio ${radios[idx]}`);
    // Set pan
    radios[idx].audioSrc.panNode.pan.setValueAtTime(0, audio.context.currentTime);
}

/***********************************************************************************
    Websocket Client Functions
***********************************************************************************/

/**
 * Create websocket connection to radio and wait for it to connect
 * @param {int} idx index of radio in radios[]
 */
 function connectRadio(idx) {
    // Log
    console.info(`Connecting to radio ${radios[idx].name}`);
    // Update radio connection icon
    $(`#radio${idx} .icon-connect`).removeClass('disconnected');
    $(`#radio${idx} .icon-connect`).addClass('connecting');
    $(`#radio${idx} .icon-connect`).parent().prop('title','Connecting to daemon');
    // Create audio context if we haven't already
    if (audio.context == null) {
        startAudioDevices();
    }
    // Create websocket
    radios[idx].wsConn = new WebSocket("wss://" + radios[idx].address + ":" + radios[idx].port);
    radios[idx].wsConn.onerror = function(event) { handleSocketError(event, idx) };
    radios[idx].wsConn.onmessage = function(event) { recvSocketMessage(event, idx) };
    radios[idx].wsConn.onclose = function(event) { handleSocketClose(event, idx) };
    // Wait for connection
    waitForWebSocket(radios[idx].wsConn, function() { onConnectWebsocket(idx) });
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
 * @param {int} idx index of radio in radios[]
 */
function onConnectWebsocket(idx) {
    //$("#navbar-status").html("Websocket connected");
    console.log(`Websocket connected for radio ${radios[idx].name}`);
    // Query radio status
    console.log(`Querying radio ${radios[idx].name} status`);
    radios[idx].wsConn.send(
        `{
            "radio": {
                "command": "query"
            }
        }`
    )
    // Start webrtc
    waitForRadioStatus(idx, function() { startWebRtc(idx) });
}

/**
 * Waits for the radiolist to be populated before calling callback
 * @param {int} idx index of radio in radios[]
 * @param {function} callback 
 */
function waitForRadioStatus(idx, callback) {
    setTimeout(
        function() {
            if (radios[idx].status.state != 'Disconnected') {
                if (callback != null) {
                    callback();
                }
            } else {
                waitForRadioStatus(idx, callback);
            }
        },
    5); // 5 ms timeout
}

/**
 * Disconnect from the websocket server
 * @param {int} idx radio index in radios[]
 */
function disconnectRadio(idx) {
    // Disconnect if we had a connection open
    if (radios[idx].wsConn.readyState == WebSocket.OPEN) {
        console.log(`Disconnecting from radio ${radios[idx].name}`);
        radios[idx].wsConn.close();
    }
}

/**
 * Callback for a new message from the websocket server and
 * parses the JSON command object. 
 * 
 * This command protocol is specified in `Docs/Websocket JSON Signalling.md`
 * @param {event} event 
 * @param {int} idx index of radio in radios[]
 */
function recvSocketMessage(event, idx) {
    // Convert to JSON
    var msgObj;
    try {
        msgObj = JSON.parse(event.data);
    } catch (e) {
        console.warn(`Got invalid data from radio ${radios[idx].name} websocket: ` + event.data);
        console.warn(e);
        return;
    }

    // Iterate through each message and its data (normally we'd only get one at a time, but I suppose you could get more than one)
    for (const [key, value] of Object.entries(msgObj)) {
        // Handle message data based on key type
        switch (key) {
            // Radio status update
            case "status":
                console.log(`Got status update for radio ${radios[idx].name}`);
                // get status data
                var radioStatus = msgObj['status'];
                //console.debug(radioStatus);
                // Strip out \u0000's from strings (TODO: figure out why python's decode method adds these and how to get rid of them)
                radioStatus['zone'] = radioStatus['zone'].replace(/\0/g, '');
                radioStatus['chan'] = radioStatus['chan'].replace(/\0/g, '');
                // Debug
                console.debug(radioStatus);
                // Update radio entry
                radios[idx].status = radioStatus;
                // Update radio card
                updateRadioCard(idx);
                // Update bottom controls
                updateRadioControls();
                // Update radio mute status
                updateMute(idx);
                break;

            // WebRTC SDP answer
            case "webRtcAnswer":
                // get params
                answerType = value['type'];
                answerSdp = value['sdp'];
                gotRtcResponse(idx,answerType,answerSdp);
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
 * @param {int} idx index of radio in radios[]
 */
function handleSocketClose(event, idx) {
    // Console warning
    console.warn(`Websocket connection closed to ${radios[idx].name}`);
    console.debug(event);
    if (event.data) {console.warn(event.data);}

    // Cleanup
    if (radios[idx].rtc != null) {
        stopWebRtc(idx);
    }
    radios[idx].wsConn = null;
    radios[idx].status.state = 'Disconnected';

    // UI Update
    $(`#radio${idx} .icon-connect`).removeClass('connected');
    $(`#radio${idx} .icon-connect`).removeClass('connecting');
    $(`#radio${idx} .icon-connect`).addClass('disconnected');
    $(`#radio${idx} .icon-connect`).parent().prop('title','Disconnected');
    updateRadioCard(idx);
}

/**
 * Handle connection errors from the server
 * @param {event} event 
 */
function handleSocketError(event, idx) {
    console.error(`Websocket connection error for radio ${radios[idx].name}`);
    console.debug(event);
    //window.alert("Server connection errror: " + event.data);
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