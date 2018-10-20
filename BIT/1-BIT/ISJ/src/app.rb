#!/bin/sh
  exec ruby -x "$0" "$@"
#!ruby -w

#encoding:utf-8

# app.rb
# Autor:       Daniel Klimaj; xklima22@stud.fit.vutbr.cz
# Popis:       Vyhladavanie, stahovanie a porovnavanie titulkov k filmom.
# Pouzitie:    ./app.rb -t title_name
#              ./app.rb -u subtitle_url
#              ./app.rb -i imdb_url

require 'open-uri'
require 'nokogiri'
require 'mechanize'
require 'zip'
require 'fileutils'

require './lib/subtitles.rb'

abort("ERROR: Invalid parameters!") unless ARGV.length == 2

MODES = {
  title: 1,
  imdb:  2,
  url:   3,
}

mode       = nil
imdb_code  = nil
cs_sub_url = nil
title_name = nil

if ARGV.length == 2
  if ARGV[0] == '-t'
    mode = MODES[:title]
    title_name = ARGV[1]
  elsif ARGV[0] == '-u'
    mode = MODES[:url]
    cs_sub_url = ARGV[1]
  elsif ARGV[0] == '-i'
    mode = MODES[:imdb]
    imdb_code = ARGV[1]
  else
    abort("ERROR: Invalid parameters!")
  end
end

# URL
URL      = 'http://subscene.com'
IMDB_URL = 'http://www.imdb.com/title/'

# Cesta k tomuto skriptu
SCRIPT_PATH = File.expand_path(File.dirname(__FILE__))

# Cesty k priecinkom na disku
PATHS = {
  subs:      'subtitles',
  cs_subs:   'subtitles/cs',
  en_subs:   'subtitles/en',
  cs_subs_d: 'subtitles/cs/*',
  en_subs_d: 'subtitles/en/*',
  logs:      'logs',
  result:    'results',
}

# XPATHy pre vyhladavanie v HTML dokumentoch
XPATHS = {
  title1:   '//li[@class="header"]/h1/span',
  title2:   '//div[@class="header"]/h2',
  imdb1:    '//li[@class="header"]/h1/a[@class="imdb"]/@href',
  imdb2:    '//div[@class="header"]/h2/a[@class="imdb"]/@href',
  t_imdb:   '//table[@id="title-overview-widget-layout"]//h1[@class="header"]' +
            '/span[@itemprop="name"][last()]',
  subs:     '//a[@id="downloadButton"]/@href',
  header:   '//div[@class="byTitle"]/h1[contains(.,"Search results")]',
  exact:    '//div[@class="box"]/h2[@class="exact"]',
  ex_title: '//div[@class="box"]/h2[@class="exact"]/following-sibling::ul[1]/' +
            'li[1]/div[@class="title"]/a/@href',
  s_menu:   '//table/tbody/tr/td[@class="a1"]',
  ss_lang:  './a/div/span[@class]',
  ss_url:   './a/@href',
}

LANGS = {
  cs: 'Czech',
  en: 'English',
}

# Vypise hlavicku odseku vypisu
def print_header head
  puts
  puts "[#{head}]"
end

# Vrati celu cestu k local_path
def full_path local_path
  "#{SCRIPT_PATH}/#{local_path}"
end

# Vrati URL k stranke titulkov
def get_url_to_subtitles url
  doc  = Nokogiri::HTML(open(url))
  doc.at_xpath(XPATHS[:subs])
end

# Ziska meno titulu (filmu)
def get_title_name url, level=1
  doc  = Nokogiri::HTML(open(url))
  if level == 1
    return doc.at_xpath(XPATHS[:title1]).content.strip
  elsif level == 2
    elem = doc.at_xpath(XPATHS[:title2])
    elem.search('./a').each { |a| a.remove }
    return elem.content.strip
  else
    return nil
  end
end

# Ziska IMDB URL
def get_imdb_url url, level=1
  doc  = Nokogiri::HTML(open(url))
  if level == 1
    return doc.at_xpath(XPATHS[:imdb1])
  elsif level == 2
    return doc.at_xpath(XPATHS[:imdb2])
  else
    return nil
  end
end

# Ziska a zoznam titulkov a stiahne titulky vo vybranom jazyku
def get_subtitle_list doc, sub_lang=LANGS[:en]
  if sub_lang != LANGS[:en] && sub_lang != LANGS[:cs]
    abort('ERROR: Invalid language!')
  end

  sub_list = []

  # Hladaj anglicke titulky
  doc.xpath(XPATHS[:s_menu]).each do |subt|
    lang = subt.at_xpath(XPATHS[:ss_lang]).content.strip
    if lang == sub_lang
      subt_url = URL + subt.at_xpath(XPATHS[:ss_url])
      sub_list.push(subt_url)
    end
  end

  abort("ERROR: #{sub_lang} subtitles not found!") if sub_list.empty?

  # Stiahnutie najdenych anglickych titulkov
  i = 1
  sub_list.each do |sl|
    if sub_lang == LANGS[:en]
      path = PATHS[:en_subs] + '/' + i.to_s + '.zip'
      sub_url = get_url_to_subtitles(sl)
      download_subtitles(sub_url, path)
      puts "EN: Archive downloaded: #{full_path(path)}"
    else
      path = PATHS[:cs_subs] + '/' + i.to_s + '.zip'
      sub_url = get_url_to_subtitles(sl)
      download_subtitles(sub_url, path)
      puts "CS: Archive downloaded: #{full_path(path)}"
    end
    i = i + 1
  end
end

# Stiahne titulky
def download_subtitles url, path
  f = File.open(path, 'w+')
  subs  = Mechanize.new
  f << subs.get_file(URL + url)
end

# Rozbali stiahnute .zip archivy a odstrani vadne archivy (napr.: .rar)
def unzip_files directory
  files = Dir.entries(directory).select {|f| !File.directory? f}
  files.each do |file|
    fpath = directory + '/' + file
    dname = file.sub(/.zip$/, '')
    zpath = directory + '/' + dname
    begin
      Zip::File.open(fpath) do |zip_file|
        Dir.mkdir(zpath)
        zip_file.each do |zf|
          zf.extract(zpath + '/' + zf.name)
        end
        puts "File extracted successfully: #{full_path(directory)}/#{file}"
      end
    rescue
      puts "Removing invalid zip archive: #{full_path(directory)}/#{file}"
      FileUtils.rm(fpath)
      FileUtils.rm_rf(zpath) if File.directory?(zpath)
    end
  end
end

# Odstrani .zip archivy
def remove_archives directory
  puts "Removing archives in: #{full_path(directory)}"
  FileUtils.rm Dir.glob(directory + '/*.zip')
end

# Stiahne titulky na zaklade nazvu titulu (filmu)
def download_subtitles_by_title title
  agent    = Mechanize.new
  page     = agent.get(URL)
  page.forms[0].field_with(name: 'q').value = title
  titles   = page.forms[0].click_button
  uri      = titles.uri.to_s

  # Analyza vysledkov vyhladavania
  t_doc  = Nokogiri::HTML(open(uri))
  header = t_doc.at_xpath(XPATHS[:header])

  unless header.nil?
    exact  = t_doc.at_xpath(XPATHS[:exact])
    unless exact.nil?
      url = URL + t_doc.at_xpath(XPATHS[:ex_title])

      print_header("INFOBOX")
      release  = get_title_name url, 2
      imdb_url = get_imdb_url url, 2
      puts "Title:     #{release}"
      puts "IMDB link: #{imdb_url}"

      print_header('DOWNLOAD')
      s_doc = Nokogiri::HTML(open(url))
      get_subtitle_list(s_doc, LANGS[:cs])
      get_subtitle_list(s_doc)
    else
      abort('ERROR: Specified title not found!')
    end
  else
    print_header("INFOBOX")
    release  = get_title_name uri, 2
    imdb_url = get_imdb_url uri, 2
    puts "Title:     #{release}"
    puts "IMDB link: #{imdb_url}"

    print_header('DOWNLOAD')
    get_subtitle_list(t_doc, LANGS[:cs])
    get_subtitle_list(t_doc)
  end

  # Rozbal stiahnute archivy s titulkami
  print_header('EXTRACTION')
  unzip_files PATHS[:cs_subs]
  unzip_files PATHS[:en_subs]
  remove_archives PATHS[:cs_subs]
  remove_archives PATHS[:en_subs]
end

def find_max arr
  max = 0
  curr = nil

  arr.each do |a|
    if a[2] >= max
      max = a[2]
      curr = a
    end
  end

  curr
end

################################################################################
################################################################################

# Odstran priecinky s titulkami
FileUtils.rm_rf(PATHS[:subs])   if File.directory?(PATHS[:subs])
FileUtils.rm_rf(PATHS[:logs])   if File.directory?(PATHS[:logs])
FileUtils.rm_rf(PATHS[:result]) if File.directory?(PATHS[:result])

# Vytvor priecinky pre stahovanie titulkov
Dir.mkdir(PATHS[:subs])    unless File.directory?(PATHS[:subs])
Dir.mkdir(PATHS[:cs_subs]) unless File.directory?(PATHS[:cs_subs])
Dir.mkdir(PATHS[:en_subs]) unless File.directory?(PATHS[:en_subs])
Dir.mkdir(PATHS[:logs])    unless File.directory?(PATHS[:logs])

if mode == MODES[:url]
  unless cs_sub_url.split('/')[5] == 'czech'
    abort('ERROR: Specified URL is not for czech subtitles!')
  end
  # Zisti nazov titulu zo zadanej URL
  release  = get_title_name cs_sub_url
  imdb_url = get_imdb_url cs_sub_url
  subs_url = get_url_to_subtitles cs_sub_url

  print_header("INFOBOX")
  puts "Title:     #{release}"
  puts "IMDB link: #{imdb_url}"

  # Stiahni ceske titulky
  print_header('DOWNLOAD')
  path = PATHS[:cs_subs] + '/original.zip'
  download_subtitles(subs_url, path)
  puts "CS: Archive downloaded: #{full_path(PATHS[:cs_subs])}/original.zip"

  # Vyhladavanie anglickych titulkov
  agent    = Mechanize.new
  page     = agent.get(URL)
  page.forms[0].field_with(name: 'q').value = release
  titles   = page.forms[0].click_button
  uri      = titles.uri.to_s

  # Analyza vysledkov vyhladavania
  t_doc  = Nokogiri::HTML(open(uri))
  header = t_doc.at_xpath(XPATHS[:header])

  unless header.nil?
    exact  = t_doc.at_xpath(XPATHS[:exact])
    unless exact.nil?
      url = t_doc.at_xpath(XPATHS[:ex_title])
      s_doc = Nokogiri::HTML(open(URL + url))
      get_subtitle_list(s_doc)
    else
      abort('ERROR: Specified title not found!')
    end
  else
    get_subtitle_list(t_doc)
  end

  # Rozbal stiahnute archivy s titulkami
  print_header('EXTRACTION')
  unzip_files PATHS[:cs_subs]
  unzip_files PATHS[:en_subs]
  remove_archives PATHS[:cs_subs]
  remove_archives PATHS[:en_subs]
elsif mode == MODES[:title]
  download_subtitles_by_title title_name
elsif mode == MODES[:imdb]
  url = IMDB_URL + imdb_code
  begin
    i_doc = Nokogiri::HTML(open(url))
  rescue
    abort('ERROR: Invalid IMDB code!')
  end
  name = i_doc.at_xpath(XPATHS[:t_imdb]).content.strip
  download_subtitles_by_title name
else
  abort('ERROR: Invalid program mode!')
end

#Porovnanie titulkov
cs_dirs = Dir.glob(File.expand_path(PATHS[:cs_subs_d])).select {|f| File.directory? f}
en_dirs = Dir.glob(File.expand_path(PATHS[:en_subs_d])).select {|f| File.directory? f}

print_header("COMPARING FILES")
compares = []
cs_dirs.each do |cd|
  csubt = (Dir.entries(cd).select {|f| !File.directory? f})[0]
  csubt = cd + '/' + csubt
  next if csubt.split('.')[-1] != 'srt'
  cs_s = Subtitles.new(csubt)
  csdir_nr = cd.split('/')[-1]

  en_dirs.each do |ed|
    esubt = (Dir.entries(ed).select {|f| !File.directory? f})[0]
    esubt = ed + '/' + esubt
    next if esubt.split('.')[-1] != 'srt'
    en_s = Subtitles.new(esubt)
    endir_nr = ed.split('/')[-1]

    log_file = PATHS[:logs] + '/' + csdir_nr + '_' + endir_nr + '.log'
    puts "Comparing: \"#{csubt}\" with \"#{esubt}\""
    rslt = cs_s.compare(en_s, log_file)
    compares.push [csdir_nr, endir_nr, rslt, csubt, esubt]
  end
end

max = find_max compares
puts max

r_cs = full_path('results/cs/')
r_en = full_path('results/en/')
r_lg = full_path('results/log/')

Dir.mkdir(PATHS[:result]) unless File.directory?(PATHS[:result])
Dir.mkdir(r_cs)  unless File.directory?(r_cs)
Dir.mkdir(r_en)  unless File.directory?(r_en)
Dir.mkdir(r_lg) unless File.directory?(r_lg)

res_cs = r_cs + File.basename(max[3])
File.open(res_cs, 'w+')
FileUtils.cp(max[3], res_cs)
res_en = r_en + File.basename(max[4])
File.open(res_en, 'w+')
FileUtils.cp(max[4], res_en)
res_log = r_lg + 'log.log'
log_path = full_path("/logs/#{max[0]}_#{max[1]}.log")
File.open(res_log, 'w+')
FileUtils.cp(log_path, res_log)

print_header('RESULTS')
puts "Max match: #{(max[2] *100).round(2)}%"
puts "Results are stored in:"
puts res_cs
puts res_en
puts res_log

# app.rb
