# Autor: Daniel Klimaj; xklima22@stud.fit.vutbr.cz
# Popis: Trieda RssNode reprezentujuca uzol RSS suboru, umoznuje ziskat informacie
#        o torrente

from rss import Rss as Rss
from log import Log as Log

class RssNode(Rss):

    def __init__(self, node, xmlns):
        self.rss_node  = node
        self.xmlns     = xmlns
        self.length    = 0
        self.title     = ""
        self.info_hash = ""
        self.author    = ""
        self.category  = ""
        self.link      = ""
        self.guid      = ""
        self.date      = ""
        self.url       = ""
        self.file_name = ""
        self.init      = False

        if node is not None and xmlns is not None:
            self.get_values()

    def get_values(self):
        '''
        Ziskanie hodnot RSS elementu
        '''

        try:    self.length    = int(self.find_first_child_by_name(self.rss_node, "{{{}}}contentLength".format(self.xmlns)).text)
        except: self.length    = 0

        try:    self.title     = self.find_first_child_by_name(self.rss_node, "title").text.encode('utf-8')
        except: self.title     = ""

        try:    self.info_hash = self.find_first_child_by_name(self.rss_node, "{{{}}}infoHash".format(self.xmlns)).text
        except: self.info_hash = ""

        try:    self.author    = self.find_first_child_by_name(self.rss_node, "author").text.encode('utf-8')
        except: self.author    = ""

        try:    self.category  = self.find_first_child_by_name(self.rss_node, "category").text
        except: self.category  = ""

        try:    self.link      = self.find_first_child_by_name(self.rss_node, "link").text.encode('utf-8')
        except: self.link      = ""

        try:    self.guid      = self.find_first_child_by_name(self.rss_node, "guid").text.encode('utf-8')
        except: self.guid      = ""

        try:    self.date      = self.find_first_child_by_name(self.rss_node, "pubDate").text
        except: self.date      = ""

        try:    self.url       = self.find_first_child_by_name(self.rss_node, "enclosure").attrib['url']
        except: self.url       = ""

        try:    self.file_name = self.find_first_child_by_name(self.rss_node, "{{{}}}fileName".format(self.xmlns)).text
        except: self.file_name = ""

        self._check_validity()

    def readable_form(self):
        '''
        Prepis dat do citatelnej podoby.
        @return String
        '''

        s = ""
        if self.init:
            s += "title: {}\n".format(self.title)
            s += "category: {}\n".format(self.category)
            s += "author: {}\n".format(self.author)
            s += "link: {}\n".format(self.link)
            s += "pubDate: {}\n".format(self.date)
            s += "torrent:infoHash: {}\n".format(self.info_hash)
            s += "torrent:fileName: {}\n".format(self.file_name)

        return s

    def _check_validity(self):
        '''
        Overenie, ci boli ziskane vsetky nutne data.
        '''

        if self.title != "" and self.date != "" and self.url != "" and self.file_name != "" and self.info_hash != "":
            self.init = True

# rss_node.yp
