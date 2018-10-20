#!/usr/bin/env python

# Autor: Daniel Klimaj; xklima22@stud.fit.vutbr.cz
# Popis: Hlavny subor

import os
import sys

import requests
import requests.packages.urllib3 as urllib3

from log import Log as Log
from torrent import Torrent as Torrent
from rss_document import RssDocument as RssDocument
from tracker import Tracker as Tracker

# Enumeracia stavov
class Mode(object):
    Default       = 0
    RssRemote     = 1
    RssLocal      = 2
    TorrentLocal  = 3
    Help          = 4

def print_help():
    '''
    Vypis napovedy.
    '''

    s = "{}  (-r url|-i filename|-t filename) [-a url]\n\n".format(sys.argv[0])
    s += "  -r [--rss] URL                         Ziskanie dat z RSS feedu z URL\n"
    s += "  -i [--input-announcement] filename     Ziskanie dat z RSS feedu zo suboru filename\n"
    s += "  -a [--tracker-annonce-url] URL         Podvrzna tracker announce URL\n"
    s += "  -t [--torrent-file] filename           Spracovanie uz stiahnuteho torrentu v subore filename\n"
    s += "  -h [--help]                            Tato sprava"

    print(s)

def parse_args():
    '''
    Spracovanie parametrov prikazoveho riadku.
    @return stvoricu Status, Mod, Parameter modu, Fake Announce URL
    '''
    args = sys.argv
    mode = Mode.Default

    if len(args) != 3 and len(args) != 5:
        if len(args) == 2 and (args[1] == "-h" or args[1] == "--help"):
            mode = Mode.Help
            return True, mode, None, None
        else:
            Log.error("AntiPirat: Neplatne parametre programu")
            print_help()
            return False, mode, None, None
    else:
        fake_announce = None
        if args[1] == "-r" or args[1] == "--rss":
            mode = Mode.RssRemote
        elif args[1] == "-i" or args[1] == "--input-announcement":
            mode = Mode.RssLocal
        elif args[1] == "-t" or args[1] == "--torrent-file":
            mode = Mode.TorrentLocal
        else:
            Log.error("AntiPirat: Neplatny 1. parameter {}".format(args[1]))
            print_help()
            return False, mode, None, None

        if len(args) == 5:
            if args[3] == "-a" or args[3] == "--tracker-annonce-url":
                fake_announce = args[4]
            else:
                Log.error("AntiPirat: Neplatny 3. parameter {}".format(args[3]))
                print_help()
                return False, mode, None, None

        return True, mode, args[2], fake_announce

def abspath(path):
    '''
    Alias pre os.path.abspath
    @param path Cesta k suboru
    '''

    return os.path.abspath(path)

def write_announce_list(rss_nodes, filename):
    '''
    Zapis informacii z RSS do citatelnej podoby.
    @param rss_nodes Zoznam RssNode
    @param filename Vystupny subor
    @return True ak je spracovanie uspesne, inak False
    '''

    f = None
    try:
        f = open(filename, "w")
    except:
        Log.error("AntiPirat: Nepodarilo sa otvorit subor {} v zapisovom mode".format(filename))
        return False

    s =  "title: movies torrents RSS feed - KickassTorrents\n"
    s += "link: http://kat.cr/\n"
    s += "description: movies torrents RSS feed\n\n\n"

    for node in rss_nodes:
        ns = node.readable_form()
        if ns != "":
            s += "{}\n".format(ns)

    f.write("{}\n".format(s.strip()))
    f.close()

    return True

def download_rss(url, output_file):
    '''
    Stiahne RSS subor z URL url.
    @param url URL k RSS
    @param output_file Vystupny subor
    @return True ak je spracovanie uspesne, inak False
    '''

    response = None

    try:
        response = requests.get(url, verify=False)
    except requests.exceptions.RequestException:
        Log.error("AntiPirat: Neplatna URL: {}".format(url))
        return False

    # Odpoved je ina ako 200 OK
    if response.status_code != 200:
        Log.error("AntiPirat: Neuspesna poziadavka na ziskanie RSS, vrateny stavovy kod {}".format(response.status_code))
        return False

    try:
        f = open(output_file, "w")
        f.write(response.text.encode('utf-8'))
        f.close()
    except:
        Log.error("AntiPirat: Doslo ku chybe pri zapisovani RSS feedu do suboru {}".format(output_file))
        return False

    return True

def parse_rss(filename):
    '''
    Ziskanie dat z RSS suboru.
    @param filename RSS subor
    @return dvojicu True, Prvy uzol s torrentom ak je spracovanie uspesne, inak False, None
    '''

    torrent_node = None
    rd = RssDocument(filename)
    if not rd.is_kat_rss():
        Log.error("AntiPirat: RSS nepochadza zo serveru http://kat.cr/")
        return False, torrent_node

    rss_first = rd.get_first()
    rss_all   = rd.get_all()

    if rss_first != None:
        if not rss_first.init:
            Log.error("AntiPirat: Prvemu zaznamu v RSS chybaju niektore dolezite elementy")
            return False, torrent_node
        else:
            torrent_node = rss_first
    else:
        Log.error("AntiPirat: V RSS {} sa nenasli ziadne zaznamy".format(filename))
        return False, torrent_node

    rslt = False
    if len(rss_all) > 0:
        rslt = write_announce_list(rss_all, abspath("movies_announce_list.txt"))
    else:
        # Nemalo by nastat
        rslt = write_announce_list([rss_first], abspath("movies_announce_list.txt"))

    return rslt, torrent_node

def download_torrent(url, filename):
    '''
    Stiahnutie torrentu z URL url.
    @param url URL torrentu
    @param filename Subor pre zapisanie torrentu
    @return True ak je spracovanie uspesne, inak False
    '''

    Log.info("AntiPirat: Stahujem torrent z URL {}".format(url))
    request = urllib3.PoolManager()
    response = request.urlopen('GET', url)

    try:
        f = open(filename, 'w')
        f.write(response.data)
        f.close()
        Log.info("AntiPirat: Stiahnuty torrent bol ulozeny do suboru {}".format(filename))
        return True
    except:
        Log.error("AntiPirat: Chyba pri zapisovani torrent suboru {}".format(filename))
        return False

def write_peerlist(tfile, filename, fake_announce):
    '''
    Zapis peerlistu.
    @param tfile Cesta k torrentu
    @param filename Cesta k suboru pre zapis peerlistu
    @param fake_announce Falosna announce URL
    @return True ak je spracovanie uspesne, inak False
    '''

    torrent = Torrent(tfile)
    if not torrent.valid:
        Log.error("AntiPirat: Nevalidny torrent subor {}".format(tfile))
        return False

    if fake_announce != None:
        Log.info("AntiPirat: Kontaktujem tracker cez fake announce url (parameter -a): {}".format(fake_announce))
        tracker = Tracker(fake_announce, torrent)
        if tracker.connect():
            peers = tracker.get_peerlist()
            if peers is not None:
                try:
                    Log.info("AntiPirat: Otvaram subor k zapisu peerlistu {}".format(filename))
                    f = open(filename, "w")
                    for p in peers:
                        f.write("{}\n".format(p))
                    Log.info("AntiPirat: Uspesne ukonceny zapis peerlistu z trackeru {}".format(fake_announce))
                    f.close()
                except:
                    Log.error("AntiPirat: Nepodarilo sa zapisat peerlist do suboru {}".format(filename))
                    return False
            else:
                Log.error("AntiPirat: Ziskavanie peerlistu zlyhalo")
                return False
        else:
            Log.error("AntiPirat: Spojenie s trackerom zlyhalo")
            return False
        Log.info("AntiPirat: Uspesne dokonceny zapis peerlistov do suboru {}".format(filename))
        return True
    else:
        trackers = torrent.http_trackers
        Log.info("AntiPirat: Pocet najdenych HTTP trackerov: {}".format(len(trackers)))

        try:
            Log.info("AntiPirat: Otvaram subor k zapisu peerlistu {}".format(filename))
            f = open(filename, "w")
        except:
            Log.error("AntiPirat: Nepodarilo sa otvorit subor {}".format(filename))
            return False

        for t in trackers:
            Log.info("AntiPirat: Kontaktujem tracker {}".format(t))
            tracker = Tracker(t, torrent)
            if tracker.connect():
                peers = tracker.get_peerlist()
                if peers is not None:
                    for p in peers:
                        f.write("{}\n".format(p))
                    Log.info("AntiPirat: Uspesne ukonceny zapis peerlistu z trackeru {}".format(t))
            else:
                Log.info("AntiPirat: Preskakujem tracker, u ktoreho doslo k chybe: {}".format(t))

        f.close()
        Log.info("AntiPirat: Uspesne dokonceny zapis peerlistov do suboru {}".format(filename))
        return True


def app_exit(b_code):
    '''
    Ukoncenie aplikacia stavovym kodom.
    @param b_code Kod v bool
    '''

    code = 0 if b_code else -1
    sys.exit(code)

def handle_rss_remote(rss_url, fake_announce_url):
    '''
    Spracovanie parametru -r (-a).
    @param rss_url URL k RSS
    @param fake_announce_url URL
    @return True ak je spracovanie uspesne, inak False
    '''

    path = abspath("movies_announce.xml")
    rslt = download_rss(rss_url, path)

    if rslt:
        rslt, torrent = parse_rss(path)

        if rslt and torrent:
            rslt = download_torrent(torrent.url, abspath(torrent.file_name))

            if rslt:
                rslt = write_peerlist(abspath(torrent.file_name), \
                    abspath("{}.peerlist".format(torrent.info_hash)), \
                    fake_announce_url)

    return rslt

def handle_rss_local(filename, fake_announce_url):
    '''
    Spracovanie parametru -i (-a).
    @param filename Cesta k RSS
    @param fake_announce_url URL
    @return True ak je spracovanie uspesne, inak False
    '''

    rslt, torrent = parse_rss(abspath(filename))

    if rslt and torrent:
        rslt = download_torrent(torrent.url, abspath(torrent.file_name))

        if rslt:
            rslt = write_peerlist(abspath(torrent.file_name), \
                abspath("{}.peerlist".format(torrent.info_hash)), \
                fake_announce_url)

    return rslt

def handle_torrent_local(tfilename, fake_announce_url):
    '''
    Spracovanie parametru -t (-a).
    @param tfilename Cesta k torrentu
    @param fake_announce_url URL
    @return True ak je spracovanie uspesne, inak False
    '''

    rslt    = False
    tpath   = abspath(tfilename)
    torrent = Torrent(abspath(tfilename))

    if torrent:
        rslt = write_peerlist(tpath, \
            abspath("{}.peerlist".format(torrent.info_hash)), \
            fake_announce_url)

    return rslt

if __name__ == '__main__':
    '''
    Main.
    '''

    # Vypnutie hlasenie chyb, vypina niektore vynimky
    urllib3.disable_warnings()

    status, mode, data, fake_announce = parse_args()

    if status == False:
        app_exit(status)

    if mode == Mode.Help:
        print_help()
        app_exit(status)

    elif mode == Mode.RssRemote:
        rslt = handle_rss_remote(data, fake_announce)
        app_exit(rslt)

    elif mode == Mode.RssLocal:
        rslt = handle_rss_local(data, fake_announce)
        app_exit(rslt)

    elif mode == Mode.TorrentLocal:
        rslt = handle_torrent_local(data, fake_announce)
        app_exit(rslt)

    else:
        Log.error("AntiPirat: Program sa dostal na nepredvidatelne miesto a ukonci sa")
        app_exit(False)

# antipirat.py
