/*****************************************************
    Global Variables
*****************************************************/

// User Config Var
var config = {
    timeFormat: "Local",
}

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