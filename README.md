# OPsystem-OTP
This program is conistant of 5 seperate files. Each help to result in a encription/decription of any text provided.

keygen.c (./keygen): this provides a random key that will help the other programs to encript and decript

otp_enc_d.c (./otp_enc_d): This serves as a server. Ran in the background with a port num as argument. Someone can connect to a port while this server is running and send it a message and a key and it will returned a encripted message.

otp_dec_d.c (./otp_dec_d): This serves as a server. Ran in the baclground with a port number as argument. Someone can connect to a port while this server is running and send it a encripted message and a key and it will return the original message.

otp_enc.c (./otp_enc): This is used to connect to the otp_enc_d server. This takes a file with a message of only letter chars and spaces, a key, and a port number.

otp_dec.c (./otp_dec): This is used to connect to the otp_dec_d server. This takes a file with an encripted message, a key and a port number.

./compileall: This is a bash script to compile all the code parts.
