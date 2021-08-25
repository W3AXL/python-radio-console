import numpy as np

class MuLaw:

    bias = 0x84
    clip = 32635

    encodeTable = [
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
    ]

    decodeTable = [0,132,396,924,1980,4092,8316,16764]

    def decode(muLawSamples):
        """
        Decode 8-bit mu-law samples to 32-bit floats

        Args:
            samples (Uint8[]): array of Uint8 samples

        Returns:
            np.float32[]: array of float samples
        """    

        # create output int16 numpy array
        output = np.zeros(len(muLawSamples), dtype=np.int16)

        # iterate through each sample and decode from Uint8 to Int16
        for idx, muLawSample in enumerate(muLawSamples):
            # make sure we have a python uint8
            muLawSample = muLawSample.astype(np.uint8).item()
            # Do the decoding
            muLawSample = ~muLawSample
            sign = (muLawSample & 0x80)
            exponent = (muLawSample >> 4) & 0x07
            mantissa = muLawSample & 0x0f
            sample = MuLaw.decodeTable[exponent] + (mantissa << (exponent + 3))
            if (sign != 0): sample = -sample
            output[idx] = sample

        # convert to float32 from int16 and return
        return output.astype(np.float32, order='C') / 32768.0

    def encode(samples):
        """
        Encode Float32 samples to 8-bit mu-law samples

        Args:
            samples (np.float32[]): float32 array of samples

        Returns:
            np.uint8[]: array of 8-bit mu-law samples
        """        

        # create output uint8 array
        output = np.zeros(len(samples), dtype=np.uint8)

        # convert float32 to int16
        int16samples = np.zeros(len(samples), dtype=np.int16)
        for idx, sample in enumerate(samples):
            i = sample * 32768
            if i > 32767: i = 32767
            if i < -32767: i = -32767
            int16samples[idx] = i

        # iterate through samples and encode
        for idx, sample in enumerate(int16samples):
            # Convert numpy int16 to python int16 so we can do bitwise stuff
            sample = sample.item()

            # Do the encoding stuff
            sign = (sample >> 8) & 0x80
            if (sign != 0): sample = -sample
            sample = sample + MuLaw.bias
            if (sample > MuLaw.clip): sample = MuLaw.clip
            exponent = MuLaw.encodeTable[(sample >> 7) & 0xff]
            mantissa = (sample >> (exponent + 3)) & 0x0f
            muLawSample = ~(sign | (exponent << 4) | mantissa)

            output[idx] = muLawSample

        return output
            