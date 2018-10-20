# Autor: Daniel Klimaj; xklima22@stud.fit.vutbr.cz
# Popis: Trieda Tracker pre pracu s torrent trackermi

import struct
import socket
import time
import re

import bencode.bencode as bnc
import requests.packages.urllib3 as urllib3

from log import Log as Log

# Nastavenia
SLEEP_TIME      = 2
TRIES           = 3
RECV_SIZE       = 4096
RECV_TIMEOUT    = 2
CONNECT_TIMEOUT = 7

class Tracker(object):

    def __init__(self, url, torrent=None):
        self.url       = url
        self.host      = None
        self.host_ip   = None
        self.port      = 80
        self.path      = "/"
        self.socket    = None
        self.query     = None
        self.info_hash = None

        self._parse_url(url)
        self._get_torrent_data(torrent)

    def _parse_url(self, url):
        '''
        Ziskanie informacii z announce URL.
        '''

        parsed = urllib3.util.parse_url(url)

        self.host    = parsed.hostname
        addr = []
        try:
            addr = socket.gethostbyname_ex(self.host)[2]
        except:
            Log.error("Tracker: Nejde ziskat IP adresu trackeru")
        if len(addr) > 0: self.host_ip = addr[0]
        self.query   = parsed.query

        if parsed.port:
            self.port = parsed.port

        if parsed.path:
            self.path = parsed.path
            self.path = re.sub(r'\?.*', '', self.path)

    def _get_torrent_data(self, torrent):
        '''
        Ziskanie dodatocnych informacii z torrentu.
        '''

        if torrent:
            self.info_hash = torrent.info_hash_url

    def connect(self):
        '''
        Pripojenie sa k trackeru.
        @return True ak je spojenie nadviazane
        '''

        Log.info("Tracker: Pokusam sa o pripojenie (timeout = {}s)".format(CONNECT_TIMEOUT))
        if self.host_ip:
            try:
                self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.socket.settimeout(CONNECT_TIMEOUT)
                self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                self.socket.connect((self.host_ip, self.port))
                return True
            except Exception as e:
                Log.error("Tracker: Nepodarilo sa pripojit k trackeru ({})".format(e))
                return False
        else:
            Log.error("Tracker: Chybna URL trackeru")
            return False

    def disconnect(self):
        '''
        Odpojenie sa od trackeru.
        '''

        if self.socket:
            self.socket.close()

    def get_peerlist(self):
        '''
        Ziskanie peerlistu z trackeru.
        '''

        if self.socket:
            data         = ""
            cdata        = []
            tries        = TRIES
            query_string = self.query

            if not query_string:
                query_string = "?info_hash={}&peer_id=AAAAABBBBBCCCCCDDDDD&downloaded=0&uploaded=0&left=1&port=6881&compact=1".format(self.info_hash)

            Log.debug("Tracker: Odosielana poziadavka: {}".format(query_string))
            request_string = \
                "GET " + self.path + query_string +" HTTP/1.1\r\n" + \
                "Connection: keep-alive\r\n" + \
                "Accept-Encoding: gzip\r\n" + \
                "\r\n"

            while tries > 0:
                try_nr = TRIES-tries+1
                try:
                    self.socket.sendall(request_string)
                except:
                    Log.error("Tracker: Pokus {}/{}: Odoslanie poziadavku na tracker zlyhalo".format(try_nr, TRIES))
                    tries -= 1
                    if tries > 0: time.sleep(SLEEP_TIME)
                    continue

                # http://www.binarytides.com/receive-full-data-with-the-recv-socket-function-in-python/
                begin  = time.time()
                errflg = False
                while True:
                    if cdata and time.time() - begin > RECV_TIMEOUT:
                        break
                    elif time.time() - begin > RECV_TIMEOUT*2:
                        break

                    try:
                        data = (self.socket.recv(RECV_SIZE))
                        if data:
                            cdata.append(data)
                            begin = time.time()
                        else:
                            time.sleep(0.1)
                    except socket.timeout:
                        Log.error("Tracker: Pokus {}/{}: Bol dosiahnuty timeout (socket.timeout)".format(try_nr, TRIES))
                        errflg = True
                        break
                    except socket.error:
                        Log.error("Tracker: Pokus {}/{}: Chyba socketu (socket.error)".format(try_nr, TRIES))
                        errflg = True
                        break
                    except:
                        Log.error("Tracker: Pokus {}/{}: Neznama chyba pri komunikacii s trackerom".format(try_nr, TRIES))
                        errflg = True
                        break

                if errflg:
                    tries -= 1
                    if tries > 0: time.sleep(SLEEP_TIME)
                    continue

                data = ''.join(cdata)

                # Zisti navratovy status kod
                status = re.search(r'HTTP\/1\.\d\s(.*?)\r\n', data)
                if not status:
                    Log.error("Tracker: Pokus {}/{}: Neplatna/nezmyslena odpoved serveru".format(try_nr, TRIES))
                    tries -= 1
                    if tries > 0: time.sleep(SLEEP_TIME)
                    continue

                if status.group(1) is not None and status.group(1) == "200 OK":
                    bstring  = re.search(r'\r\n\r\n(.*)', data, re.DOTALL)
                    if bstring:
                        bstring = bstring.group(1)
                    else:
                        Log.error("Tracker: Pokus {}/{}: Nepodarilo sa ziskat peerlist z odpovede serveru".format(try_nr, TRIES))
                        tries -= 1
                        if tries > 0: time.sleep(SLEEP_TIME)
                        continue

                    # Obcas moze nastat chyba pri bdecode
                    bdecoded = None
                    try:
                        bdecoded = bnc.bdecode(bstring)
                    except:
                        Log.error("Tracker: Pokus {}/{}: Nepodarilo sa dekodovat bencode z odpovede trackeru \"{}\"".format(try_nr, TRIES, bstring))
                        tries -= 1
                        if tries > 0: time.sleep(SLEEP_TIME)
                        continue

                    bpeers = None
                    try:
                        bpeers   = bdecoded['peers']
                    except:
                        Log.error("Tracker: Pokus {}/{}: V odpovedi trackeru nie je kluc peers ".format(try_nr, TRIES))
                        tries -= 1
                        if tries > 0: time.sleep(SLEEP_TIME)
                        continue

                    offset   = 0
                    peers    = []

                    # Dekoduj peerlist (!i = int, !H = ushort)
                    try:
                        while offset < len(bpeers):
                            ip      = struct.unpack_from("!i", bpeers, offset)[0]
                            ip      = socket.inet_ntoa(struct.pack("!i", ip))
                            offset += 4
                            port    = struct.unpack_from("!H", bpeers, offset)[0]
                            offset += 2
                            peers.append("{}:{}".format(ip, port))
                    except:
                        Log.error("Tracker: Pokus {}/{}: Chyba pri dekodovani peerlistu".format(try_nr, TRIES))
                        tries -= 1
                        if tries > 0: time.sleep(SLEEP_TIME)
                        continue

                    return peers
                else:
                    Log.error("Tracker: Pokus {}/{}: Vrateny stavovy kod \'{}\'".format(try_nr, TRIES, status.group(1)))
                    tries -= 1
                    if tries > 0: time.sleep(SLEEP_TIME)
                    continue

            Log.error("Tracker: Dosiahnuty maximalny pocet pokusov - Neboli ziskane ziadne data z trackeru")
            return None

# tracker.py
