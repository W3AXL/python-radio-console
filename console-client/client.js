/*****************************************************
    Global Variables
*****************************************************/

// User Config Var
var config = {
    timeFormat: "Local",
}

// Radio List (TODO: This will be populated from the server)
var radioList = [
    {
        name: "VHF XTL5000",
        muted: false,
        error: false,
    },
    {
        name: "UHF XTL5000",
        muted: false,
        error: false,
    },
];

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
    // Get client timezone
    const d = new Date();
    timeZone = d.toLocaleString('en', {timeZoneName: 'short'}).split(' ').pop();
    // Setup clock timer
    setInterval(updateClock, 250);
    // Bind buttons
    bindRadioCardButtons();
    // Bind body click to deselecting radios
    $("#body").click(function() {
        deselectRadios();
    });
}

/**
 * Binds radio card buttons & click events to functions
 */
function bindRadioCardButtons() {
    // Bind clicking of the card to selection of a radio
    $(".radio-card").on('click', function(event) {
        var cardId = $(this).attr('id');
        selectRadio(cardId);
        // Prevent the #body deselect from firing
        event.stopPropagation();
    })
    // Bind the minimize button
    $(".minimize-radio-card").click(function(event) {
        
    })
}

// Keydown handler
$(document).on("keydown", function(e) {
    switch (e.which) {
        // Spacebar
        case 32:
            startPtt();
            break;
    }
});

// Keyup handler
$(document).on("keyup", function(e) {
    switch (e.which) {
        // Spacebar
        case 32:
            stopPtt();
            break;
    }
});

// Handle losing focus of the window
$(window).blur(function() {
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
}

function toggleMute(obj) {
    // Get ID of radio to mute
    var radio = $(obj).closest(".radio-card").attr('id');
    console.log()
    // Update the UI to show muted
    $(obj).find("ion-icon").attr("name","volume-mute-sharp");
}

/*****************************************************
    Radio Backend Functions
*****************************************************/

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

function stopPtt() {
    if (pttActive) {
        $("#" + selectedRadio).removeClass("transmitting");
        console.log("PTT released");
        pttActive = false;
    }
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
function closePopup(obj=null) {
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
        $("#clock").html(getTimeUTC(timestr + "UTC"));
    } else {
        console.error("Invalid time format!")
    }
}

/**
 * Populate radio cards based on the radios in radioList[]
 */
function populateRadios() {
    radioList.foreach((radio, index) => {
        addRadioCard("radio" + String(index), radio.name);
    });
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
                    <a href="#"><ion-icon name="volume-high-sharp" onclick="muteRadio(this)"></ion-icon></a>
                    <a href="#"><ion-icon name="warning-sharp"></ion-icon></ion-icon></ion-icon></a>
                </div>
            </div>
            <div class="content">
                <div class="channel">
                    <h3>Channel</h3>
                    <span class="channel-text">Channel Text</span>
                </div>
                <div class="last-id">
                    <h3>Last ID</h3>
                    <span class="id-text">ID Text</span>
                </div>
            </div>
        </div>
    `;
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