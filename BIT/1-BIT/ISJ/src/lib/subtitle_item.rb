#encoding:utf-8

# subtitle_item.rb
# Autor:       Daniel Klimaj; xklima22@stud.fit.vutbr.cz
# Popis:       Trieda reprezentujuca titulok (1 blok).

class SubtitleItem

  TIME_REGEXP = /(\d+):(\d+):(\d+),(\d+)/

  HR_TO_MS    = 3600000
  MIN_TO_MS   = 60000
  SEC_TO_MS   = 1000

  def initialize index, stime, etime, data
    @index      = index
    @start_time = to_ms(stime)
    @end_time   = to_ms(etime)
    @data       = data
    @words      = @data.split.size
  end

  # Getter pre index
  def index
    @index
  end

  # Getter pre start_time - cas kedy sa titulka ukaze v ms
  def stime
    @start_time
  end

  # Getter pre end_time - cas kedy sa titulka skryje v ms
  def etime
    @end_time
  end

  # Getter pre data - text titulku
  def data
    @data
  end

  # Getter pre words - pocet slov titulku
  def words
    @words
  end

  # Vrati dobu, pocas ktorej je titulka zobrazena v ms
  def duration
    @end_time - @start_time
  end

  private

  # Prevedie cas formatu 'hh:mm:ss,ms' na ms
  def to_ms time
    tparts = TIME_REGEXP.match(time)
    ms = 0
    #tparts => 1: hrs, 2: mins, 3: secs, 4: msecs
    unless tparts.nil?
      ms += tparts[1].to_i * HR_TO_MS
      ms += tparts[2].to_i * MIN_TO_MS
      ms += tparts[3].to_i * SEC_TO_MS
      ms += tparts[4].to_i
    end

    ms
  end

end
