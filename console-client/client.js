/*****************************************************
    Global Variables
*****************************************************/

// User Config Var
var config = {
    timeFormat: "Local",

    serverAddress: "",
    serverPort: 0,
    serverAutoConn: false
}

// Radio List (populated from server)
var radioList = [];

// TCP Connection to server
var serverSocket;

/*****************************************************
    State variables
*****************************************************/

// Detected timezone
timeZone = "";
// Selected radio
var selectedRadio = null;
// PTT state
var pttActive = false;
// Menu state
var menuOpen = false;

/*****************************************************
    Page Setup Functions
*****************************************************/

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
    // Populate radio cards
    populateRadios();
    // Setup clock timer
    setInterval(updateClock, 250);
    // Bind buttons
    bindRadioCardButtons();
    // Bind body click to deselecting radios
    $("#body").click(function () {
        deselectRadios();
    });
}

// Keydown handler
$(document).on("keydown", function (e) {
    switch (e.which) {
        // Spacebar
        case 32:
            startPtt();
            break;
    }
});

// Keyup handler
$(document).on("keyup", function (e) {
    switch (e.which) {
        // Spacebar
        case 32:
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

/*****************************************************
    Radio UI Functions
*****************************************************/

/**
 * Select a radio
 * @param {string} id the id of the radio to select
 */
function selectRadio(id) {
    // Deselect all radio cards
    deselectRadios();
    // Select the new radio card
    $(`#${id}`).addClass("selected");
    // Update the variable
    selectedRadio = id;
    // Update controls
    updateRadioControls();
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
 * Populate radio cards based on the radios in radioList[]
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
 * Add a radio card with the specified id and name
 * @param {string} id ID of the card element
 * @param {string} name Name to display in header
 */
function addRadioCard(id, name) {
    var newCardHtml = `
        <div class="radio-card" id="${id}">
            <div class="header">
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
        var cardId = $(this).attr('id');
        selectRadio(cardId);
        // Prevent the #body deselect from firing
        event.stopPropagation();
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

/*****************************************************
    Radio Backend Functions
*****************************************************/

/**
 * Start radio PTT
 */
function startPtt() {
    if (!pttActive && selectedRadio) {
        console.log("Starting PTT on " + selectedRadio);
        $("#" + selectedRadio).addClass("transmitting");
        pttActive = true;
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
        $("#" + selectedRadio).removeClass("transmitting");
        console.log("PTT released");
        pttActive = false;
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

/*****************************************************
    Global UI Functions
*****************************************************/

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

/*****************************************************
    Global Backend Functions
*****************************************************/

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

/*****************************************************
    Websocket Client Functions
*****************************************************/

/**
 * Connect to the server's websocket
 */
function connectServer() {
    serverSocket = new WebSocket("ws://localhost:9995");
    serverSocket.onerror = handleConnectionError;
    serverSocket.onmessage = recvServerMessage;
}

/**
 * Disconnect from the websocket server
 */
function disconnectServer() {
    if (serverSocket.readyState == WebSocket.OPEN) {
        serverSocket.close();
    }
}

/**
 * Callback for a new message from the websocket server
 * @param {event} event 
 */
function recvServerMessage(event) {

    // Response to ?radios request
    if (event.data.startsWith("radios:")) {
        // get the JSON of the current radios
        var statusJson = event.data.substring(7);
        // set our radio list to the new status
        radioList = JSON.parse(statusJson);
        // Populate radio cards
        populateRadios()
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
function sendServerMessage(message) {
    serverSocket.send(message);
}

/**
 * Handle connection errors from the server
 * @param {event} event 
 */
function handleConnectionError(event) {
    console.error("TCP connection to server error: ", event);
}