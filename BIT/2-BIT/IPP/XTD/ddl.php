<?php

#XTD:xklima22

/**
 * Subject:      IPP
 * Project No.:  1
 * Task:         XTD
 * Author:       Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * File:         ddl.php
 * Description:  Classes representing entities used in DDL (Table, Column,
 *               Subelement (Reference), Relation).
 */

require "ucs.php";

mb_internal_encoding("UTF-8");

/******************************************************************************/
/***                          CLASS DATATYPE                                ***/
/******************************************************************************/
class DataType
{
  const T_Undef    = 0; # Default value
  const T_Bit      = 1;
  const T_Int      = 2;
  const T_Float    = 3;
  const T_Nvarchar = 4;
  const T_Text     = 5;

  /**
  * Converts numeric value to string representation of DataType.
  * @param $dt integer
  * @return string representation of DataType
  **/
  public static function to_string($dt)
  {
    switch($dt)
    {
      case 1:
        return "BIT";
      case 2:
        return "INT";
      case 3:
        return "FLOAT";
      case 4:
        return "NVARCHAR";
      case 5:
        return "NTEXT";
      default:
        return "UNDEFINED";
    }
  }
} # DataType

/******************************************************************************/
/***                           CLASS RELTYPE                                ***/
/******************************************************************************/
class RelType
{
  const R_Empty = 0; # Default value
  const R_11    = 1;
  const R_1N    = 2;
  const R_N1    = 3;
  const R_NM    = 4;

  /**
  * Converts numeric value to string representation of RelType.
  * @param $rt integer
  * @return string representation of RelType
  **/
  public static function to_string($rt)
  {
    switch($rt)
    {
      case 1:
        return "1:1";
      case 2:
        return "1:N";
      case 3:
        return "N:1";
      case 4:
        return "N:M";
      default:
        return "EPS";
    }
  }
} # RelType

/******************************************************************************/
/***                      ABSTRACT CLASS NAMEDCLASS                         ***/
/******************************************************************************/
abstract class NamedClass
{
  protected $name;

  /**
  * Constructor.
  * @param $name string
  **/
  public function __construct($name)
  {
    $this->name = $name;
  }

  /**
  * Getter for $name field.
  * @return $name
  **/
  public function get_name()
  {
    return $this->name;
  }
} # NamedClass

/******************************************************************************/
/***                             CLASS TABLE                                ***/
/******************************************************************************/
class Table extends NamedClass
{
  private $columns;
  private $subelements;
  private $relations;

  /**
  * Constructor.
  * @param $name string
  **/
  public function __construct($name)
  {
    parent::__construct($name);
    $this->columns     = array();
    $this->subelements = array();
    $this->relations   = array();
  }

  /**
  * Getter for $columns field.
  * @return $columns array
  **/
  public function get_columns()
  {
    return $this->columns;
  }

  /**
  * Getter for $subelements field.
  * @return $subelements array
  **/
  public function get_subelements()
  {
    return $this->subelements;
  }

  /**
  * Getter for $relations field.
  * @return $relations array
  **/
  public function get_relations()
  {
    return $this->relations;
  }

  /**
  * Adds relation $rel into $relations array.
  * @param $rel instance of Relation class
  **/
  public function add_relation($rel)
  {
    $this->relations[] = $rel;
  }

  /**
  * Adds relation $col into $columns array.
  * @param $col instance of Column class
  **/
  public function add_column($col)
  {
    $this->columns[] = $col;
  }

  /**
  * Adds relation $sub_el into $subelements array.
  * @param $sub_el instance of Subelement class
  **/
  public function add_subelement($sub_el)
  {
    $this->subelements[] = $sub_el;
  }

  /**
  * Checks if table contains column with name $col_name.
  * @param $col_name string
  * @return Column instance or null if column is not present in table
  **/
  public function find_column($col_name)
  {
    return self::find_by_name($col_name, $this->columns);
  }

  /**
  * Checks if table contains subelement with name $subelement_name.
  * @param $subelement_name string
  * @return Subelement instance or null if subelement is not present in table
  **/
  public function find_subelement($subelement_name)
  {
    return self::find_by_name($subelement_name, $this->subelements);
  }

  /**
  * Checks if table has relation with table $rel.
  * @param $rel string
  * @return Relation instance or null if relation does not exist
  **/
  public function find_relation($rel)
  {
    return self::find_by_name($rel, $this->relations);
  }

  // PRIVATE FUNCTIONS

  /**
  * Returns string representation of table and it's contents.
  * @param $merge if true then subelements with multiple occurences will
  * be reduced into one occurence
  * @return string
  **/
  private function to_string($merge = false)
  {
    $str = "CREATE TABLE " . $this->get_name() . "(\n";
    $str .= "\tprk_" . $this->get_name() . "_id INT PRIMARY KEY";
    
    foreach($this->columns as $col)
    {
      $str .= ",\n\t" . $col->get_name() . " " .
        DataType::to_string($col->get_type());
    }

    if($merge)
    {
      foreach($this->subelements as $se)
      {
        $str .= ",\n\t" . $se->get_name() . "_id INT";
      }
    }
    else
    {
      foreach($this->subelements as $se)
      {
        if($se->get_max_count() == 1 || $se->get_max_count() == -1)
        {
          $str .= ",\n\t" . $se->get_name() . "_id INT";
        }
        else
        {
          for($i = 1; $i <= $se->get_max_count(); $i++)
          {
            $str .= ",\n\t" . $se->get_name() . $i . "_id INT";
          }
        }
      }
    }

    $str .= "\n);\n\n";

    return $str;
  }

  /**
  * Removes table's subelements which exceedes $limit and creates reference
  * of table in subelement's table.
  * @param $limit integer
  * @param $tables reference to array of tables, tables may be altered
  **/
  private function limit_columns($limit, &$tables)
  {
    $subelements = $this->get_subelements();
    $count = count($subelements);

    if($count > 0)
    {
      for($i=($count-1); $i>=0; $i--)
      {
        if($subelements[$i]->get_max_count() > $limit)
        {
          $se_table = &self::find_by_name($subelements[$i]->get_name(), $tables);
          $se_table->add_subelement(new Subelement($this->get_name(), -1));
          unset($subelements[$i]);
          $this->set_subelements($subelements);
        }
      }
    }
  }

  /**
  * Sets subelements array to $se_list.
  * @param $se_list array of Subelement instances
  **/
  private function set_subelements($se_list)
  {
    $this->subelements = $se_list;
  }

  /**
  * Looks for direct relations of table. If relation is found then is added
  * into relations array.
  * @param $tables array of Table instances
  **/
  private function find_direct_relations($tables)
  {
    # Add 1:1 relation
    $this->add_relation(new Relation($this->get_name(), RelType::R_11));

    # Find N:M & N:1 relations
    foreach($this->get_subelements() as $se)
    {
      $se_table = self::find_by_name($se->get_name(), $tables);
      if($se_table)
      {
        $sse = $se_table->find_subelement($this->get_name());
        if($sse)
        {
          $this->add_relation(new Relation($se->get_name(), RelType::R_NM));
        }
        else
        {
          $this->add_relation(new Relation($se->get_name(), RelType::R_N1));
        }
      }
    }

    # Find 1:N relations
    foreach($tables as $table)
    {
      if($table->get_name() != $this->get_name())
      {
        $se = $table->find_subelement($this->get_name());
        if($se)
        {
          $this->add_relation(new Relation($table->get_name(), RelType::R_1N));
        }
      }
    }
  }

  /**
  * Looks for transitive relations of table with other tables. Uses UCS
  * algorithm to find the shortest path to other tables. Requires existing
  * direct relations to work.
  * @param $tables array of Table instances
  **/
  private function find_indirect_relations($tables)
  {
    # Find transitive relations
    foreach($tables as $table)
    {
      if($this->find_relation($table->get_name()) == null)
      {
        $rel_type = $this->determine_relation_to($table, $tables);
        if($rel_type != RelType::R_Empty)
        {
          $this->add_relation(new Relation($table->get_name(), $rel_type));
        }
      }
    }
  }

  /**
  * Determines type of transitive relation of table to $table.
  * @param $table other table
  * @param $tables array of Table instances
  * @return RelType
  **/
  private function determine_relation_to($table, $tables)
  {
    $path = Ucs::search($this->get_name(), $table->get_name(), $tables);

    $rel = RelType::R_Empty;
    if($path)
    {
      for($i=0; $i<(count($path)-1); $i++)
      {
        $t = Table::find_by_name($path[$i], $tables);
        $r = $t->find_relation($path[$i+1]);

        if($rel != RelType::R_Empty)
        {
          if($rel != $r->get_type())
          {
            $rel = RelType::R_NM;
          }
        }
        else
        {
          $rel = $r->get_type();
        }
      }
    }

    return $rel;
  }

  /**
  * Checks if table has collision of column names with subelement names.
  * @return true if there is collision
  **/
  private function check_collision()
  {
    foreach($this->get_columns() as $col)
    {
      foreach($this->get_subelements() as $se)
      {
        if($col->get_name() == ($se->get_name() . "_id"))
        {
          return true;
        }
      }
    }

    return false;
  }

  // PUBLIC STATIC FUNCTIONS

  /**
  * Looks for object with $name in array $arr
  * @param $name object's name
  * @param $arr array of objects, must be instance of NamedClass
  * @return object or null
  **/
  public static function find_by_name($name, $arr)
  {
    if(is_array($arr))
    {
      foreach($arr as $a)
      {
        if($a->get_name() == $name)
        {
          return $a;
        }
      }
    }

    return null;
  }

  /**
  * Checks if object with $name is present in array $arr
  * @param $name object's name, must be instance of NamedClass
  * @param $arr array of objects
  * @return true if object is in array $arr
  **/
  public static function exists($name, $arr)
  {
    if(self::find_by_name($name, $arr) != null)
    {
      return true;
    }

    return false;
  }

  /**
  * Loops $tables and prints their structure.
  * @param $tables reference to array of Table instances, may be altered
  * @param $merge optional, sets whether subelements with multiple occurence
  * be merged.
  * @param $limit optional, sets limit for subelements with multiple occurences
  * @return structure of tables as string
  **/
  public static function print_tables(&$tables, $merge = false, $limit = -1)
  {
    foreach($tables as $table)
    {
      if($table->check_collision())
      {
        exit(90);
      }
    }

    if($limit >= 0)
    {
      foreach($tables as $table)
      {
        $table->limit_columns($limit, $tables);
      }
    }

    foreach($tables as $table)
    {
      if($table->check_collision())
      {
        exit(90);
      }
    }

    $str = "";
    foreach($tables as $table)
    {
      $str .= $table->to_string($merge);
    }

    return $str;
  }

  /**
  * Loops $tables and prints their relations to other tables.
  * @param $tables reference to array of Table instances, may be altered
  * @param $limit optional, sets limit for subelements with multiple occurences
  * @param $tab_width optional, sets tab with in output string
  * @return relations of tables as xml string
  **/
  public static function show_relations(&$tables, $limit = -1, $tab_width = 2)
  {
    foreach($tables as $table)
    {
      if($table->check_collision())
      {
        exit(90);
      }
    }

    if($limit >= 0)
    {
      foreach($tables as $table)
      {
        $table->limit_columns($limit, $tables);
      }
    }

    foreach($tables as $table)
    {
      if($table->check_collision())
      {
        exit(90);
      }
    }

    foreach($tables as $table)
    {
      $table->find_direct_relations($tables);
    }

    foreach($tables as $table)
    {
      $table->find_indirect_relations($tables);
    }

    $xml_text = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<tables>\n";
    foreach($tables as $table)
    {
      $xml_text .= "\t<table name=\"" . $table->get_name() . "\">\n";
      foreach($table->get_relations() as $rel)
      {
        $xml_text .= "\t\t<relation ";
        $xml_text .= "to=\"" . $rel->get_name() . "\" ";
        $xml_text .= "relation_type=\"" .
          RelType::to_string($rel->get_type()) . "\" />\n";
      }
      $xml_text .= "\t</table>\n";
    }
    $xml_text .= "</tables>\n";

    $spaces = "";
    for($i=0; $i<$tab_width; $i++) {$spaces .= " ";}
    $xml_text = str_replace("\t", $spaces, $xml_text);

    return $xml_text;
  }
} # Table

/******************************************************************************/
/***                             CLASS COLUMN                               ***/
/******************************************************************************/
class Column extends NamedClass
{
  private $type;
  private $is_val;

  // PUBLIC FUNCTIONS

  /**
  * Constructor.
  * @param $name string
  * @param $type DataType
  **/
  public function __construct($name, $type)
  {
    parent::__construct($name);
    $this->type   = $type;
  }

  /**
  * Getter for $type field.
  * @return $type DataType
  **/
  public function get_type()
  {
    return $this->type;
  }

  /**
  * Setter for $type field.
  * @param $type DataType
  **/
  public function set_type($type)
  {
    $this->type = $type;
  }

  // PUBLIC STATIC FUNCTIONS

  /**
  * Determines DataType of column by it's $value.
  * @param $value value of column
  * @param $is_text boolean, sets whether it's text element
  * @return DataType
  **/
  public static function determine_type($value, $is_text = false)
  {
    $value = strtolower(trim($value));
    if($value == "0" || $value == "1" || $value == "true" || $value == "false")
    {
      return DataType::T_Bit;
    }
    elseif(preg_match("/^\d+$/", $value))
    {
      return DataType::T_Int;
    }
    elseif(preg_match("/^[+|-]?\d+\.\d+$/", $value) ||
    preg_match("/^[+|-]?\d+\.\d+e[+|-]?\d+$/", $value))
    {
      return DataType::T_Float;
    }
    elseif($is_text)
    {
      return DataType::T_Text;
    }
    else
    {
      return DataType::T_Nvarchar;
    }
  }
} # Column

/******************************************************************************/
/***                           CLASS SUBELEMENT                             ***/
/******************************************************************************/
class Subelement extends NamedClass
{
  private $count;

  /**
  * Constructor.
  * @param $name string
  * @param $count integer, maximal number of occurences of subelement within
  * one table
  **/
  public function __construct($name, $count)
  {
    parent::__construct($name);
    $this->count  = $count;
  }

  /**
  * Getter for $count filed.
  * @return integer
  **/
  public function get_max_count()
  {
    return $this->count;
  }

  /**
  * Setter for $count filed.
  * @param $count integer
  **/
  public function set_max_count($count)
  {
    $this->count = $count;
  }
} # Subelement

/******************************************************************************/
/***                           CLASS RELATION                               ***/
/******************************************************************************/
class Relation extends NamedClass
{
  private $type;

  /**
  * Constructor.
  * @param $name string
  * @param $type RelType
  **/
  public function __construct($name, $type = R_Empty)
  {
    parent::__construct($name);
    $this->type = $type;
  }

  /**
  * Getter for $type field.
  * @return RelType
  **/
  public function get_type()
  {
    return $this->type;
  }
} # Relation

?>
