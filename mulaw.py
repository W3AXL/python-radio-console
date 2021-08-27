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

    def __decodeSample(muLawSample):
        """
        Decode a mu-law encoded uint8 to an int16 audio sample

        Args:
            muLawSample (uint8): input mu-law encoded uint8 sample

        Returns:
            int16: output audio sample
        """
        # make sure we have a python uint8
        muLawSample = muLawSample.astype(np.uint8).item()
        # Do the decoding
        muLawSample = ~muLawSample
        sign = (muLawSample & 0x80)
        exponent = (muLawSample >> 4) & 0x07
        mantissa = muLawSample & 0x0f
        sample = MuLaw.decodeTable[exponent] + (mantissa << (exponent + 3))
        if (sign != 0): sample = -sample
        return sample

    def decode(muLawSamples):
        """
        Decode 8-bit mu-law samples to 32-bit floats

        Args:
            samples (Uint8[]): array of Uint8 samples

        Returns:
            np.float32[]: array of float samples
        """    

        # Decode the samples
        output = np.array(list(map(MuLaw.__decodeSample, muLawSamples)), dtype=np.int16)

        # convert to float32 from int16 and return
        return output.astype(np.float32, order='C') / 32768.0

    def __encodeSample(sample):
        """
        Convert an int16 sample to a mu-law encoded uint8 sample

        Args:
            sample (np.int16): input audio sample

        Returns:
            uint8: output mu-law encoded sample
        """

        # Convert numpy int16 to python int16
        sample = sample.item()
        sign = (sample >> 8) & 0x80
        if (sign != 0): sample = -sample
        sample = sample + MuLaw.bias
        if (sample > MuLaw.clip): sample = MuLaw.clip
        exponent = MuLaw.encodeTable[(sample >> 7) & 0xff]
        mantissa = (sample >> (exponent + 3)) & 0x0f
        return ~(sign | (exponent << 4) | mantissa)

    def encode(samples):
        """
        Encode Float32 samples to 8-bit mu-law samples

        Args:
            samples (np.float32[]): float32 array of samples

        Returns:
            np.uint8[]: array of 8-bit mu-law samples
        """        

        # convert float32 to int16 (list comprehension should be faster than a for loop)
        int16samples = np.clip([n * 32768 for n in samples], -32768, 32768).astype(np.int16)

        # Encode samples to mu-law
        output = np.array(list(map(MuLaw.__encodeSample, int16samples)), dtype=np.uint8)

        return output
            