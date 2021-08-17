### Making the HTTPS connection work

The certificates included here were generated using the guide from:
https://stackoverflow.com/questions/7580508/getting-chrome-to-accept-self-signed-localhost-certificate

An SSL connection is required to use some of the javascript web audio APIs

You'll have to trust the certs/localCA.pem certificate as a Trusted Root Certificate Authority on you client machine