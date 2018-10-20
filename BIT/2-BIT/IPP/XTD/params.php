<?php

#XTD:xklima22

/**
 * Subject:      IPP
 * Project No.:  1
 * Task:         XTD
 * Author:       Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * File:         params.php
 * Description:  Provides function for parameter parsing, sets appropriate
 *               variables by parameters and provides accessors for these
 *               variables.
 */

mb_internal_encoding("UTF-8");

class Params
{
  // FIELDS

  private static $a_switch      = false;
  private static $b_switch      = false;
  private static $g_switch      = false;
  private static $help_switch   = false;
  private static $input_switch  = false;
  private static $output_switch = false;
  private static $header_switch = false;
  private static $etc_switch    = false;

  private static $input_file    = "";
  private static $output_file   = "";
  private static $etc_n         = -1;
  private static $header        = "--";
  private static $stdout        = false;

  // PUBLIC STATIC FUNCTIONS

  /**
  * Parses script params and sets appropriate values. May exit application
  * when wrong combination of parameters is used.
  **/
  public static function parse()
  {
    $shortops = "abg";
    $longopts = array(
      "help",
      "input:",
      "output:",
      "header:",
      "etc:"
    );

    $options = getopt($shortops, $longopts);

    foreach($options as $opt => $optarg)
    {
      switch($opt)
      {
        case 'a':
          self::$a_switch = true;
          break;
        case 'b':
          self::$b_switch = true;
          break;
        case 'g':
          self::$g_switch = true;
          break;
        case 'help':
          self::$help_switch = true;
          break;
        case 'input':
          self::$input_switch = true;
          self::$input_file = self::abs_path($optarg);
          break;
        case 'output':
          self::$output_switch = true;
          self::$output_file = self::abs_path($optarg);
          break;
        case 'etc':
          self::$etc_switch = true;
          self::$etc_n = $optarg;
          break;
        case 'header':
          self::$header_switch = true;
          self::$header .= $optarg;
          self::$header .= "\n\n";
          break;
      }
    }

    if(self::$help_switch && (self::$a_switch || self::$b_switch ||
    self::$g_switch || self::$input_switch || self::$output_switch ||
    self::$etc_switch || self::$header_switch))
    {
      exit(1);
    }

    if(self::$help_switch)
    {
      self::print_help();
      exit(0);
    }

    if(!self::$input_switch)
    {
      self::$input_file = "php://stdin";
    }

    if(self::$b_switch && self::$etc_switch)
    {
      exit(1);
    }

    if(self::$etc_switch)
    {
      if(!preg_match("/^\d+$/", self::$etc_n))
      {
        exit(1);
      }
      self::$etc_n = (int)self::$etc_n;
    }

    if(self::$input_switch && !file_exists(self::$input_file))
    {
      exit(2);
    }

    if(!self::$output_switch)
    {
      self::$stdout = true;
    }
  }

  /**
  * Checks whether input file is defined. Affected by --input="fn" parameter.
  * @return true if input file is defined
  **/
  public static function input_defined()
  {
    return self::$input_switch;
  }

  /**
  * Gets header string. Affected by --header parameter.
  * @return header string
  **/
  public static function get_header()
  {
    return self::$header;
  }

  /**
  * Gets input file path. Affected by --input="fn" parameter.
  * @return input file path
  **/
  public static function get_infile()
  {
    return self::$input_file;
  }

  /**
  * Gets output file path. Affected by --output="fn" parameter.
  * @return output file path or null if not defined
  **/
  public static function get_outfile()
  {
    if(self::$output_switch)
    {
      return self::$output_file;
    }
    
    return null;
  }

  /**
  * Gets limit of same name attribute columns.
  * Affected by --etc=n parameter.
  * @return limit
  **/
  public static function get_limit()
  {
    return self::$etc_n;
  }

  /**
  * Checks whether script should ignore attribute columns.
  * Affected by -a parameter
  * @return true if attribute columns should be ignored
  **/
  public static function ignore_attr_cols()
  {
    return self::$a_switch;
  }

  /**
  * Checks whether script should merge multiple reference columns into one.
  * Affected by -b parameter.
  * @return true if references shold be merged
  **/
  public static function merge_subelements()
  {
    return self::$b_switch;
  }

  /**
  * Checks whether script should show relations between tables.
  * Affected by -g parameter.
  * @return true if relations should be shown
  **/
  public static function show_relations()
  {
    return self::$g_switch;
  }

  /**
  * Checks whether script should add header at the beginning of output.
  * Affected by --header parameter.
  * @return true header should be added to output
  **/
  public static function print_header()
  {
    return self::$header_switch;
  }

  /**
  * Prints help. Affected by --help parameter.
  **/
  public static function print_help()
  {
    echo "Project:\tXTD - XML to DDL\n" .
    "Author:\t\tDaniel Klimaj; xklima22@stud.fit.vutbr.cz\n" .
    "Date:\t\t2015-03-06\n\n" .
    "Usage: xtd.php [--help] [--input=\"infile\" [--output=\"outfile\"] " .
    "[-a] [-b] [-etc=n] [-g] [--header=\"text\"]]\n\n" .
    "Parameters:\n" .
    "\t--help\t\t\tPrints this message\n" .
    "\t--input=\"infile\"\tXML file to be converted to DDL\n" .
    "\t--output=\"outfile\"\tFile to print output in\n" .
    "\t-a\t\t\tIgnore attributes\n" .
    "\t-b\t\t\tMerge columns with same name\n" .
    "\t-g\t\t\tShow relations\n" .
    "\t--etc=n\t\t\tLimit same name foreign keys to n\n" .
    "\t--header\t\tAdds header with \"text\" to output\n";
  }

  // PRIVATE STATIC FUNCTIONS

  /**
  * Creates absolute path out of $path.
  * @param $path string
  * @return absolute path
  **/
  private static function abs_path($path)
  {
    if($path[0] == ".")
    {
      $path = dirname(__FILE__) . "/" . $path;
    }

    $parts = explode("/", $path);
    $newpath = array();
    foreach($parts as $part)
    {
      if($part == ".")
      {
        continue;
      }
      elseif($part == "..")
      {
        array_pop($newpath);
      }
      else
      {
        $newpath[] = $part;
      }
    }

    $path = implode("/", $newpath);
    return $path;
  }
} # Params

?>
