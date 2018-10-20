# Autor: Daniel Klimaj; xklima22@stud.fit.vutbr.cz
# Popis: Trieda RssDocument pre ziskavanie dat z RSS suborov

import xml.etree.ElementTree as XML
import re

from rss import Rss as Rss
from rss_node import RssNode as RssNode

class RssDocument(Rss):

    def __init__(self, path):
        self.path  = path
        self.doc   = XML.parse(self.path)
        self.root  = self.doc.getroot()
        self.xmlns = self.get_xmlns()

    def is_kat_rss(self):
        '''
        Overenie, ci RSS pochadza z http://kat.cr/.
        @return True ak je RSS z http://kat.cr/
        '''

        channel = self.find_first_child_by_name(self.root, "channel")

        if channel is not None:
            link = self.find_first_child_by_name(channel, "link")
            try:
                if link.text.strip() == "http://kat.cr/":
                    return True
            except:
                pass

        return False

    def get_xmlns(self):
        '''
        Ziskanie xmlns.
        @return xmlns alebo None ak xmlns nie je definovane
        '''

        f = open(self.path).read()
        xmlns = re.search(r'<rss.*?xmlns:torrent=\"(.*?)\">', f)
        return xmlns.group(1) if xmlns else None

    def get_first(self):
        '''
        Ziskanie prveho podelementu z elementu channel.
        @return RssNode alebo None
        '''

        rslt    = None
        channel = self.find_first_child_by_name(self.root, "channel")

        if channel is not None:
            item = self.find_first_child_by_name(channel, "item")
            rslt = RssNode(item, self.xmlns)

        return rslt

    def get_all(self):
        '''
        Ziskanie vsetkych podelementov z elementu channel.
        @return Zoznam RssNode alebo prazdny zoznam.
        '''

        nodes = []

        channel = self.find_first_child_by_name(self.root, "channel")

        if channel is not None:
            items = channel.findall("item")
            for item in items:
                nodes.append(RssNode(item, self.xmlns))

        return nodes
