/**
* Microphone AudioWorklet for handling microphone data
* @extends AudioWorkletProcessor
*/
registerProcessor('microphone-processor', class extends AudioWorkletProcessor {

    // constructor
    constructor() {
        super();
    }

    // process function
    process(inputs, outputs, parameters) {
        // get first channel of first input (ignore the others, if any, since we only want mono mic audio)
        const inputData = inputs[0][0];
        // Convert the Float32Array to an array string, rounded to 4 decimal places
        var dataString = "";
        inputData.forEach(function(element) {
            dataString += (element.toFixed(4) + ",");
        });
        // Send this data string back to the main script via the port
        this.port.postMessage(dataString);
        return true;
    }

});