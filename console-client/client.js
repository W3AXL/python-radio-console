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
    spkrBufferDur: 0.1,
    micBufferDur: 0.1,
    // audio transfer sample rate
    transferSamplerate: 16000,
    // Input device, buffer, resampler, and processor
    input: null,
    inputStream: null,
    inputBuffer: null,
    inputBufferSize: 0,
    inputProcessor: null,
    // Output device, buffer, and processor
    output: null,
    outputBuffer: null,
    outputProcessor: null,
}

testInput = null,

/***********************************************************************************
    State variables
***********************************************************************************/

// Detected timezone
timeZone = "";
// Selected radio
var selectedRadio = null;
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
    // Connect websocket first
    connectWebsocket();
    // Wait for connect to start audio devices if they haven't already started
    if (!audio.context) {
        waitForWebSocket(serverSocket, startAudioDevices);
    }
}

/**
 * Disconnect from websocket and teardown audio devices
 */
function disconnect() {
    // disconnect websocket
    disconnectWebsocket();
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
        // Update the variable
        selectedRadio = null;
    } else {
        // Deselect all radio cards
        deselectRadios();
        // Select the new radio card
        $(`#${id}`).addClass("selected");
        // Update the variable
        selectedRadio = id;
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
    } else {
        radioCard.find("#icon-mute").attr('name', 'volume-high-sharp');
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
        // Get the radio index
        var idx = getRadioIndex(selectedRadio);
        // Get the radio from the list
        var radio = radioList[idx];
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
        // Only send the TX command if we have a valid socket
        if (serverSocket) {
            serverSocket.send("!startTx:" + String(getRadioIndex(selectedRadio)));
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
            serverSocket.send("!stopTx:" + String(getRadioIndex(selectedRadio)));
        }
    }
}

/**
 * Change channel on selected radio
 * @param {bool} down Whether to go down or not
 */
function changeChannel(down) {
    if (!pttActive && selectedRadio && serverSocket) {
        if (down) {
            console.log("Changing channel down on " + selectedRadio);
            serverSocket.send("!chanDn:" + String(getRadioIndex(selectedRadio)));
        } else {
            console.log("Changing channel up on " + selectedRadio);
            serverSocket.send("!chanUp:" + String(getRadioIndex(selectedRadio)));
        }
    }
}

/**
 * Toggle monitor
 */
function toggleMonitor() {
    if (!pttActive && selectedRadio && serverSocket) {
        console.log("Toggling monitor on " + selectedRadio);
        serverSocket.send("!mon:" + String(getRadioIndex(selectedRadio)));
    }
}

/**
 * Hit nuisance delete button
 */
 function nuisanceDelete() {
    if (!pttActive && selectedRadio && serverSocket) {
        console.log("Nuisance delete: " + selectedRadio);
        serverSocket.send("!nuis:" + String(getRadioIndex(selectedRadio)));
    }
}

/**
 * Toggle low power
 */
function togglePower() {
    if (!pttActive && selectedRadio && serverSocket) {
        console.log("Toggling power on " + selectedRadio);
        serverSocket.send("!lpwr:" + String(getRadioIndex(selectedRadio)));
    }
}

/**
 * Turn scan on or off for selected radio
 */
function toggleScan() {
    if (!pttActive && selectedRadio && serverSocket) {
        console.log("Toggling scan for " + selectedRadio);
        serverSocket.send("!scan:" + String(getRadioIndex(selectedRadio)));
    }
}

/**
 * Toggle talkaround (I should really standardize on Talkaround vs Direct)
 */
function toggleDirect() {
    if (!pttActive && selectedRadio && serverSocket) {
        console.log("Toggling talkaround for " + selectedRadio);
        serverSocket.send("!dir:" + String(getRadioIndex(selectedRadio)));
    }
}

/**
 * Toggle the status of radio mute
 * @param {string} obj element whose parent radio to toggle mute on
 */
function toggleMute(event, obj) {
    // Get ID of radio to mute
    var radioId = $(obj).closest(".radio-card").attr('id');
    // Get index of radio in list
    var idx = getRadioIndex(radioId);
    // Change mute status
    if (radioList[idx].muted) {
        console.log("Unmuting " + radioId);
        radioList[idx].muted = false;
    } else {
        console.log("Muting " + radioId);
        radioList[idx].muted = true;
    }
    // Update card
    updateRadioCard(idx);
    // Stop propagation so we don't also select the muted radio
    event.stopPropagation();
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
    Audio Handling Function
***********************************************************************************/

/** 
* Setup audio devices and context, etc
* @return {bool} True on success
*/
function startAudioDevices() {
    // Create audio context
    audio.context = new AudioContext();
    console.log("Created audio context");

    // Find the right getUserMedia()
    if (!navigator.getUserMedia) {
        navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia;
    }
    // See if we got one
    if (navigator.getUserMedia) {
        // Get the microphone
        navigator.getUserMedia({audio:true},
            function(stream) {
                startMicrophone(stream);
                return true;
            },
            function(e) {
                alert('Error capturing microphone device');
                return false;
            }
        );
    } else {
        alert('Cannot capture microphone: getUserMedia() not supported in this browser');
    }
}

/**
 * Start capturing the microphone
 * @param {MediaStream} stream the MediaStream for the microphone
 */
function startMicrophone(stream) {
    console.log("Starting microphone");

    // Create an empty buffer
    audio.inputBuffer = "";

    // Create input stream source
    audio.input = audio.context.createMediaStreamSource(stream);
    
    // Import the microphone processor (async function so we do .then())
    audio.context.audioWorklet.addModule('microphone-processor.js').then(() => {
        // create a new node of it
        audio.inputProcessor = new AudioWorkletNode(audio.context, 'microphone-processor');
        // Bind the data handler
        audio.inputProcessor.port.onmessage = event => {
            sendMicData(event.data);
        }
        // connect everything together
        audio.input.connect(audio.inputProcessor);
    });

    // Tell the server we're ready to do audio things
    serverSocket.send("!startAudio");
}

/**
 * Buffer and send the mic data string to the server
 * @param {Float32Array} data Float32 intput samples
 */
function sendMicData(data) {
    // Convert the Float32Array data to a Mu-Law Uint8Array
    var muLawData = encodeMuLaw(data);
    // Convert this mu-law data to a comma-separated string
    var dataString = "";
    muLawData.forEach(function(element) {
        dataString += (element.toString() + ",");
    });
    // only send stuff if we're actually PTTing into a radio
    if (pttActive && serverSocket && selectedRadio) {
        // Add data to buffer (concat keeps dimensions correct)
        audio.inputBuffer += dataString;
        audio.inputBufferSize += 1;
        // Push data if buffer is full
        if (audio.inputBufferSize >= audio.micBufferDur * audio.transferSamplerate) {
            // Send string
            serverSocket.send("micAudio:" + audio.inputBuffer);
            // Clear buffer
            audio.inputBuffer = "";
            audio.inputBufferSize = 0;
        }
    }
}

function getSpkrData(dataString) {
        // Convert the comma-separated string of mu-law samples to a Uint8Array
        const spkrMuLawData = Uint8Array.from(dataString.split(','));
        // Decode to Float32Array
        const spkrData = decodeMuLaw(spkrMuLawData);
        // Resample to client samplerate
        const resampled = waveResampler.resample(spkrData, audio.transferSamplerate, audio.context.sampleRate, {method: "point"});
        const resampledFloat32 = Float32Array.from(resampled);
        // Create a new buffer and source to play the received data
        const buffer = audio.context.createBuffer(1, resampledFloat32.length, audio.context.sampleRate);
        buffer.copyToChannel(resampledFloat32, 0);
        var source = audio.context.createBufferSource();
        source.buffer = buffer;
        source.connect(audio.context.destination);
        source.start(0);
}

/***********************************************************************************
    Audio Encoding/Decoding Functions

    Borrowed from many places, including:
        - https://github.com/rochars/alawmulaw/blob/master/lib/mulaw.js

***********************************************************************************/

const muLawClip = 32635;

const muLawBias = 0x84;

const muLawEncodeTable = [
    0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
    5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
];

const muLawDecodeTable = [0, 132, 396, 924, 1980, 4092, 8316, 16764];

/**
 * Convert a Float32 array of samples to mu-law encoded 8 bit samples
 * @param {Float32Array} samples input Float32 samples
 * @returns {Uint8Array} array of Mu-Law encoded samples
 */
function encodeMuLaw(samples) {
    // Convert input float samples to PCM
    var intSamples = floatToPcm(samples);
    // Create output sample array
    var output = new Uint8Array(samples.length);
    // Process each sample with the discrete mu-law encoding algorithm
    intSamples.forEach((sample, idx) => {
        // int vars (saves memory I guess?)
        let sign, exponent, mantissa;
        // get sign/magnitude
        sign = (sample >> 8) & 0x80;
        if (sign != 0) sample = -sample;
        // mu-law algorithm
        sample = sample + muLawBias;
        if (sample > muLawClip) sample = muLawClip;
        exponent = muLawEncodeTable[(sample>>7) & 0xFF];
        mantissa = (sample >> (exponent+3)) & 0x0F;
        output[idx] = ~(sign | (exponent << 4) | mantissa);
    });
    // return
    return output;
}

/**
 * Convert mu-law encoded Uint8 samples to Float32 samples
 * @param {Uint8Array} samples input Mu-law Uint8 samples
 * @returns {Float32Array} output Float32 samples
 */
function decodeMuLaw(samples) {
    // Create output array
    var output = new Float32Array(samples.length);
    // Iterate over input array and decode mu-law
    samples.forEach((muLawSample, idx) => {
        // int vars
        let sign, exponent, mantissa;
        // do mu-law stuff
        muLawSample = ~muLawSample;
        sign = (muLawSample & 0x80);
        exponent = (muLawSample >> 4) & 0x07;
        mantissa = muLawSample & 0x0F;
        sample = muLawDecodeTable[exponent] + (mantissa << (exponent + 3));
        if (sign != 0) sample = -sample;
        output[idx] = sample;
    });
    // Return float samples
    return pcmToFloat(output);
}

/**
 * Convert float32 samples to 16-bit PCM by clamping to [-1, 1],
 * multiplying by 32768, and taking the integer portion
 * @param {Float32Array} samples array of input Float32 samples
 * @returns {Int16Array} array of output signed PCM samples
 */
function floatToPcm(samples) {
    var output = new Int16Array(samples.length);
    samples.forEach((itm, idx) => {
        output[idx] = Math.floor(Math.max(-1, Math.min(itm, 1)) * 32768);
    });
    return output;
}

/**
 * Convert Int16 PCM samples to Float 32 samples by dividing
 * 32768 and clamping to [-1, 1]
 * @param {Int16Array} samples array of input Int16 samples
 * @returns {Float32Array} array of output Float32 samples
 */
function pcmToFloat(samples) {
    var output = new Float32Array(samples.length);
    samples.forEach((itm, idx) => {
        output[idx] = Math.max(-1, Math.min(itm / 32768, 1));
    });
    return output;
}

/***********************************************************************************
    Websocket Client Functions
***********************************************************************************/

/**
 * Connect to the server's websocket
 */
function connectWebsocket() {
    // Change button
    $("#server-connect-btn").html("Connecting...");
    $("#server-connect-btn").prop("disabled",true);
    // Log
    console.log("Connecting to " + config.serverAddress + ":" + config.serverPort);
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
    console.log("Connected!");
    // Change button
    $("#server-connect-btn").html("Disconnect");
    $("#server-connect-btn").prop("disabled",false);
    // Change status
    $("#navbar-status").html("Connected");
    $("#navbar-status").removeClass("pending");
    $("#navbar-status").addClass("connected");
    // Query for radios
    sendSocketMessage('?radios');
}

/**
 * Disconnect from the websocket server
 */
function disconnectWebsocket() {
    // Change button
    $("#server-connect-btn").html("Disconnecting...");
    $("#server-connect-btn").prop("disabled", true);
    // Change status
    $("#navbar-status").html("Disconnecting");
    $("#navbar-status").removeClass("connected");
    $("#navbar-status").addClass("pending");
    // Disconnect if we had a connection open
    if (serverSocket.readyState == WebSocket.OPEN) {
        console.log("Disconnecting from server");
        disconnecting = true;
        serverSocket.close();
    }
}

/**
 * Callback for a new message from the websocket server
 * @param {event} event 
 */
function recvSocketMessage(event) {

    // Response to ?radios request
    if (event.data.startsWith("radios:")) {
        console.log("Got master radio list update");
        // get the JSON of the current radios
        var radioListJson = event.data.substring(7);
        // set our radio list to the new status
        radioList = JSON.parse(radioListJson);
        // Populate radio cards
        populateRadios();
        // Bind buttons
        bindRadioCardButtons();
    }

    // single adio status update
    else if (event.data.startsWith("radio")) {
        // Get radio ID from status message
        var radioIndex = event.data.substring(5, event.data.lastIndexOf(':{'));
        console.log("Got status update for radio" + radioIndex);
        // Slice JSON from message and parse
        var radioStatusJson = event.data.substring(6 + radioIndex.length);
        var radioStatus = JSON.parse(radioStatusJson);
        // Strip out \u0000's from strings (TODO: figure out why python's decode method adds these and how to get rid of them)
        radioStatus['zone'] = radioStatus['zone'].replace(/\0/g, '');
        radioStatus['chan'] = radioStatus['chan'].replace(/\0/g, '');
        //console.log(radioStatus);
        // Update radio entry
        radioList[radioIndex] = radioStatus;
        // Update radio card
        updateRadioCard(radioIndex);
        // Update bottom controls
        updateRadioControls();
    }

    // Speaker audio data
    else if (event.data.startsWith("spkrAud:")) {
        // Get string of comma-separated floats
        var dataString = event.data.substring(8);
        // send to speaker data function
        getSpkrData(dataString);
    }

    // Message error
    else if (event.data == "NACK") {
        console.error("Got NACK from server");
    }

    // Unknown message handler
    else {
        console.warn("Got unknown message from server: " + event.data);
    }
}

/**
 * Send a message to the websocket server
 * @param {string} message message to send
 */
function sendSocketMessage(message) {
    serverSocket.send(message);
}

/**
 * Handle the websocket closing
 * @param {event} event socket closed event
 */
function handleSocketClose(event) {
    console.warn("Server connection closed");
    if (event.data) {console.warn(event.data);}
    if (!disconnecting) {
        window.alert("Lost connection to server!");
    }
    // Update button
    $("#server-connect-btn").html("Connect");
    $("#server-connect-btn").prop("disabled", false);
    // Change status
    $("#navbar-status").html("Disconnected");
    $("#navbar-status").removeClass("connected");
    $("#navbar-status").removeClass("pending");
    // Clear radio cards
    clearRadios();
    // Reset variables
    disconnecting = false;
    serverSocket = null;
}

/**
 * Handle connection errors from the server
 * @param {event} event 
 */
function handleSocketError(event) {
    console.error("Server connection error: " + event.data);
    window.alert("Server connection errror: " + event.data);
}