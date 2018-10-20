<?php

#XTD:xklima22

/**
 * Subject:      IPP
 * Project No.:  1
 * Task:         XTD
 * Author:       Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * File:         ucs.php
 * Description:  Provides functions for UCS search
 */

mb_internal_encoding("UTF-8");

class Ucs
{
  // PUBLIC STATIC FUNCTIONS

  /**
  * Performs UCS search. Looks for shortest path from $start Table to $end Table.
  * @param $start start Table
  * @param $end end Table
  * @param $tables array of Table instances
  * @return array of Tables or empty array if path does not exist
  **/
  public static function search($start, $end, $tables)
  {
    # Invalid table name(s)
    if(!self::is_valid($start, $tables) || !self::is_valid($end, $tables))
    {
      return array();
    }

    # Same tables
    if($start == $end)
    {
      return array($start);
    }

    # UCS search
    $open = array();
    $closed = array();

    $open[] = new UcsItem($start);

    while($open)
    {
      # Find minimal length
      $min = self::min($open);
      $tn = end($min->get_tables());

      # Search successful
      if($tn == $end)
      {
        return $min->get_tables();
      }

      # Continue searching, expand
      $tbl = Table::find_by_name($tn, $tables);
      $closed[] = $min;
      self::delete($open, $min);
      foreach($tbl->get_relations() as $rel)
      {
        if(in_array($rel->get_name(), $min->get_tables()))
        {
          continue;
        }

        $temp = $min->expand($rel->get_name());
        self::contains($open, $temp, true);
        if(!self::contains($closed, $temp))
        {
          $open[] = $temp;
        }
      }
    }

    # No path between tables
    return array();
  }

  // PRIVATE STATIC FUNCTIONS

  /**
  * Checks if $table is existing table.
  * @param $table Table instance
  * @param $tables array of Table instances
  * @return true if $table is in $tables
  **/
  private static function is_valid($table, $tables)
  {
    foreach($tables as $t)
    {
      if($t->get_name() == $table)
      {
        return true;
      }
    }

    return false;
  }

  /**
  * Looks for first path with minimal value.
  * @param $arr reference to array of UcsItem instances, may be altered
  * @return UcsItem instance with minimal value
  **/
  private static function min(&$arr)
  {
    $min = $arr[0];
    for ($i=1; $i<count($arr); $i++)
    {
      if($arr[$i]->get_length() < $min->get_length())
      {
        $min = $arr[$i];
      }
    }

    return $min;
  }

  /**
  * Checks if array of UcsItems $arr contains UcsItem $item.
  * @param $arr reference to array of UcsItem instances, may be altered
  * @param $item UcsItem
  * @param $alter bool, sets whether $arr items may be altered
  * @return true if $arr contains $item
  **/
  private static function contains(&$arr, $item, $alter = false)
  {
    for($i=0; $i<count($arr); $i++)
    {
      if($arr[$i]->cmp_tables($item->get_tables()))
      {
        if($alter && ($arr[$i]->get_length() > $item->get_length()))
        {
          $arr[$i]->set_length($item->get_length());
        }
        return true;
      }
    }

    return false;
  }

  /**
  * Deletes $item from array $arr. Restores indexes upon exit.
  * @param $arr reference to array of UcsItem instances, may be altered
  * @param $item UcsItem
  **/
  private static function delete(&$arr, $item)
  {
    for($i=0; $i<count($arr); $i++)
    {
      if($arr[$i]->cmp($item))
      {
        unset($arr[$i]);
        break;
      }
    }
    $arr = array_values($arr);
  }
} # Ucs

class UcsItem
{
  private $tables;
  private $length;

  // PUBLIC FUNCTIONS

  /**
  * Constructor.
  * @param $table Table instance or array of Table instances
  * @param $length integer, optional, defaults to 0
  **/
  public function __construct($table, $length = 0)
  {
    if(is_array($table))
    {
      $this->tables = $table;
      $this->length = $length;
    }
    else
    {
      $this->tables = array($table);
      $this->length = 0;
    }
  }

  /**
  * Getter for $tables field.
  * @return array of strings
  **/
  public function get_tables()
  {
    return $this->tables;
  }

  /**
  * Expands UcsItem.
  * @param $table string
  * @return new instance of UcsItem
  **/
  public function expand($table)
  {
    $new = new UcsItem($this->get_tables(), $this->get_length());
    $new->add_table($table);
    return $new;
  }

  /**
  * Compares this UcsItem with $other UcsItem.
  * @param $other UcsItem
  * @return true if this equals to $other
  **/
  public function cmp($other)
  {
    if($this->get_tables() == $other->get_tables() &&
    $this->get_length() == $other->get_length())
    {
      return true;
    }

    return false;
  }

  /**
  * Compares tables of this UcsItem with $tables.
  * @param $talbes UcsItem
  * @return true if tables of this UcsItem are the same as $tables
  **/
  public function cmp_tables($tables)
  {
    if($this->tables == $tables)
    {
      return true;
    }

    return false;
  }

  /**
  * Getter for $length field.
  * @return integer
  **/
  public function get_length()
  {
    return $this->length;
  }

  /**
  * Setter for $length field.
  * @param $length integer
  **/
  public function set_length($length)
  {
    $this->length = $length;
  }

  // PRIVATE FUNCTIONS

  /**
  * Adds $table into $tables array and increases length.
  * @param $talbe string
  **/
  private function add_table($table)
  {
    $this->tables[] = $table;
    $this->length += 1;
  }
} # UcsItem

?>
