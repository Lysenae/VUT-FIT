.PHONY: all clean test pack download

CS=mcs
CLIENT=Client
SERVER=Server
CLIENT_EXE=ipkhttpclient
SERVER_EXE=ipkhttpserver

all: download $(CLIENT).o $(SERVER).o

$(CLIENT).o: $(CLIENT).cs
	$(CS) $(CLIENT).cs -out:$(CLIENT_EXE)

$(SERVER).o: $(SERVER).cs
	$(CS) $(SERVER).cs -out:$(SERVER_EXE)

clean:
	rm -rf tests
	rm -f $(CLIENT_EXE)
	rm -f $(SERVER_EXE)
	rm -f *.tgz
	rm -f ipkHttpClient-*
	rm -f ipkHttpServer-*

test: clean all
	bash do_test.sh

pack: clean
	tar -cvzf xklima22.tgz *

download:
	echo 'ipk2015' | sudo -S apt-get install --yes mono-devel
