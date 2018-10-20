#!/usr/bin/php
<?php
#XTD:xklima22

/**
 * Subject:      IPP
 * Project No.:  1
 * Task:         XTD
 * Author:       Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * File:         xtd.php
 * Description:  Main module.
 */

require 'params.php';
require 'ddl.php';

mb_internal_encoding("UTF-8");

Params::parse();

$tables = array();
$xml_str = "";

$xml_file = fopen(Params::get_infile(), 'r') or exit(2);

if(Params::input_defined())
{
  $xml_str = fread($xml_file, filesize(Params::get_infile()));
}
else
{
  while(!feof($xml_file))
  {
    $xml_str .= fgets($xml_file);
  }
}

if($xml_str != "")
{
  $xml_iterator = new SimpleXMLIterator($xml_str);
  $xml_iterator->rewind();
  loop_xml($xml_iterator, "", $tables, Params::ignore_attr_cols());
}

$text = "";
$out_file = Params::get_outfile();

if(Params::show_relations())
{
  $text = Table::show_relations($tables, Params::get_limit(), 4);
}
else
{
  if(Params::print_header())
  {
    $text .= Params::get_header();
  }
  $text .= Table::print_tables($tables, Params::merge_subelements(),
    Params::get_limit());
}

if($out_file == null)
{
  echo $text;
}
else
{
  $file = fopen($out_file, 'w') or exit(3);
  fwrite($file, $text);
  fclose($file);
}

/******************************************************************************/

/**
* Recursively loops through xml elements and maps data into Tables,
* Columns and Subelements.
* @param $element current XmlElement
* @param $parent name of parent element
* @param $tables reference to array of Tables, amy be altered
* @param $ignore_attributes skip attributes?, defaults to false
**/
function loop_xml($element, $parent, &$tables, $ignore_attributes = false)
{
  for($element->rewind(); $element->valid(); $element->next())
  {
    $name = strtolower(trim($element->current()->getName()));
    if(!Table::exists($name, $tables))
    {
      $tables[] = new Table($name);
    }

    $value = trim((string)$element->current());

    if($value != "")
    {
      $val_type = Column::determine_type($value, true);
      $table = &Table::find_by_name($name, $tables);
      $col = &$table->find_column("value");
      if($col == NULL)
      {
        $table->add_column(new Column("value", $val_type));
      }
      else
      {
        if($col->get_type() < $val_type)
        {
          $col->set_type($val_type);
        }
      }
    }

    if(!$ignore_attributes)
    {
      $attributes = $element->current()->attributes();
      foreach($attributes as $a => $v)
      {
        $attr_name = strtolower(trim($a));
        $attr_type = Column::determine_type($v);
        $table = &Table::find_by_name($name, $tables);
        $col = &$table->find_column($attr_name);
        if($col == NULL)
        {
          $table->add_column(new Column($attr_name, $attr_type));
        }
        else
        {
          if($col->get_type() < $attr_type)
          {
            $col->set_type($attr_type);
          }
        }
      }
    }

    if($element->hasChildren())
    {
      $children = $element->getChildren();
      $counts = array();

      foreach($children as $child)
      {
        $child_name = strtolower(trim($child->getName()));
        if(array_key_exists($child_name , $counts))
        {
          $counts[$child_name] += 1;
        }
        else
        {
          $counts[$child_name] = 1;
        }
      }

      $children->rewind();
      foreach($children as $child)
      {
        $child_name = strtolower(trim($child->getName()));
        $table = &Table::find_by_name($name, $tables);
        $subelement = &$table->find_subelement($child_name);

        if($subelement == null)
        {
          $table->add_subelement(new Subelement($child_name, 
            $counts[$child_name]));
        }
        else
        {
          if($subelement->get_max_count() < $counts[$child_name])
          {
            $subelement->set_max_count($counts[$child_name]);
          }
        }
      }
      loop_xml($children, $name, $tables, $ignore_attributes);
    }
  }
}

?>
