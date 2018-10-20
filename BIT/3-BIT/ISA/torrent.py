# Autor: Daniel Klimaj; xklima22@stud.fit.vutbr.cz
# Popis: Trieda Torrent pre ziskavanie dat z .torrent suborov

import os
import sys
import hashlib
import urllib

import bencode.bencode as bnc

from log import Log as Log

class Torrent(object):

    def __init__(self, path):
        self.path          = path
        self.announce_list = []
        self.http_trackers = []
        self.info_hash     = ""
        self.info_hash_url = ""
        self.name          = ""
        self.valid         = False

        if self.verify_path():
            if self.decode():
                self.get_http_trackers()
                self.valid = True

    def verify_path(self):
        '''
        Overenie existencie torrentu.
        @return True ak torrent existuje
        '''

        if(os.path.exists(self.path)):
            self.path = os.path.abspath(self.path)
            return True

        Log.error("Torrent: Neplatna cesta k torrentu {}".format(self.path))
        self.path = None
        return False

    def decode(self):
        '''
        Dekodovanie bencode torrentu.
        @return True v pridade uspesneho spracovania
        '''

        torrent, bdictionary = None, None

        try:
            torrent = open(self.path, 'rb')
        except:
            Log.error("Torrent: Nepodarilo sa otvorit torrent {}".format(self.path))
            return False

        try:
            bdictionary = bnc.bdecode(torrent.read())
        except:
            Log.error("Torrent: Nepodarilo sa dekodovat bencode v torrente {}".format(self.path))
            return False

        # Spracovanie info sekcie
        info               = bdictionary['info']
        self.info_hash     = hashlib.sha1(bnc.bencode(info)).hexdigest()
        self.info_hash_url = urllib.quote_plus(hashlib.sha1(bnc.bencode(info)).digest())
        self.name          = info['name']

        # Ziskanie trackerov
        try:
            announce_list = bdictionary['announce-list']
            for al in announce_list:
                if isinstance(al, list):
                    for a in al:
                        self.announce_list.append(a)
                else:
                    self.announce_list.append(a)
            self.announce_list = set(self.announce_list)
        except:
            self.announce_list.append(bdictionary['announce'])

        return True

    def get_http_trackers(self):
        '''
        Ziskanie HTTP trackerov zo zoznamu trackerov.
        '''

        for al in self.announce_list:
            if al.startswith("http:"):
                self.http_trackers.append(al)

# torrent.py
