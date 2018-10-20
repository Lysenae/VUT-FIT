#! /usr/bin/env python3

#SYN:xklima22

# Soubor: syn.py
# Autor:  Daniel Klimaj, xklima22@stud.fit.vutbr.cz
# Popis:  IPP Project #2 - Zvyraznovani syntaxe

import getopt, sys, os, re

HELP = 'IPP 2015/2: SYN - Zvyraznovani syntaxe\n' + \
'Autor: Daniel Klimaj; xklima22@stud.fit.vutbr.cz\n\n' + \
'Usage: ./syn.py [--input=infile --output=outfile --format=fmtfile --help ' + \
'--nooverlap --br --escape]\n\n' + \
'Parametry:\n' + \
'\t--input=infile\t\tVstupni soubor\n' + \
'\t--output=outfile\tVystupni soubor\n' + \
'\t--format=fmtfile\tSoubor s formatovanim\n' + \
'\t--help\t\t\tVytiskne napovedu\n' + \
'\t--nooverlap\t\tZabrani prekryvani tagu\n' + \
'\t--br\t\t\tVlozi tag <br \> na konec radku\n' + \
'\t--escape\t\tEscapovani specialnich znaku\n'

# Trida simulujici enumerator stavu KA
class State(object):
    START           = 1
    ESCAPE          = 2
    SYM             = 3
    MODIFIED        = 4
    OR              = 5
    NEG             = 6
    NEG_ESCAPE      = 7
    NEG_SYM         = 8
    GRP_OPEN        = 9
    GRP_ESCAPE      = 10
    GRP_SYM         = 11
    GRP_OR          = 12
    GRP_MODIFIED    = 13
    GRP_NEG         = 14
    GRP_NEG_ESCAPE  = 15
    GRP_NEG_SYM     = 16
    GRP_CLOSE       = 17

def parse_params():
    '''
    Spracovani parametru prikazoveho radku.
    @returns Vraci Dict (slovnik) so ziskanymi parametry.
    '''

    params    = {}
    error     = 0
    shortopts = ''
    longopts  = ['help', 'format=', 'input=', 'output=', 'br', 'nooverlap', \
    'escape']

    try:
        opts, args = getopt.getopt(sys.argv[1:], shortopts, longopts)
    except getopt.GetoptError as err:
        sys.stderr.write("Invalid argument \'--{}\'\n".format(err.opt))
        error = True

    if not error:
        for opt, optarg in opts:
            if opt == '--help':
                params['help'] = True
            elif opt == '--format':
                path = os.path.abspath(optarg)
                if not os.path.exists(path):
                    sys.stderr.write("")
                else:
                    params['format'] = path
            elif opt == '--input':
                path = os.path.abspath(optarg)
                if not os.path.exists(path):
                    sys.stderr.write('File \'{}\' does not exist\n'.format(\
                        optarg))
                    error = 2
                else:
                    params['input'] = path
            elif opt == '--output':
                params['output'] = os.path.abspath(optarg)
            elif opt == '--br':
                params['br'] = True
            elif opt == '--nooverlap':
                params['nooverlap'] = True
            elif opt == '--escape':
                params['escape'] = True

    if not error:
        if 'help' in params and len(params) > 1:
            sys.stderr.write('Invalid combination with argument --help\n')
            error = 1

    return error, params

def is_special(c):
    '''
    Overi, zda je znak c jeden ze specialnich znaku v regulernich vyrazu.
    Specialni znaky jsou {, }, [, ], $, ^, ?, <, >, -, &, \.
    @returns Vraci True jestli znak c je specialni znak.
    '''

    if c in ["{", "}", "[", "]", "$", "^", "?", "<", ">", "-", "&", "\\"]:
        return True

    return False

def is_sym(c):
    '''
    Overi, zda je znak c je platny symbol regulerniho vyrazu.
    @returns Vraci True jestli znak c je platny symbol regexu.
    '''

    if ord(c) >= 32 and c not in [".", "|", "!", "*", "+", "%", "(", ")"]:
        return True

    return False

def is_escaping(c):
    '''
    Overi, zda je znak c znak, ktery muze byt escapovan znakem %.
    @returns Vraci True jestli znak c muze byt escapovan.
    '''

    if c in ["s", "a", "d", "l", "L", "w", "W", "t", "n", ".", "|", "!", \
    "*", "+", "%", "(", ")"]:
        return True

    return False

def process_regexp(rgx):
    '''
    Spracuje regulerni vyraz s pomoci KA a provede ho na regulerni vyraz
    podporovan Pythonem.
    @param rgx Vstupni regulerni vyraz
    @returns Vraci dvojici [valid, regexp], kde valid urcuje zda vstupni reg.
    vyraz bylo mozne spracovat s KA, regexp je vystupni regulerni vyraz nebo
    prazdni retezec, jestli doslo k chybe.
    '''

    i             = 0
    state         = State.START
    converted     = ""
    neg_str       = ""
    operator      = ""
    open_brackets = 0
    valid         = False

    OPEN_BR       = "("
    CLOSE_BR      = ")"
    PERCENT       = "%"
    NEG           = "!"
    OR            = "|"
    DOT           = "."
    MODIFIERS     = ["+", "*"]
    RE_COMPATIBLE = ["s", "d", "t", "n", ".", "|", "*", "+", "(", ")"]
    RE_LOWER      = "[a-z]"
    RE_UPPER      = "[A-Z]"
    RE_ALPHA      = "[a-zA-Z]"
    RE_ALNUM      = "[a-zA-Z0-9]"
    RE_NLOWER     = "[^a-z]"
    RE_NUPPER     = "[^A-Z]"
    RE_NALPHA     = "[^a-zA-Z]"
    RE_NALNUM     = "[^a-zA-Z0-9]"
    EMPTY         = ""

    while True:
        if i >= len(rgx):
            # FAILURE:
            if open_brackets != 0:
                break
            if not state in [State.START, State.SYM, State.MODIFIED, \
            State.NEG_SYM, State.GRP_CLOSE]:
                break

            # append operator (empty string if operator has already been appended)
            if state == State.MODIFIED:
                converted += operator

            # SUCCESS
            valid = True
            break

        c = rgx[i]

        if state == State.START:
            if c == OPEN_BR:
                state = State.GRP_OPEN
                converted += OPEN_BR
                open_brackets += 1
            elif c == PERCENT:
                state = State.ESCAPE
            elif is_sym(c) or is_special(c):
                state = State.SYM
                if is_special(c):
                    converted += ("\{}".format(c))
                else:
                    converted += c
            elif c == NEG:
                state = State.NEG
            else:
                break

        elif state == State.ESCAPE:
            if is_escaping(c):
                if c == "a":
                    converted += "."
                elif c in RE_COMPATIBLE:
                    converted += "\{}".format(c)
                elif c == "l":
                    converted += (RE_LOWER)
                elif c == "L":
                    converted += (RE_UPPER)
                elif c == "w":
                    converted += (RE_ALPHA)
                elif c == "W":
                    converted += (RE_ALNUM)
                elif c == NEG:
                    converted += "!"
                elif c == PERCENT:
                    converted += PERCENT

                state = State.SYM
            else:
                break

        elif state == State.SYM:
            if c in MODIFIERS:
                state = State.MODIFIED
                operator = c
            elif is_sym(c) or is_special(c):
                state = State.SYM
                if is_special(c):
                    converted += ("\{}".format(c))
                else:
                    converted += c
            elif c == NEG:
                state = State.NEG
            elif c == PERCENT:
                state = State.ESCAPE
            elif c == OPEN_BR:
                state = State.GRP_OPEN
                converted += OPEN_BR
                open_brackets += 1
            elif c == OR:
                state = State.OR
                converted += OR
            elif c == DOT:
                state = State.OR
            else:
                break

        elif state == State.MODIFIED:
            if c in MODIFIERS:
                state = State.MODIFIED
                if c != operator and operator == "+":
                    operator = c
            else:
                converted += operator
                operator = ""
                if is_sym(c) or is_special(c):
                    state = State.SYM
                    if is_special(c):
                        converted += ("\{}".format(c))
                    else:
                        converted += c
                elif c == NEG:
                    state = State.NEG
                elif c == PERCENT:
                    state = State.ESCAPE
                elif c == OPEN_BR:
                    state = State.GRP_OPEN
                    converted += OPEN_BR
                    open_brackets += 1
                elif c == OR:
                    state = State.OR
                    converted += OR
                elif c == DOT:
                    state = State.OR
                else:
                    break

        elif state == State.OR:
            if is_sym(c) or is_special(c):
                state = State.SYM
                if is_special(c):
                    converted += ("\{}".format(c))
                else:
                    converted += c
            elif c == NEG:
                state = State.NEG
            elif c == PERCENT:
                state = State.ESCAPE
            elif c == OPEN_BR:
                state = State.GRP_OPEN
                converted += OPEN_BR
                open_brackets += 1
            else:
                break

        elif state == State.NEG:
            if is_sym(c) or is_special(c):
                state = State.NEG_SYM
                if is_special(c):
                    converted += ("[^\{}]".format(c))
                else:
                    converted += ("[^{}]".format(c))
            elif c == PERCENT:
                state = State.NEG_ESCAPE
            elif c == NEG:
                state = State.NEG
            else:
                break

        elif state == State.NEG_SYM:
            if is_sym(c) or is_special(c):
                state = State.SYM
                if is_special(c):
                    converted += ("\{}".format(c))
                else:
                    converted += c
            elif c == NEG:
                state = State.NEG
            elif c == PERCENT:
                state = State.ESCAPE
            elif c == OPEN_BR:
                state = State.GRP_OPEN
                converted += OPEN_BR
                open_brackets += 1
            elif c == OR:
                state = State.OR
                converted += OR
            elif c in MODIFIERS:
                state = State.MODIFIED
                operator = c
            elif c == DOT:
                state = State.OR
            else:
                break

        elif state == State.NEG_ESCAPE:
            if is_escaping(c):
                if c == "a":
                    converted += ("[^.]")
                elif c in RE_COMPATIBLE:
                    converted += ("[^\{}]".format(c))
                elif c == "l":
                    converted += (RE_NLOWER)
                elif c == "L":
                    converted += (RE_NUPPER)
                elif c == "w":
                    converted += (RE_NALPHA)
                elif c == "W":
                    converted += (RE_NALNUM)
                elif c == NEG:
                    converted += "[^!]"
                elif c == PERCENT:
                    converted += "[^%]"

                state = State.NEG_SYM
            else:
                break

        elif state == State.GRP_OPEN:
            if c == OPEN_BR:
                state = State.GRP_OPEN
                converted += OPEN_BR
                open_brackets += 1
            elif c == CLOSE_BR:
                state = State.GRP_CLOSE
                open_brackets -= 1
                converted += CLOSE_BR
            elif is_sym(c) or is_special(c):
                state = State.GRP_SYM
                if(is_special(c)):
                    converted += ("\{}".format(c))
                else:
                    converted += c
            elif c == NEG:
                state = State.GRP_NEG
            elif c == PERCENT:
                state = State.GRP_ESCAPE
            else:
                break

        elif state == State.GRP_CLOSE:
            if open_brackets == 0:
                if c == OPEN_BR:
                    state = State.GRP_OPEN
                    converted += OPEN_BR
                    open_brackets += 1
                elif c == PERCENT:
                    state = State.ESCAPE
                elif is_sym(c) or is_special(c):
                    state = State.SYM
                    if is_special(c):
                        converted += ("\{}".format(c))
                    else:
                        converted += c
                elif c == NEG:
                    state = State.NEG
                elif c == OR:
                    state = State.OR
                    converted += OR
                elif c in MODIFIERS:
                    state = State.MODIFIED
                    operator = c
                elif c == DOT:
                    state = State.OR
                else:
                    break
            elif open_brackets > 0:
                if c == OPEN_BR:
                    state = State.GRP_OPEN
                    converted += OPEN_BR
                    open_brackets += 1
                elif c == CLOSE_BR:
                    state = State.GRP_CLOSE
                    open_brackets -= 1
                    converted += CLOSE_BR
                elif is_sym(c) or is_special(c):
                    state = State.GRP_SYM
                    if(is_special(c)):
                        converted += ("\{}".format(c))
                    else:
                        converted += c
                elif c == NEG:
                    state = State.GRP_NEG
                elif c == PERCENT:
                    state = State.GRP_ESCAPE
                elif c == OR:
                    state = State.GRP_OR
                    converted += OR
                elif c in MODIFIERS:
                    state = State.GRP_MODIFIED
                    operator = c
                elif c == DOT:
                    state = State.GRP_OR
                else:
                    break

            else:
                break

        elif state == State.GRP_SYM:
            if c in MODIFIERS:
                state = State.GRP_MODIFIED
                operator = c
            elif is_sym(c) or is_special(c):
                state = State.GRP_SYM
                if is_special(c):
                    converted += ("\{}".format(c))
                else:
                    converted += c
            elif c == NEG:
                state = State.GRP_NEG
            elif c == PERCENT:
                state = State.GRP_ESCAPE
            elif c == OPEN_BR:
                state = State.GRP_OPEN
                converted += OPEN_BR
                open_brackets += 1
            elif c == CLOSE_BR:
                state = State.GRP_CLOSE
                converted += CLOSE_BR
                open_brackets -= 1
            elif c == OR:
                state = State.GRP_OR
                converted += OR
            elif c == DOT:
                state = State.GRP_OR
            else:
                break

        elif state == State.GRP_ESCAPE:
            if is_escaping(c):
                if c == "a":
                    converted += (".")
                elif c in RE_COMPATIBLE:
                    converted += ("\{}".format(c))
                elif c == "l":
                    converted += (RE_NLOWER)
                elif c == "L":
                    converted += (RE_NUPPER)
                elif c == "w":
                    converted += (RE_NALPHA)
                elif c == "W":
                    converted += (RE_NALNUM)
                elif c == NEG:
                    converted += "!"
                elif c == PERCENT:
                    converted += PERCENT

                state = State.GRP_SYM
            else:
                break

        elif state == State.GRP_OR:
            if is_sym(c) or is_special(c):
                state = State.GRP_SYM
                if is_special(c):
                    converted += ("\{}".format(c))
                else:
                    converted += c
            elif c == NEG:
                state = State.GRP_NEG
            elif c == PERCENT:
                state = State.GRP_ESCAPE
            elif c == OPEN_BR:
                state = State.GRP_OPEN
                converted += OPEN_BR
                open_brackets += 1
            else:
                break


        elif state == State.GRP_MODIFIED:
            if c in MODIFIERS:
                state = State.GRP_MODIFIED
                if c != operator and operator == "+":
                    operator = c
            else:
                converted += operator
                operator = ""
                if is_sym(c) or is_special(c):
                    state = State.GRP_SYM
                    if is_special(c):
                        converted += ("\{}".format(c))
                    else:
                        converted += c
                elif c == NEG:
                    state = State.GRP_NEG
                elif c == PERCENT:
                    state = State.GRP_ESCAPE
                elif c == OPEN_BR:
                    state = State.GRP_OPEN
                    converted += OPEN_BR
                    open_brackets += 1
                elif c == CLOSE_BR:
                    state = State.GRP_CLOSE
                    converted += CLOSE_BR
                    open_brackets -= 1
                elif c == OR:
                    state = State.GRP_OR
                    converted += OR
                elif c == DOT:
                    state = State.GRP_OR
                else:
                    break

        elif state == State.GRP_NEG:
            if is_sym(c) or is_special(c):
                state = State.GRP_NEG_SYM
                if is_special(c):
                    converted += ("[^\{}]".format(c))
                else:
                    converted += ("[^{}]".format(c))
            elif c == PERCENT:
                state = State.GRP_NEG_ESCAPE
            elif c == NEG:
                state = State.GRP_NEG
            else:
                break

        elif state == State.GRP_NEG_SYM:
            if is_sym(c) or is_special(c):
                state = State.GRP_SYM
                if is_special(c):
                    converted += ("\{}".format(c))
                else:
                    converted += c
            elif c == NEG:
                state = State.GRP_NEG
            elif c == PERCENT:
                state = State.GRP_ESCAPE
            elif c == OPEN_BR:
                state = State.GRP_OPEN
                converted += OPEN_BR
                open_brackets += 1
            elif c == CLOSE_BR:
                    state = State.GRP_CLOSE
                    converted += CLOSE_BR
                    open_brackets -= 1
            elif c == OR:
                state = State.GRP_OR
                converted += OR
            elif c in MODIFIERS:
                state = State.GRP_MODIFIED
                operator = c
            elif c == DOT:
                state = State.GRP_OR
            else:
                break

        elif state == State.GRP_NEG_ESCAPE:
            if is_escaping(c):
                if c == "a":
                    converted += "[^.]"
                elif c in RE_COMPATIBLE:
                    converted += "[^\{}]".format(c)
                elif c == "l":
                    converted += (RE_NLOWER)
                elif c == "L":
                    converted += (RE_NUPPER)
                elif c == "w":
                    converted += (RE_NALPHA)
                elif c == "W":
                    converted += (RE_NALNUM)
                elif c == NEG:
                    converted += "[^!]"
                elif c == PERCENT:
                    converted += "[^%]"

                state = State.GRP_NEG_SYM
            else:
                break
        i += 1

    return valid, (converted if valid else EMPTY)

def process_format(fmt):
    '''
    Provede format fmt na otvaraci a zaviraci HTML tagy.
    @param fmt Vstupni format
    @returns Vraci trojici [valid, o, c], kde valid urcuje zda bylo mozne
    vstupni format provest na HTML tagy, o a c jsou otviraci (open), resp.
    zaviraci (closing) HTML tagy.
    '''

    fp, o, c, valid = fmt.split(","), "", "", True
    for f in fp:
        f = f.strip()
        if f == "bold":
            o += "<b>"
            c = "</b>" + c
        elif f == "italic":
            o += "<i>"
            c = "</i>" + c
        elif f == "underline":
            o += "<u>"
            c = "</u>" + c
        elif f == "teletype":
            o += "<tt>"
            c = "</tt>" + c
        elif re.match(r"size:\d+", f):
            if int(f.split(":")[1]) in range(1,8):
                o += "<font {}>".format(f.replace(":", "="))
                c = "</font>" + c
            else:
                valid = False
                break
        elif re.match(r"color:[a-fA-F0-9]{6}", f):
            o += "<font {}>".format(f.replace(":", "=#"))
            c = "</font>" + c
        else:
            valid = False
            break

    return valid, o, c

def print_output(text, opts, formatted=False):
    '''
    Vytiskne text na standardni vystup nebo do zadaneho souboru.
    Text muze byt modifikovan paramtry
    prikazoveho radky opts.
    @param text Vstupni text
    @param opts Parametry prikazoveho radku
    @param formatted Priznak, zda text byl formatovan, implicitne False
    @returns Vraci stavove kody 0 pri uspechu nebo 3 jestli se nepodari
    otevrit vystupni soubor.
    '''

    if not formatted:
        if 'escape' in opts:
            text = text.replace("&", "&amp;")
            text = text.replace("<", "&lt;")
            text = text.replace(">", "&gt;")

    if 'br' in opts:
        text = text.replace("\n", "<br />\n")

    if 'output' in opts:
        f = None
        try:
            f = open(opts['output'], 'w')
        except:
            sys.stderr.write("Failed to open file \'{}\'\n".format(\
                opts['output']))
            return 3
        f.write(text)
        f.close()
    else:
        sys.stdout.write(text)

    return 0

def exit(errcode, params, text="", formatted=False):
    '''
    Vytisknuti textu a ukonceni skriptu stavovym kodem errcode.
    @param errcode Stavovy kod
    @param params Parametry prikazoveho radky
    @param text Vstupni text, implicitne prazdny retezec
    @param formatted Priznak, zda text byl formatovan
    '''

    status = print_output(text, params, formatted)
    if status != 0:
        errcode = status
    sys.exit(errcode)

# Trida reprezentujici formatovani textu
class Formating(object):
    def __init__(self, string):
        '''
        Konstruktor. Nastavi hodnoty podle vstupniho retezce string.
        @param string Vstupni formatovaci retezec
        '''

        self.in_use     = 0
        self.idx        = 0
        self.start_idxs = []
        self.end_idxs   = []
        self.os_idx     = []
        self.oe_idx     = []

        m = re.search(r"(.*?)\t+(.*)$", string)
        if m != None and len(m.groups()) == 2:
            r_valid, self.regexp           = process_regexp(m.group(1).strip())
            f_valid, self.open, self.close = process_format(m.group(2).strip())
            self.string                    = string
            self.valid                     = r_valid and f_valid

            if not r_valid:
                sys.stderr.write("Invalid regexp \'{}\'\n".format(\
                    m.group(1).strip()))
            if not f_valid:
                sys.stderr.write("Invalid formatting option \'{}\'\n".format(\
                    m.group(2).strip()))
        else:
            self.valid  = False
            self.open   = ""
            self.close  = ""
            self.string = ""
            self.regexp = ""

    def __eq__(self, other):
        '''
        Porovnavani dvou instancii tridy Formatting. Podpora operatoru ==, !=.
        @param other Jina instance tridy Formatting
        @returns Vraci True jestli se instance zhoduji ve vsech polich valid,
        open, close, string a regexp.
        '''

        if self.valid == other.valid and self.open == other.open and \
        self.close == other.close and self.string == other.string and \
        self.regexp == other.regexp:
            return True

        return False

    def __ne__(self, other):
        '''
        Porovnavani dvou instancii tridy Formatting. Podpora operatoru ==, !=.
        @param other Jina instance tridy Formatting
        @returns Vraci True jestli se instance nezhoduji v polich valid,
        open, close, string nebo regexp.
        '''
        return not self.__eq__(other)

    def __lt__(self, other):
        '''
        Pro podporu vestavene funkce sorted().
        @param other Jina instance tridy Formatting
        @returns Vraci True jestli volajici instance ma nizsi hodnotu
        start_idxs na indexu idx.
        '''

        return self.start_idxs[self.idx] < other.start_idxs[other.idx]

if __name__ == '__main__':
    err, params = parse_params()

    if err > 0:
        exit(err, params)

    if 'help' in params:
        exit(0, params, HELP, True)

    intext = None
    if 'input' in params:
        f = None
        try:
            f = open(params['input'])
        except:
            exit(2, params)

        intext = f.read()
        f.close()
    else:
        intext = sys.stdin.read()

    formats = []
    if 'format' not in params:
        exit(0, params, intext)
    else:
        f = None
        try:
            f = open(params['format'])
        except: # Selhani pri otevirani formatovaciho souboru
            exit(0, params, intext)
        else:
            for line in f.readlines():
                fmt = Formating(line.strip())
                if fmt.valid:
                    formats.append(fmt)
                else:
                    f.close()
                    exit(4, params)
            f.close()

    outtext   = ""
    formatted = True

    # Formatovani textu
    tlen = len(intext)
    flen = len(formats)
    if formats:
        for i in range(flen):
            rgx = re.compile(formats[i].regexp, re.DOTALL)
            for match in rgx.finditer(intext):
                formats[i].start_idxs.append(match.start())
                formats[i].end_idxs.append(match.end())

        if 'nooverlap' in params:
            s, e = [], []
            for t in range(flen):
                for ti in range(len(formats[t].start_idxs)):
                    for o in range(flen):
                        if t != o:
                            for to in range(len(formats[o].start_idxs)):
                                #if formats[t].start_idxs[ti] > formats[o].start_idxs[to] and \
                                if formats[t].start_idxs[ti] < formats[o].end_idxs[to] and \
                                formats[t].end_idxs[ti] > formats[o].end_idxs[to]:
                                    s.append(formats[o].end_idxs[to])
                                    e.append(formats[o].end_idxs[to])

                formats[t].os_idx = s[:]
                formats[t].oe_idx = e[:]
                s, e = [], []

            for i in range(flen):
                formats[i].start_idxs = sorted(set(formats[i].os_idx + formats[i].start_idxs))
                formats[i].end_idxs = sorted(set(formats[i].oe_idx + formats[i].end_idxs))

        else:
            for i in range(flen):
                if len(formats[i].start_idxs) != len(formats[i].end_idxs):
                    exit(4, params)
                else:
                    new_o, new_c = [], []
                    for j in range(len(formats[i].start_idxs)):
                        if formats[i].start_idxs[j] != formats[i].end_idxs[j]:
                            new_o.append(formats[i].start_idxs[j])
                            new_c.append(formats[i].end_idxs[j])

                    formats[i].start_idxs = new_o[:]
                    formats[i].end_idxs   = new_c[:]

        for i in range(tlen+1):
            if 'nooverlap' in params:
                ends = []
                for j in range(flen):
                    if i in formats[j].oe_idx:
                        for e in range(len(formats[j].end_idxs)):
                            f = formats[j]
                            if formats[j].end_idxs[e] == i:
                                f.idx = e
                                ends.append(f)

                if len(ends) > 0:
                    ends = sorted(ends)
                    ends.reverse()
                    for e in range(len(ends)):
                        for f in range(flen):
                            if ends[e] == formats[f]:
                                outtext += ends[e].close
                                formats[f].in_use -= 1

            for j in range(flen):
                n = flen-j-1
                if formats[n].in_use > 0:
                    if i in formats[n].end_idxs:
                        outtext += formats[n].close
                        formats[n].in_use -= 1

            for j in range(flen):
                if i in formats[j].start_idxs:
                    outtext += formats[j].open
                    formats[j].in_use += 1

            if(i < tlen):
                if 'escape' in params:
                    if intext[i] == "<":
                        outtext += "&lt;"
                    elif intext[i] == ">":
                        outtext += "&gt;"
                    elif intext[i] == "&":
                        outtext += "&amp;"
                    else:
                        outtext += intext[i]
                else:
                    outtext += intext[i]
    else:
        outtext = intext
        formatted = False

    exit(0, params, outtext, formatted)

# syn.py
