compile:
	clang -g hashScan.c md5/md5.c Core/core.c Strings/Strings.c Networking/Networking.c control/control.c -o hashScan -lcrypto -Wall -Werror -lcurl -lm
