// Config
var config = {
    timeFormat: "Local",
    timeZone: ""
}

// Menu var
var menuOpen = false;

// Selected radio
var selectedRadio = "";

/**
 * Page load function. Starts timers, etc.
 */
function pageLoad() {
    console.log("Starting client-side runtime");
    // Get client timezone
    const d = new Date();
    config.timeZone = d.toLocaleString('en', {timeZoneName: 'short'}).split(' ').pop();
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
        console.log("Radio " + cardId + " selected");
        selectRadio(cardId);
        // Prevent the #body deselect from firing
        event.stopPropagation();
    })
    // Bind the minimize button
    $(".minimize-radio-card").click(function(event) {
        
    })
}

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
    $(".radio-card").removeClass("selected");
    selectedRadio = "";
}

/**
 * Toggles the state of the sidebar menu
 */
function toggleMainMenu() {
    if (menuOpen) {
        console.log("Hiding main menu");
        $("#sidebar-mainmenu").addClass("sidebar-closed");
        $("#button-mainmenu").removeClass("button-active")
        menuOpen = false;
    } else {
        console.log("Showing main menu");
        $("#sidebar-mainmenu").removeClass("sidebar-closed");
        $("#button-mainmenu").addClass("button-active")
        menuOpen = true;
    }
}

/**
 * Update the clock based on the selected time format
 */
function updateClock() {
    var timestr = "HH:mm:ss"
    if (config.timeFormat == "Local") {
        var time = getTimeLocal(timestr);
        $("#clock").html(time + " " + config.timeZone);
    } else if (config.timeFormat == "UTC") {
        $("#clock").html(getTimeUTC(timestr + "UTC"));
    } else {
        console.error("Invalid time format!")
    }
}

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

// Bind pageLoad function to document load
$(document).ready(pageLoad());