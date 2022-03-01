# krw_util
This library is an interface for all operating systems that need to perform kernel read write operations.
This means that windows, linux, android, mac and ios all call to this interface to then go to their respective
libraries. To be honest, thinking of scrapping it, but it allows be to rely on one definition, kernel_xxx
instead of having to implement each kernel routine or header beforehand.