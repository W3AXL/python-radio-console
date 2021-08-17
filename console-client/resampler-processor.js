/**
* Resampler processor for resampling audio data
* @extends AudioWorkletProcessor
*/
registerProcessor('resampler-processor', class extends AudioWorkletProcessor {

    // constructor
    constructor() {
        super();
    }

    // process function
    process(inputs, outputs, parameters) {
        // We assume only one item here
        const input = inputs[0];
        const output = outputs[0];

        if (input.length > 0) {
            // Get our mono channels
            const inputChannel = input[0];
            const outputChannel = output[0];

            for (let i = 0; i < inputChannel.length; i++) {
                // resampler code goes here
            }
        }

        return true;
    }

});