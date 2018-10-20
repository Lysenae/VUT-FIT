#encoding:utf-8

# subtitles.rb
# Autor:       Daniel Klimaj; xklima22@stud.fit.vutbr.cz
# Popis:       Trieda reprezentujuca subor s titulkami.

require './lib/subtitle_item.rb'

class Subtitles

  SUB_ITEM_REGEXP = /(\d+\n\d+:\d+:\d+,\d+.*?\d+:\d+\d+,\d+\n.*?\n\n)/m
  SI_INDEX_REGEXP = /(\d+)\n/
  SI_TIME_REGEXP  = /(\d+:\d+:\d+,\d+)\s/
  SI_DATA_REGEXP  = /\d+:\d+:\d+,\d+\n(.*$)/m

  ENCODING = 'UTF-8'

  def initialize fpath
    @path       = File.expand_path(fpath)
    @name       = File.basename(fpath)
    @fdata      = to_utf8(File.open(fpath).read).gsub(/\r/, '')
    @items      = init_items
    @words      = word_count
    @rows       = @fdata.scan(/\n/).count
    @last_index = 1
    @item_count = @items.length
  end

  # Getter pre name - nazov suboru s titulkami
  def name
    @name
  end

  # Getter pre path - cesta k suboru s titulkami
  def path
    @path
  end

  # Getter pre items - casti titulkov
  def items
    @items
  end

  # Vrati polozku titulkov na indexe index
  def item index
    @items[index-1]
  end

  # Getter pre words - pocet slov titulkov (len text, ktory sa zobrazuje)
  def words
    @words
  end

  # Getter pre rows - pocet riadkov v subore
  def rows
    @rows
  end

  # Gatter pre last_index -  index naposledy kontrolovaneho titulku
  def last_index
    @last_index
  end

  # Setter pre last_index
  def last_index= index
    @last_index = index
  end

  # Vrati pocet blokov s titulkami
  def item_count
    @item_count
  end

  # Porovnanie titulkov
  def compare other, f_log
    stime_matches         = 0
    duration_matches      = 0
    item_words_matches    = 0

    f = File.open(f_log, 'w+')

    @items.each do |i|
      ok = true
      f.puts(i.index)
      f.puts("CS: #{i.data}")

      while ok
        other_item = other.item(other.last_index)
        return 0 if other_item.nil?
        if i.stime >= other_item.stime && i.stime <= other_item.stime + 1000
          stime_matches += 1
          duration_matches += 1 if i.duration == other_item.duration
          item_words_matches += 1 if i.words == other_item.words

          f.puts("EN: #{other.item(other.last_index).data}")
          f.puts
          ok = false
        end

        if other.last_index < other.item_count
          other.last_index += 1
        else
          f.puts("EN: \t") if ok
          f.puts
          break
        end

        if (other.item(other.last_index).stime + 1000) - i.stime > 1000
          f.puts("EN: \t") if ok
          f.puts
          break
        end
      end
    end

    rslt = stime_matches.to_f/@item_count + duration_matches.to_f/@item_count +
      item_words_matches.to_f/@item_count
    rslt /= 3
    rslt
  end

  private

  # Vynutene prevedie kodovanie na UTF-8 a odstrani chybne znaky
  # Riesi problem s invalid sequence of bytes error
  def to_utf8(str)
    str = str.force_encoding(ENCODING)
    return str if str.valid_encoding?
    str.encode(ENCODING, 'binary', invalid: :replace, undef: :replace,
      replace: '')
  end

  # Rozdeli subor s titulkami na jednotlive casti
  # Vracia rozdeleny text zo suboru, nespracovany
  def get_items
    @fdata.scan(SUB_ITEM_REGEXP)
  end

  # Inicializuje triedy reprezentujuce casti titulkov
  # Vracia pole SubtitleItems
  def init_items
    items = []
    get_items.each do |item|
      item = item[0]
      index = SI_INDEX_REGEXP.match(item)[1].strip.to_i
      times = item.scan(SI_TIME_REGEXP)
      data  = SI_DATA_REGEXP.match(item)[1].gsub(/\n/, ' ').strip
      items.push SubtitleItem.new(index, times[0].to_s.strip,
        times[1].to_s.strip, data)
    end

    items
  end

  # Vypocita pocet slov
  def word_count
    cnt = 0
    @items.each { |i| cnt += i.words }
    cnt
  end

end
