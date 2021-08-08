// Config
var config = {
    timeFormat: "Local",
    timeZone: ""
}

// Menu var
var menuOpen = false;

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
}

/**
 * Toggles the state of the sidebar menu
 */
function toggleMenu() {
    if (menuOpen) {
        
    } else {

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