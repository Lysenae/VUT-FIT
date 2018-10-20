# Autor: Daniel Klimaj; xklima22@stud.fit.vutbr.cz
# Popis: Trieda Rss, spolocna nadtrieda pre RssNode a RssDocument

import xml.etree.ElementTree as XML

class Rss(object):

    def find_first_child_by_name(self, node, name):
        '''
        Vyhladanie prveho podelementu s menom name v elemente node.
        @param node Element, v ktorom sa vyhladava
        @param name Nazov elementu, ktory sa hlada
        @return XML element alebo None
        '''

        nodes = node.findall(name)
        return nodes[0] if len(nodes) > 0 else None

# rss.py